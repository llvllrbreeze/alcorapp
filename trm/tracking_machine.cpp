#define WIN32_LEAN_AND_MEAN

#include "tracking_machine.h"
//#include "alcor.apps/trm/gil_wrap_utils.hpp"
//---------------------------------------------------------------------------
namespace all { namespace trm {
//---------------------------------------------------------------------------

tracking_machine::tracking_machine():running_(true)
{
  ////
  fire_callback = boost::bind
    (&tracking_machine::idle_cb, this);


  //Task Listener
  tasklistener.reset(new task_listener("config/trm_service.ini"));
  tasklistener->notify_evt = boost::bind(&tracking_machine::taskreceived, this, ::_1);
  tasklistener->notify_roi = boost::bind(&tracking_machine::setup_roi, this, ::_1,::_2,::_3,::_4);
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

   //P3DX
  p3dx.reset(new act::p3_gateway()); 
  if (p3dx->open("config/p3_conf.ini"))
    printf("Robot connected!\n");

  //p3_adapter.reset(new act::p3_odometry_adapter_t(p3dx) );
  //ptu_control.reset(new act::pantilt_control_loop_t );
  //ptu_control->set_ptu(ptu);
  //ptu_control->set_slam(p3_adapter);
  
  ////open views
  //rgb_win.reset(new cimglib::CImgDisplay(bee->ncols(), bee->nrows(), "rgb"));
  //rgb_cimg.reset(new cimglib::CImg<core::uint8_t>());

  //Streaming
  stream_source_ptr.reset(new all::core::memory_stream_source_t( bee->nrows(), bee->ncols() ) );
  stream_server_ptr = new all::core::stream_server_t(stream_source_ptr,"config/trm_stream_server.ini");
  stream_server_ptr->run_async();

  thisthread.reset( 
      new boost::thread
      (
          boost::bind(&tracking_machine::threadloop, this) 
      )
    );  
}
//---------------------------------------------------------------------------
tracking_machine::~tracking_machine()
{
  if (stream_server_ptr)
  {
    stream_server_ptr->stop();
    core::BOOST_SLEEP(200);
    delete stream_server_ptr;
  }
}
//---------------------------------------------------------------------------
void tracking_machine::threadloop()
  {
  ///
  workspace.reset( new matlab::matlab_engine_t);
  workspace->command_line("cd tracking");
  workspace->command_line("init_tracking");


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
//###########################################################################
//CALLBACK
//---------------------------------------------------------------------------
void tracking_machine::idle_cb()
{
  if (bee->grab())
  {
    core::uint8_sarr rightim = bee->get_color_buffer(core::right_img);
    stream_source_ptr->update_image(rightim);
  }
}
//---------------------------------------------------------------------------
void tracking_machine::setup_cb()
{
  printf("Roi selezionato: r: %d c: %d h:%d w: %d\n", 
    r_roi, c_roi, h_roi, w_roi);

  //half height
  int h_2 = h_roi/2;
  //half width
  int w_2 = w_roi/2;
  //
  double centro_r = r_roi + h_2;
  double centro_c = c_roi + w_2;

  //forst, center the ptu on the target/roi
  move_ptu_to_screen_rc(centro_r, centro_c, 1.5);

  //then grab again and launch model setup
  if (bee->grab())
  {
    core::uint8_sarr rightim = 
      bee->get_color_buffer(core::right_img);

    mxArray* mx_rimage = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                          ,matlab::row_major
                                                          ,bee->nrows()
                                                          ,bee->ncols());
    
    //Push into Workspace
    workspace->put_array("rgb", mx_rimage);

    //update roi data
    
todo:

    //ROI
    workspace->put_scalar("r_roi", r_roi);
    workspace->put_scalar("c_roi", c_roi);
    workspace->put_scalar("h_roi", h_roi);
    workspace->put_scalar("w_roi", w_roi);

    printf("SETUP\n");

    ////
    workspace->command_line
      ("[centro_r centro_c] = trm_model_setup(rgb, r_roi, c_roi, h_roi, w_roi, 0.5)");
    ///

    printf("Done Setup\n\n");

    double  centro_r = workspace->get_scalar_double("centro_r");
    double  centro_c = workspace->get_scalar_double("centro_c");

    //Se tutto a posto vai in idled_tracking
    process_event(idle_track_event());
    //}
    //else
    //  process_event(reset_event());
  }
  else
  {
    printf("\nERROR : Cannot GRAB!\n");
    process_event(reset_event());
  }
}
//---------------------------------------------------------------------------
void tracking_machine::tracking_cb()
{    

  if (bee->grab())
  {
    //
    core::pantilt_angle_t current_pt = 
      ptu->get_fast_pantilt();

    core::uint8_sarr rightim = bee->get_color_buffer(core::right_img);

    mxArray* mx_rimage = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                          ,matlab::row_major
                                                          ,bee->nrows()
                                                          ,bee->ncols());
    //
    stream_source_ptr->update_image(rightim);

    //Push into Workspace
    workspace->put_array("rgb", mx_rimage);

    ///MEAN SHIFT : ottenere un centro
    workspace->command_line("[centro_r centro_c]= trm_track(rgb, [120, 160])");

    //***GATHER***
    int centro_r = 
      static_cast<int> (workspace->get_scalar_double("centro_r") );

    int centro_c = 
      static_cast<int> (workspace->get_scalar_double("centro_c") );

  if(centro_c > 0)//che non sia nullo ....
  {
    //
    move_ptu_to_screen_rc(centro_r, centro_c);

    //***Target Angular SET POINT***    
    //double theta_target= current_pan + pan_delta;
    double pan_delta     = pinhole.delta_pan_from_pixel(centro_c);
    double th_robot      = p3dx->get_odometry().getTh().deg();

    //theta globale
    double loc_theta_target = current_pt.get_pan(math::deg_tag) +pan_delta;
    double glo_theta_target = loc_theta_target + th_robot;
    //
    ///ProfonditÓ 3D
    core::depth_image_t depthim;
    core::single_sarr depth = bee->get_depth_buffer();

    depthim.assign(bee->nrows(), bee->ncols(), depth.get());

    //centro dell'intorno
    core::pixelcoord_t center;
    center.row = centro_r;
    center.col = centro_c;
    //raggio dell'intorno
    size_t  hsize =  7;
    //
    core::mystat vstat  
      = core::estimate_depth(depthim, center, hsize);
    //
    double distanza = vstat.mean;
    double speed  = 100;//??

    if(distanza < 1.2)
    {
      speed = 0;
    }

    //relative goal
    math::point2d 
      target(distanza, math::angle(loc_theta_target, math::deg_tag));
    //
    printf("Set Target %f %f %f\n", target.get_x1(), target.get_x2(), target.orientation().deg());
    //
    p3dx->set_target_to_follow 
      (target, speed );

    //Display Image
    //const unsigned char red  [3] = {255,  0,  0};
    //rgb_cimg->assign(rightim.get(),  bee->ncols(), bee->nrows(),1,3);
    //rgb_cimg->draw_circle(centro_c, centro_r, 10.0,red);
    //rgb_cimg->display(*rgb_win);
    printf("Distanza %.2f\n", distanza);

  }//centro_c>0
  else
  {
    //TODO: skip??
    process_event(fail_event());
  }
  ///se va storto..
  //
  }

}
//---------------------------------------------------------------------------
void tracking_machine::idle_tracking_cb()
{

}
//---------------------------------------------------------------------------
void tracking_machine::failed_cb()
{

}
//---------------------------------------------------------------------------
//###########################################################################
//---------------------------------------------------------------------------
  ///START_SETUP
bool tracking_machine::start_setup(setup_event const&)
{
  printf("start_setup\n");
    boost::mutex::scoped_lock lock(process_guard);
  ////
  fire_callback = boost::bind
    (&tracking_machine::setup_cb, this);
  ///
  return true;
}
//---------------------------------------------------------------------------
  ///RESET
bool tracking_machine::go_reset(reset_event const&)
{
  printf("\nReset\n");
    //
  //ptu_control->enable(false);
  p3dx->enable_stop_mode();

  //
  ptu->set_pantilt(0,0);
  //
  boost::mutex::scoped_lock lock(process_guard);
  ////
  fire_callback = boost::bind
    (&tracking_machine::idle_cb, this);
  ///
  return true;
}
//---------------------------------------------------------------------------
  ///IDLE TRACKING
bool tracking_machine::go_idle_tracking (idle_track_event const&)
{
  //ptu_control->enable(false);
  p3dx->enable_stop_mode();

  boost::mutex::scoped_lock lock(process_guard);
  ////
  fire_callback = boost::bind
    (&tracking_machine::idle_tracking_cb, this);
  return true;
}
//---------------------------------------------------------------------------
  ///FAIL
bool tracking_machine::go_fail (fail_event const&)
{
  //ptu_control->enable(false);
  p3dx->enable_stop_mode();
  //
  boost::mutex::scoped_lock lock(process_guard);
    ////
  fire_callback = boost::bind
    (&tracking_machine::failed_cb, this);

  return true;
}
//---------------------------------------------------------------------------
  ///START TRACKING
bool tracking_machine::start_tracking (track_event const&)
{
  //INIT TRACKING ...
  //TODO:Aggiorna i setpoint
  float pan, tilt;
  ptu->get_current_pantilt(pan,tilt);
  //
  //double pan_error = pinhole.delta_pan_from_pixel(centro_c) ;
  //Desired Heading
  double theta_rob = p3dx->get_odometry().getTh().deg();

  //
  math::point2d target(1.0, math::angle(pan,math::deg_tag));
  p3dx->set_target_to_follow(target, 0);
  p3dx->enable_follow_mode();

  ///
  fire_callback = boost::bind
    (&tracking_machine::tracking_cb, this);

  return true;
}
//---------------------------------------------------------------------------
//###########################################################################
//Support Routines
//###########################################################################
void tracking_machine::move_ptu_to_screen_rc(float row, float col, double waitsec)
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
    ptu->set_pantilt(nupan, nutilt, waitsec);

    //printf("Row: %f Col: %f \n", row, col);
    //printf("current: %.2f %.2f\n", current.get_pan_deg(),current.get_tilt_deg());
    //printf("Delta:   %.2f %.2f\n",  delta.get_pan_deg(), delta.get_tilt_deg());
  }
}
//###########################################################################
//---------------------------------------------------------------------------
void tracking_machine::taskreceived(int evt)
{
  boost::mutex::scoped_lock lock(process_guard);
  switch (evt)
  {  
  case etag::RESET :
    process_event(trm::tracking_machine::reset_event() );
    break;

  //case etag::SETUP :
  //  process_event(trm::tracking_machine::setup_event() );
  //  break;

  case etag::TRACK :
    process_event(trm::tracking_machine::track_event() );
    break;

  case etag::IDLETRACK :
    process_event(trm::tracking_machine::idle_track_event() );
    break;

  case etag::RESUME :
    process_event(trm::tracking_machine::resume_event() );
    break;

  case etag::FAIL :
    process_event(trm::tracking_machine::fail_event() );
    break;

  case etag::RECOVER :
    process_event(trm::tracking_machine::recover_event() );
    break;

  default:
    break;
  }
}
//---------------------------------------------------------------------------
void tracking_machine::setup_roi(int r, int c, int h, int w)
{
 //
 r_roi = r;
 c_roi = c;
 h_roi = h;
 w_roi = w;

 boost::mutex::scoped_lock lock(process_guard);
 //
 process_event(trm::tracking_machine::setup_event() );
}
//---------------------------------------------------------------------------
}}//all::trm
//---------------------------------------------------------------------------