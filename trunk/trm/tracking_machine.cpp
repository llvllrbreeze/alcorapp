#define WIN32_LEAN_AND_MEAN

#include "tracking_machine.h"
#include "alcor.apps/trm/gil_wrap_utils.hpp"
//---------------------------------------------------------------------------
namespace all { namespace trm {
//---------------------------------------------------------------------------

tracking_machine::tracking_machine():running_(true)
{
  ////
  fire_callback = boost::bind
    (&tracking_machine::idle_cb, this);

  thisthread.reset( 
      new boost::thread
      (
          boost::bind(&tracking_machine::threadloop, this) 
      )
    );  

  //Task Listener
  tasklistener.reset(new task_listener("config/trm_service.ini"));
  tasklistener->notify = boost::bind(&tracking_machine::taskreceived, this, ::_1);
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

  p3_adapter.reset(new act::p3_odometry_adapter_t(p3dx) );

  ptu_control.reset(new act::pantilt_control_loop_t );
  ptu_control->set_ptu(ptu);
  ptu_control->set_slam(p3_adapter);
  
  ////open views
  rgb_win.reset(new cimglib::CImgDisplay(bee->ncols(), bee->nrows(), "rgb"));
  rgb_cimg.reset(new cimglib::CImg<core::uint8_t>());
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

}
//---------------------------------------------------------------------------
void tracking_machine::setup_cb()
{
  if (bee->grab())
  {
    //printf("Image Grabbed\n");
    core::uint8_sarr rightim = bee->get_color_buffer(core::right_img);

    //printf("buffer2array %d %d\n", bee->nrows(), bee->ncols());
    mxArray* mx_rimage = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                          ,matlab::row_major
                                                          ,bee->nrows()
                                                          ,bee->ncols());
    
  //Push into Workspace
    //printf("put_array \n");
    workspace->put_array("rgb", mx_rimage);
    printf("SETUP\n");
    ////
    workspace->command_line("[centro_r centro_c coerente] = fh_setup(rgb)");
    ///

    printf("Done Setup\n\n");
    int coerente = workspace->get_scalar_int("coerente");

    if(coerente)
    {
      double  centro_r = workspace->get_scalar_double("centro_r");
      double  centro_c = workspace->get_scalar_double("centro_c");
      //
      move_ptu_to_screen_rc(centro_r, centro_c, 1.5);
      //Se tutto a posto vai in idled_tracking
      process_event(idle_track_event());
    }
    else
      process_event(reset_event());
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
  
  //ptu_control->enable(false);

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

    
    //Push into Workspace
    workspace->put_array("rgb", mx_rimage);

  ///MEAN SHIFT : ottenere un centro
   workspace->command_line("[centro_r centro_c]= fh_track_and_show(rgb, [120, 160])");

  //***GATHER***
  int centro_r = static_cast<int> (workspace->get_scalar_double("centro_r") );
  int centro_c = static_cast<int> (workspace->get_scalar_double("centro_c") );

  if(centro_c > 0)//che non sia nullo ....
  {
    //
    move_ptu_to_screen_rc(centro_r, centro_c);

    //***Target Angular SET POINT***    
    //double theta_target= current_pan + pan_delta;
    double pan_delta     = pinhole.delta_pan_from_pixel(centro_c);
    double th_robot      = p3dx->get_odometry().getTh().deg();
    //theta globale
    double loc_theta_target = current_pt.pan +pan_delta;
    double glo_theta_target = loc_theta_target + th_robot;
    //
    //ptu_control->set_polar_reference(math::deg_tag, glo_theta_target);
    //ptu_control->enable(true);

    ///Profondità 3D
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
    p3dx->set_target_to_follow 
      (target, speed );

    //Display Image
    const unsigned char red  [3] = {255,  0,  0};
    rgb_cimg->assign(rightim.get(),  bee->ncols(), bee->nrows(),1,3);
    rgb_cimg->draw_circle(centro_c, centro_r, 10.0,red);
    rgb_cimg->display(*(rgb_win.get()));
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
bool tracking_machine::go_reset (reset_event const&)
{
  printf("\nReset\n");
    //
  ptu_control->enable(false);
  p3dx->enable_stop_mode();
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
  ptu_control->enable(false);
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
  ptu_control->enable(false);
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
bool tracking_machine::start_tracking   (track_event const&)
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
  //ptu_control->set_polar_reference(math::deg_tag, theta_rob + pan);
  //
  //ptu_control->enable(true);

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
  core::pantilt_angle_t pt = ptu->get_fast_pantilt();
  //
  core::pantilt_angle_t delta;
  pinhole.pantilt_from_pixel(row, col, delta);
  //
  float nupan  = static_cast<float>(-delta.pan)  + pt.pan;
  float nutilt = static_cast<float>(-delta.tilt) + pt.tilt;

  ptu->set_pantilt(nupan, nutilt, waitsec);
      //printf("\nCentro %d : %d\n", (int)row, (int)col);
  //printf("PTU COM %f : %f\n", nupan, nutilt);
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

  case etag::SETUP :
    process_event(trm::tracking_machine::setup_event() );
    break;

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
}}//all::trm
//---------------------------------------------------------------------------