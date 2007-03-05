#include "tracking_machine.h"
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
  p3dx.reset(new act::p3dx_gateway()); 


  p3_adapter.reset(new act::p3_odometry_adapter_t(p3dx) );

  ptu_control.reset(new act::pantilt_control_loop_t );
  ptu_control->set_ptu(ptu);
  ptu_control->set_slam(p3_adapter);

}
//---------------------------------------------------------------------------
void tracking_machine::threadloop()
  {
  ///
  workspace.reset( new matlab::matlab_engine_t);
  workspace->command_line("init_tracking");

    while (running_)
    {
      {
        boost::mutex::scoped_lock lock(process_guard);
        if(!fire_callback.empty())
              fire_callback();    
      }
    boost::thread::yield();    
    core::BOOST_SLEEP(100);
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
    core::uint8_sarr rightim = bee->get_color_buffer(core::right_img);

    mxArray* mx_rimage = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                          ,matlab::row_major
                                                          ,bee->nrows()
                                                          ,bee->ncols());
    
  //Push into Workspace
    workspace->put_array("rgb", mx_rimage);
    printf("SETUP\n");
    ////
    workspace->command_line("[centro_r centro_c coerente] = fh_setup(rgb)");
    ///

    printf("Done Setup\n\n");
    int coerente = workspace->get_scalar_int("coerente");
    if(coerente)
      //Se tutto a posto vai in idled_tracking
      process_event(idle_track_event());
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
  if (bee->grab())
  {
  core::uint8_sarr rightim = bee->get_color_buffer(core::right_img);

  mxArray* mx_rimage = 
    matlab::buffer2array<core::uint8_t>::create_from_planar(rightim.get()
                                                        ,matlab::row_major
                                                        ,bee->nrows()
                                                        ,bee->ncols());
  //Push into Workspace
  workspace->put_array("rgb", mx_rimage);
  ///ottenere un centro
   workspace->command_line("[centro_r centro_c]= fh_track_and_show(rgb, [240, 320])");
  //***GATHER***
  int centro_r = static_cast<int> (hsm.workspace->get_scalar_double("centro_r") );
  int centro_c = static_cast<int> (hsm.workspace->get_scalar_double("centro_c") );
  if(centro_c > 0)//che non sia nullo ....
  {
  ///stimare profondit� e direzione
    double pan_delta = hsm.pinhole.delta_pan_from_pixel(centro_c);
  ///valutare setpoin:   
  ///se near stop
  ///snn� calcola velocit� giusta
  ///passare setpoint al robot
  //***ROBOT SET POINT***
    //Desired Heading
  theta_rob             =   hsm.p3dx->get_odometry().getTh().deg();  
  theta_target          =   theta_rob + current_pan + pan_delta;
  double delta_heading  =   theta_target - theta_rob;

  if(fabs(delta_heading) > 5)//ops .. una soglia :D
    hsm.p3dx->set_delta_heading(math::angle(delta_heading, math::deg_tag));

  }//centro_c>0
  else
  {
    //TODO: skip??
    centro_c = 320;
    centro_r = 240;
  }
  ///se va storto..
  process_event(fail_event());
}
//---------------------------------------------------------------------------
void tracking_machine::idle_tracking_cb()
{

}
//---------------------------------------------------------------------------
//###########################################################################
//---------------------------------------------------------------------------
  ///START_SETUP
bool tracking_machine::start_setup(setup_event const&)
{
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
  boost::mutex::scoped_lock lock(process_guard);
  ////
  fire_callback = boost::bind
    (&tracking_machine::idle_tracking_cb, this);
  return true;
}
//---------------------------------------------------------------------------
  ///START TRACKING
bool tracking_machine::start_tracking   (track_event const&)
{
  double  centro_r = workspace->get_scalar_double("centro_r");
  double  centro_c = workspace->get_scalar_double("centro_c");
  //
  move_ptu_to_screen_rc(centro_r, centro_c, 1.5);

  //INIT TRACKING ...
  //TODO:Aggiorna i setpoint
  float pan, tilt;
  ptu->get_current_pantilt(pan,tilt);
  //
  double pan_error = pinhole.delta_pan_from_pixel(centro_c) ;
  //Desired Heading
  double theta_rob = p3dx->get_odometry().getTh().deg();

  //
  ptu_control->set_polar_reference(math::deg_tag,theta_rob + pan);
  //
  ptu_control->enable(true);
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
  printf("PTU COM %f : %f\n", nupan, nutilt);
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