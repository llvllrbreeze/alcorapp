#define WIN32_LEAN_AND_MEAN
//---------------------------------------------------------------------------
#include "tracking_machine.h"
#include "alcor/core/config_parser_t.hpp"
//---------------------------------------------------------------------------
namespace all { namespace trm {
//---------------------------------------------------------------------------

tracking_machine::tracking_machine():running_(true)
{
  ////
  fire_callback = boost::bind
    (&tracking_machine::idle_cb, this);

  config_parser_t config;
  config.load(core::tags::ini, "config/trm_app_details.ini");

  init_command      = config.get<std::string>("matlab.init_command");
  setup_command     = config.get<std::string>("matlab.setup_command");
  tracking_command  = config.get<std::string>("matlab.tracking_command");

  min_safe_distance = config.get<double>("limits.min_safe_distance");
  max_safe_distance = config.get<double>("limits.max_safe_distance");

  min_tracking_vel  = config.get<double>("limits.min_tracking_vel");
  max_tracking_vel  = config.get<double>("limits.max_tracking_vel");

  tilt_threshold    = config.get<double>("limits.tilt_threshold");

  printf("\nLimits\n");
  printf("-> min_safe_distance: %f\n", min_safe_distance);
  printf("-> max_safe_distance: %f\n", max_safe_distance);
  printf("-> min_tracking_vel: %f\n", min_tracking_vel);
  printf("-> max_tracking_vel: %f\n", max_tracking_vel);
  printf("-> tilt_threshold: %f\n\n", tilt_threshold);

  std::string 
    beeconfigfile = config.get<std::string>("bumblebee.config");
  std::string 
    p3configfile = config.get<std::string>("p3.config");

  //Task Listener
  tasklistener.reset(new task_listener("config/trm_service.ini"));

  tasklistener->notify_evt = 
    boost::bind(&tracking_machine::taskreceived, this, ::_1);

  tasklistener->notify_roi = 
    boost::bind(&tracking_machine::setup_roi, this, ::_1,::_2,::_3,::_4, ::_5);

  tasklistener->run_async();

  //bumblebee
  //bee.reset(new sense::bumblebee_driver_t());
  bee = sense::bumblebee_driver_t::create();
  bee->open(beeconfigfile);
  //PTU
  ptu.reset (new act::directed_perception_ptu_t);
  ptu->open("config/dpptu_conf.ini");

  //PINHOLE
  pinhole.focal = bee->focal();
  pinhole.ncols = bee->ncols();
  pinhole.nrows = bee->nrows();
  //
  half_rows = bee->nrows()/2;
  half_cols = bee->ncols()/2;

   //P3DX
  //p3dx.reset(new act::p3_gateway()); 
  //if (p3dx->open("config/p3_conf.ini"))
  //  printf("Robot connected!\n");

  p3dx.reset(new act::p3_client_t(p3configfile.c_str() )); 
  //if (p3dx->open("config/p3_conf.ini"))
  p3dx->run_async();
    printf("Robot connected!\n");
  
  
  ////open views
  //rgb_win.reset(new cimglib::CImgDisplay(bee->ncols(), bee->nrows(), "rgb"));
    rgb_cimg.reset(new CImg<core::uint8_t>(bee->ncols(), bee->nrows()), core::null_deleter() );

  //Streaming
  stream_source_ptr.reset(new all::core::memory_stream_source_t( bee->nrows(), bee->ncols() ) );
  stream_source_ptr->set_quality(80);
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
    core::BOOST_SLEEP(100);
    delete stream_server_ptr;
  }
}
//---------------------------------------------------------------------------
void tracking_machine::threadloop()
  {
  //
  workspace.reset( new matlab::matlab_engine_t);
  //
  workspace->command_line(init_command);
  //Enter loop
  while (running_)
  {
    {
      boost::mutex::scoped_lock lock(process_guard);
      if(!fire_callback.empty())
            fire_callback();    
    }
    boost::thread::yield();    
    core::BOOST_SLEEP(10);
  }

  }
//###########################################################################
//CALLBACK
//---------------------------------------------------------------------------
void tracking_machine::idle_cb()
{
  if (bee->grab())
  {
    rightim = bee->get_color_buffer(core::right_img);
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
  centro_r = r_roi + h_2;
  centro_c = c_roi + w_2;

  //forst, center the ptu on the target/roi
  move_ptu_to_screen_rc(centro_r, centro_c, 2.0);

  //then grab again and launch model setup
  if (bee->grab())
  {
    rightim = 
      bee->get_color_buffer(core::right_img);

    mxArray* mx_rimage = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                          ,matlab::row_major
                                                          ,bee->nrows()
                                                          ,bee->ncols());
    
    //Push into Workspace
    workspace->put_array("rgb", mx_rimage);

    //update roi data
    r_roi = (bee->nrows()/2) - h_2;
    c_roi = (bee->ncols()/2) - w_2;


    //ROI
    workspace->put_scalar("r_roi", r_roi);
    workspace->put_scalar("c_roi", c_roi);
    workspace->put_scalar("h_roi", h_roi);
    workspace->put_scalar("w_roi", w_roi);
    workspace->put_scalar("scala_resize", scala_resize);
    //
    printf("SETUP\n");

    //workspace->command_line
    //  (setup_command.c_str());
    workspace->command_line("[centro_r centro_c] = fh_setup_stepRGB(rgb, r_roi, c_roi, h_roi, w_roi, scala_resize)");
    //workspace->command_line("[centro_r centro_c] = trm_model_setup(rgb, r_roi, c_roi, h_roi, w_roi, scala_resize)");
    ///
    printf("Done Setup\n\n");

    //centro_r = static_cast<int>( workspace->get_scalar_double("centro_r") );
    //centro_c = static_cast<int>( workspace->get_scalar_double("centro_c") );

    centro_r = static_cast<int> ( (r_roi + (static_cast<double> (h_roi/2.0) )  ));
    centro_c = static_cast<int> ( (c_roi + (static_cast<double>  (w_roi/2.0) ) ));

    printf("setup centro: %d %d \n", centro_r, centro_c);
    //
    double pan_delta     = pinhole.delta_pan_from_pixel(centro_c);
    double th_robot      = p3dx->get_odometry().getTh().deg();

    //theta globale
    core::pantilt_angle_t 
                 current_pt = ptu->get_fast_pantilt();

    double loc_theta_target = current_pt.get_pan(math::deg_tag) + pan_delta;
    glo_theta_target = loc_theta_target + th_robot;

    //center again
    move_ptu_to_screen_rc(centro_r, centro_c ,0.5);

    //
    centro_r = bee->nrows()/2;
    centro_c = bee->ncols()/2;

    //Se tutto a posto vai in idled_tracking
    process_event(idle_track_event());
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
#ifdef TIMEDEBUG_
  printf("\n--------------------\n");
  boost::timer profile;
  gprofile.restart();
#endif

  math::angle pana_;
  math::angle tilta_;

  //center pantilt, depending on last glo_theta_target and theta_rob
  double th_robot      = p3dx->get_odometry().getTh().deg();

  //to compensate
  pana_.set_deg(glo_theta_target - th_robot);
  tilta_.set_deg(pinhole.delta_tilt_from_pixel(centro_r));

  //double pan_         = glo_theta_target - th_robot;
  //double tilt_        = pinhole.delta_tilt_from_pixel(centro_r);

  #ifdef TIMEDEBUG_
    profile.restart();
  #endif
  if( ::fabs((double) centro_r - (double)half_rows ) < tilt_threshold)
  {
    //compensate only pan ...
    ptu->set_pan(pana_.deg()); 
    centro_c = half_cols;
  }
  else
  {
    //compensate!!
    ptu->set_pantilt(pana_.deg(), tilta_.deg());
    centro_c = half_cols;
    centro_r = half_rows;
  }

  #ifdef TIMEDEBUG_
    printf("->set_pantilt %.3f\n", profile.elapsed());
  #endif

  //Grab Images!
  if (bee->grab())
  {

    #ifdef TIMEDEBUG_
      profile.restart();
    #endif
    //Current PanTilt
    core::pantilt_angle_t current_pt = 
      ptu->get_fast_pantilt();

    #ifdef TIMEDEBUG_
      printf("->get_fast_pantilt %.3f\n", profile.elapsed());
    #endif
    
    rightim = 
      bee->get_color_buffer(core::right_img);

    #ifdef TIMEDEBUG_
      profile.restart();
    #endif
    mxArray* mx_rimage = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                          ,matlab::row_major
                                                          ,bee->nrows()
                                                          ,bee->ncols());
    #ifdef TIMEDEBUG_
      printf("->create_from_planar %.3f\n", profile.elapsed());
    #endif

    //Push into Workspace
    workspace->put_array("rgb", mx_rimage);

    #ifdef TIMEDEBUG_
    profile.restart();
    #endif

    //the center
    workspace->put_scalar("centro_c", centro_c);
    workspace->put_scalar("centro_r", centro_r);

    ///MEAN SHIFT : ottenere un centro
    workspace->command_line("[centro_r centro_c]= fh_track_stepRGB(rgb, [centro_r, centro_c])");

    //workspace->command_line(tracking_command.c_str());

    #ifdef TIMEDEBUG_
      printf("->track routine %.3f\n", profile.elapsed());
    #endif

    //***GATHER***
    centro_r = 
      static_cast<int> (workspace->get_scalar_double("centro_r") );

    centro_c = 
      static_cast<int> (workspace->get_scalar_double("centro_c") );

    //printf("\nCentro: %d : %d\n", centro_r , centro_c);

    if(centro_c > 0)//che non sia nullo ....
    {
      //proviamo a saltarla... mmmh no..
      //move_ptu_to_screen_rc(centro_r, centro_c);

      //***Target Angular SET POINT***    
      //double theta_target= current_pan + pan_delta;
      double pan_delta     = pinhole.delta_pan_from_pixel(centro_c);
      double th_robot      = p3dx->get_odometry().getTh().deg();

      //local offset (that is respect to robot nose)
      double loc_theta_target = current_pt.get_pan_deg() + pan_delta;      
      //theta globale (useful for next iteration)
      glo_theta_target = loc_theta_target + th_robot;

      #ifdef TIMEDEBUG_
      profile.restart();
      #endif   
      //Profondità 3D
      depth = bee->get_depth_buffer();      
      #ifdef TIMEDEBUG_
        printf("->get_depth_buffer %.3f\n", profile.elapsed());
      #endif

      #ifdef TIMEDEBUG_
      profile.restart();
      #endif         
      depthim.assign(bee->nrows(), bee->ncols(), depth.get());
      #ifdef TIMEDEBUG_
        printf("->depthim.assign %.3f\n", profile.elapsed());
      #endif

      #ifdef TIMEDEBUG_
      profile.restart();
      #endif   
      //centro dell'intorno
      core::pixelcoord_t center;
      center.row = centro_r;
      center.col = centro_c;

      //raggio dell'intorno
      size_t  hsize =  8;
      //
      core::mystat vstat  
        = core::estimate_depth(depthim, center, hsize);

      #ifdef TIMEDEBUG_
        printf("->estimate_depth %.3f\n", profile.elapsed());
      #endif

      //
      double distanza = vstat.mean;
      double speed  = 100;//??

      if( fabs (loc_theta_target) > 4.0 )
      {
        //relative goal
        math::point2d 
          target(distanza, math::angle(glo_theta_target, math::deg_tag));
        ////
        //printf("Set Target %f %f %f\n"
        //  , target.get_x1()
        //  , target.get_x2()
        //  , target.orientation().deg());
        //
        if(distanza < min_safe_distance)
        {
          //stop
          speed = 0;
        }
        else if (distanza < max_safe_distance)
        {
          //(vmax-vmin)/(dmax - dmin) * (D - dmin) + vmin
          speed = (   (max_tracking_vel-min_tracking_vel)
                    / (max_safe_distance - min_safe_distance)
                    * (distanza - min_safe_distance) 
                  )
                  + min_tracking_vel ;
        }
        else
        {
          //saturate
          speed = max_tracking_vel;
        }
        //
        p3dx->set_relative_goto(target, speed );

        //Display Image
        //char* dtag = "" ;
        //sprintf( dtag, "Distanza: %.2f", distanza);


        const unsigned char color  [3] = {217,  241,  60};

        ///shared memory
        rgb_cimg->assign(rightim.get(),  bee->ncols(), bee->nrows(),1,3, true)
                .draw_rectangle(centro_c - (w_roi/2) , centro_r - (h_roi/2)
                          ,centro_c + (w_roi/2) , centro_r + (h_roi/2)
                          ,color
                          , 0.5);
        
        //send image stream
        stream_source_ptr->update_image(rightim);
      }

    }//centro_c>0
    else
    {
      printf("FAIL!!");
      //TODO: skip??
      process_event(fail_event());
    }

  ///se va storto..
  //
  }
  #ifdef TIMEDEBUG_
  printf("Cycle time: %.2f\n", gprofile.elapsed());
  #endif
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

  ////
  //math::point2d target(1.0, math::angle(pan,math::deg_tag));
  //p3dx->set_relative_goto(target, 0);
  //p3dx->enable_follow_mode();

  printf("enabling goto\n");
  p3dx->enable_goto_mode();

  ///
  fire_callback = boost::bind
    (&tracking_machine::tracking_cb, this);

#ifdef TIMEDEBUG_
  gprofile.restart();
#endif

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
void tracking_machine::setup_roi(int r, int c, int h, int w, int scala)
{
 //
 r_roi = r;
 c_roi = c;
 h_roi = h;
 w_roi = w;
 scala_resize = 
   static_cast<double>(scala/100.0);
 boost::mutex::scoped_lock lock(process_guard);
 //
 process_event(trm::tracking_machine::setup_event() );
}
//---------------------------------------------------------------------------
}}//all::trm
//---------------------------------------------------------------------------