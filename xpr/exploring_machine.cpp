#include "exploring_machine.h"
//-----------------------------------------------------------------------++
///experimental
#include "alcor/core/pixel_traits.h"
#include "alcor/core/depth_image_utils.hpp"
//---------------------------------------------------------------------------
namespace all { namespace  xpr {
//---------------------------------------------------------------------------
  //ctor
  exploring_machine::exploring_machine()
  {
  //Idling ... default
  fire_callback = boost::bind
    (&exploring_machine::idled_cb, this);

  //Task Listener
  tasklistener.reset(new task_listener("config/xrp_service.ini"));
  tasklistener->notify_evt = boost::bind(&exploring_machine::taskreceived, this, ::_1);
  tasklistener->run_async();

  //bumblebee
  bee.reset(new sense::bumblebee_driver_t());
  bee->open("config/bumblebeeB.ini");

  //PTU
  ptu.reset (new act::directed_perception_ptu_t);
  ptu->open("config/dpptu_conf.ini");

  //PINHOLE
  pinhole.focal = bee->focal();
  pinhole.ncols = bee->ncols();
  pinhole.nrows = bee->nrows();

 //P3AT
  p3at.reset(new act::p3_gateway()); 

  ///
  if (p3at->open("config/p3at_conf.ini"))
        printf("Robot connected!\n");

  ///P3AT server
  p3at_server.reset(new act::p3_server_t("config/p3at_conf.ini"));
  p3at_server->set_gateway_ptr(p3at);
  p3at_server->run_async();

  //STREAMING
  rgb_stream_source_ptr.reset(new all::core::memory_stream_source_t( bee->nrows(), bee->ncols() ) );
  rgb_stream_server_ptr = new all::core::stream_server_t(rgb_stream_source_ptr,"config/bumblebee_stream_server.ini");
  rgb_stream_server_ptr->run_async();

  ///SPLAM STREAMING
  splam_stream_source_ptr.reset(new all::core::memory_stream_source_t( splam.get_row(), splam.get_col() ) );
  splam_stream_server_ptr = new all::core::stream_server_t(splam_stream_source_ptr,"config/.....ini");
  splam_stream_server_ptr->run_async();

  ///Streaming OPen GL outside???

  
  //Initiate thread
  thisthread.reset( 
      new boost::thread
      (
          boost::bind(&exploring_machine::threadloop, this) 
      )
    ); 
  }
//---------------------------------------------------------------------------
  ///
  void exploring_machine::taskreceived(int evt)
  {
    boost::mutex::scoped_lock lock(process_guard);

    switch (evt)
    {  
    case etag::RESET :
      process_event(exploring_machine::reset_evt() );
      break;

    case etag::IDLE :
      process_event(exploring_machine::idle_evt() );
      break;

    case etag::EXPLORE :
      process_event(exploring_machine::explore_evt() );
      break;

    case etag::RESUME :
      process_event(exploring_machine::resume_evt() );
      break;

    case etag::VISIT :
      process_event(exploring_machine::visit_evt() );
      break;

    default:
      break;
    }
  }
//---------------------------------------------------------------------------
void exploring_machine::threadloop()
  {
  ///
  workspace.reset( new matlab::matlab_engine_t);
  ///
  workspace->command_line("cd explore");
  ///
  while (running_)
  {
    {
      boost::mutex::scoped_lock lock(process_guard);
      if(!fire_callback.empty())
            fire_callback();    
    }
  boost::thread::yield();    
  core::BOOST_SLEEP(50);
  }
  }
//---------------------------------------------------------------------------
  //[FIRE callbacks]
//---------------------------------------------------------------------------
  //idled
  void exploring_machine::idled_cb()
  {
  if (bee->grab())
  {
    rightim = bee->get_color_buffer(core::right_img);
    rgb_stream_source_ptr->update_image(rightim);
  }
  }
//---------------------------------------------------------------------------
  ///
  void exploring_machine::exploring_cb()
  {
  math::angle pana_;
  math::angle tilta_;

  //center pantilt, depending on last glo_theta_target and theta_rob
  splam.get_splam_data(splamdata);
  //
  math::pose2d robotpose = splamdata.get_current_position();

  //to compensate
  pana_.set_deg(glo_theta_target.deg() - robotpose.getTh().deg());

  //compensate!!
  ptu->set_pan(pana_.deg()); 

  if (bee->grab())
  {
    rightim = bee->get_color_buffer(core::right_img);
    rgb_stream_source_ptr->update_image(rightim);
  }
   
  }
//---------------------------------------------------------------------------
  ///
  void exploring_machine::observing_cb()
  {
    //
    splam.get_splam_data(splamdata);
    //
    math::pose2d robotpose = splamdata.get_current_position();
    //
    core::pantilt_angle_t currentptu = ptu->get_fast_pantilt();

    //GRAB
    if (bee->grab())
    {
      ///GET
      rightim = 
        bee->get_color_buffer(core::right_img);
      ///
      depthim =
        bee->get_depth_buffer();

      //ADAPT
      ///transform
      mxArray* mx_rimage = 
        matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                            ,matlab::row_major
                                                            ,bee->nrows()
                                                            ,bee->ncols());
      //XYZ
      mxArray* mx_xyz =
      matlab::buffer2array<core::single_t>::create_from_planar(
                              depthim.get()
                            , matlab::row_major
                            ,bee->nrows()
                            ,bee->ncols());


      //WORKSPACE
      //Push into Workspace
      workspace->put_array("xyz", mx_xyz);
      //Push into Workspace
      workspace->put_array("rgb", mx_rimage);
      
      ///COMMANDs
      workspace->command_line
        ("[valX, valY, prob, odimX, odimY] = fair_attention_wrap(rgb, xyz)");

      //***GATHER***
      valX = 
        static_cast<int> (workspace->get_scalar_double("valX") );

      valY = 
        static_cast<int> (workspace->get_scalar_double("valY") );
      
      prob = 
        static_cast<int> (workspace->get_scalar_double("prob") );

      odimX = 
        static_cast<int> (workspace->get_scalar_double("odimX") );

      odimY = 
        static_cast<int> (workspace->get_scalar_double("odimY") );

      //first, center the ptu on the target/roi
      move_ptu_to_screen_rc(valX, valY );

      //Profondità 3D
      core::depth_image_t mydepthim;
      mydepthim.assign(bee->nrows(), bee->ncols(), depthim.get());

      //centro dell'intorno
      core::pixelcoord_t center;
      center.row = valX;
      center.col = valY;

      //raggio dell'intorno
      size_t  hsize =  (odimX > odimY) ? (odimX):(odimY);
      //
      core::mystat vstat  
        = core::estimate_depth(mydepthim, center, hsize);

      //---------------------------
      double distanza = vstat.mean;

      //angolo (all::math::angle) rispetto il centro dell'immagine dell'oggetto visto
      math::angle th ( pinhole.delta_pan_from_pixel(valY) , math::deg_tag) ;

      ////local offset (that is respect to robot nose)
      double loc_theta_target = currentptu.get_pan_deg() + th.deg(); 

      ////theta globale (useful for next iteration)
      glo_theta_target.set_deg( loc_theta_target + robotpose.get_th(math::deg_tag) );

      // relative_goal = goal relativo... costruttore con modulo ed angolo
      all::math::point2d relative_goal(distanza/2.0, glo_theta_target);

      //SETPOINT NAVIGAZIONE
      p3at->set_relative_goto(math::point2d(0,0) , 0);

      //
      rgb_stream_source_ptr->update_image(rightim);

      ///SWITCH TO EXPLORE ...mmmmm
      //fire_callback = boost::bind
      //  (&exploring_machine::exploring_cb, this);

      printf("Navigate with setpoint\n");
      process_event(exploring_machine::explore_evt());

    }
    else
    {
      printf("cannot grab!\n");
      process_event(exploring_machine::reset_evt());
    }
  }

  ///
  void exploring_machine::visiting_cb()
  {
  }
//---------------------------------------------------------------------------
  //[TRANSITION FUNCTIONS]
  //
  bool exploring_machine::go_explore (explore_evt const&)
  {
    //
    p3at->enable_goto_mode();
    //
    p3at->set_relative_goto(math::point2d(0,0) , 0);
    //
    boost::mutex::scoped_lock lock(process_guard);
    ///PROVVISORIO
    fire_callback = boost::bind
      (&exploring_machine::observing_cb, this);

    return true;
  }
//---------------------------------------------------------------------------
  ///
  bool exploring_machine::go_explore  (resume_evt  const&)
  {
    //
    boost::mutex::scoped_lock lock(process_guard);
    ///
    fire_callback = boost::bind
      (&exploring_machine::observing_cb, this);

    return true;
  }
//---------------------------------------------------------------------------
  //
  bool exploring_machine::go_idle (idle_evt const&)
  {
    //
    p3at->enable_stop_mode();
    //
    boost::mutex::scoped_lock lock(process_guard);
    ///
    fire_callback = boost::bind
      (&exploring_machine::idled_cb, this);
    return true;
  }
  //---------------------------------------------------------------------------
  //
  bool exploring_machine::go_idle    (reset_evt const&)
  { 
    //
    p3at->enable_stop_mode();
    //
    boost::mutex::scoped_lock lock(process_guard);
    ///
    fire_callback = boost::bind
      (&exploring_machine::idled_cb, this);
    return true;
  }
//---------------------------------------------------------------------------
  //
  bool exploring_machine::go_visit   (visit_evt const&)
  {
    return true;
  }

//---------------------------------------------------------------------------
//###########################################################################
  void exploring_machine::move_ptu_to_screen_rc(float row, float col)
{
  if(ptu)
  {
    //
    core::pantilt_angle_t current = 
      ptu->get_fast_pantilt();
    //
    core::pantilt_angle_t delta;
    //
    pinhole.pantilt_from_pixel(row, col, delta);
    //
    float nupan  = delta.get_pan_deg() + current.get_pan_deg();
    float nutilt = delta.get_tilt_deg()+ current.get_tilt_deg();
    
    //
    ptu->set_pantilt(nupan, nutilt);

    //printf("Row: %f Col: %f \n", row, col);
    //printf("current: %.2f %.2f\n", current.get_pan_deg(),current.get_tilt_deg());
    //printf("Delta:   %.2f %.2f\n",  delta.get_pan_deg(), delta.get_tilt_deg());
  }
}
//---------------------------------------------------------------------------
}}//all::xpr
//---------------------------------------------------------------------------