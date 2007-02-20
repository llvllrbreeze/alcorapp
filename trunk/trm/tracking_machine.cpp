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
  tasklistener.reset(new task_listener("config\trm_service.ini"));
  tasklistener->notify = boost::bind(&tracking_machine::taskreceived, this, ::_1);
  tasklistener->run_async();

  //P3DX
  p3dx = act::create_p3dx_client();
  //bumblebee
  bee.reset(new sense::bumblebee_driver_t());
  bee->open("config/bumblebeeB.ini");

  //
  pinhole.focal = bee->focal();//pare non andare bene .. chissà
  //pinhole.focal = 253.07;
  pinhole.ncols = bee->ncols();
  pinhole.nrows = bee->nrows();

}
//---------------------------------------------------------------------------
void tracking_machine::threadloop()
  {
  ///
  workspace.reset( new matlab::matlab_engine_t);
  workspace->command_line("init_attention");

    while (running_)
    {
      {
        boost::mutex::scoped_lock lock(process_guard);
        if(!fire_callback.empty())
              fire_callback();    
      }
    boost::thread::yield();    
    Sleep(50);
    }
  }
//---------------------------------------------------------------------------
void tracking_machine::setup_cb()
{
  //TODO: grab!
  printf("SETUP\n");
  ////
  workspace->command_line("[centro_r centro_c coerente] = fh_setup(rgb)");
  ///

  printf("Done Setup\n\n");
  //Se tutto a posto vai in idled_tracking
  process_event(idle_track_event());
}
//---------------------------------------------------------------------------
//###########################################################################
//---------------------------------------------------------------------------
  ///START TRACKING
bool tracking_machine::start_tracking   (track_event const&)
{
  //skip
  //machine.pre_tracking_ini();
  double  centro_r = workspace->get_scalar_double("centro_r");
  double  centro_c = workspace->get_scalar_double("centro_c");
  //
  //pt_move_to_screen_coord(centro_r, centro_c, 1.5);

  //INIT TRACKING ...
  //TODO:Aggiorna i setpoint
  //float pan, tilt;
  //machine.ptu->get_current_pantilt(pan,tilt);

  //double pan_error = machine.pinhole.delta_pan_from_pixel(centro_c) ;
  //Desired Heading
  //double theta_rob = machine.p3dx->get_odometry().getTh().deg();

  //tracking_loop::Instance().theta_target =  
  //          theta_rob + pan;

  //tracking_loop::Instance().current_pan =
  //          pan;

  //tracking_loop::Instance().current_tilt =
  //          tilt;

  //tracking_loop::Instance().centro_r = 240;
  //tracking_loop::Instance().centro_c = 320;

  return true;
}
//---------------------------------------------------------------------------
//###########################################################################
//---------------------------------------------------------------------------
void tracking_machine::taskreceived(int)
{

}
//---------------------------------------------------------------------------
}}//all::trm
//---------------------------------------------------------------------------