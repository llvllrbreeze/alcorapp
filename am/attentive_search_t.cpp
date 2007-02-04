#include "attentive_search_t.h"
#include "alcor\core\config_parser_t.h"
#include "alcor\sense\slam2d_adpater.h"
//-----------------------------------------------------------------------++
#include <boost/thread/thread.hpp>
#include <boost/timer.hpp>
//-----------------------------------------------------------------------++
#include <windows.h>
//-----------------------------------------------------------------------++
using namespace all;
//-----------------------------------------------------------------------++
//###########################################################################
am::attentive_search_t::attentive_search_t()
  :running(true)
{
  ///
  current_state = &listen::Instance();

  ///
  evt_listener = create_listener();
  ///
  evt_listener->subscribe(this);

  ///
  bumblebee.open_rgb();
  bumblebee.open_xyz();
  bumblebee.use_internal_buffers();
  //adesso hard coded .. ma poi
  //saranno letti direttamente da ipc_bumblebee_recv_t...
  pinhole.focal = bumblebee.focal();//pare non andare bene .. chissà
  pinhole.focal = 253.07;
  pinhole.ncols = bumblebee.width();
  pinhole.nrows = bumblebee.height();
  ////USING THE MARLIN
  //pinhole.focal = 607.5;
  //pinhole.ncols = 640;
  //pinhole.nrows = 480;

  ///
#ifdef USEPANTILT
  init_ptu();
#endif

#ifdef XODOLOG
  xlog.open("odolog.txt",std::ios::out);
#endif

  ///DORO
  p3dx = act::create_p3dx_client();

  ///SPLAM
  splam.reset(new splam_client_t);

  //slam2d_adapter .. a temporary shared pointer.
  boost::shared_ptr<sense::slam2d_adapter> 
    slam_adapter_sptr(new sense::slam2d_adapter(splam) );

  //
  pt_control_loop_sptr.reset(new act::pantilt_control_loop_t(ptu,slam_adapter_sptr) );

  ///The MACHINE
  machine_thr.reset(
    new boost::thread 
    (
    boost::bind(
    &am::attentive_search_t::run_thread
    , this) 
    )
    );

}
//am::attentive_search_t::attentive_search_t()
//###########################################################################
void am::attentive_search_t::init_ptu()
{
//#ifdef USEPANTILT

  all::core::config_parser_t config;

  //
  config.load(all::core::ini,"config/dpptu_conf.ini");

  //
  double minpan = config.as_double("ptu.minpan", -120);
  //
  double maxpan = config.as_double("ptu.maxpan", 120);
  //
  double mintilt = config.as_double("ptu.mintilt", -20);
  //
  double maxtilt = config.as_double("ptu.maxtilt", 20);

  ////
  //ptu_limiter.set_range(std::make_pair(minpan,maxpan)
  //                     ,std::make_pair(mintilt,maxtilt));

  ptu.reset(new all::act::directed_perception_ptu_t);
  ptu->open();

  ptu->set_angle_format(math::deg_tag);

//#endif
}
//###########################################################################
void am::attentive_search_t::get_notified(void*)
{
  process_event(evt_listener->get_event());
}
//###########################################################################
void am::attentive_search_t::process_event(am::events::tag event)
{
  ///
  //printf("Processing Event: %d\n", event);
  ///
  boost::mutex::scoped_lock lock(process_guard);
  ///
  //printf("Lock Acquired for event: %d\n", event);

  switch (event)
  { 

  case am::events::START:
    printf("\n\n**Invoking event START**\n\n");
    current_state->onSTARTSEARCH(*this);
   break;

  case am::events::TRACK:
    printf("\n\n**Invoking event TRACK**\n\n");
    current_state->onTRACKING(*this);
   break;

  case am::events::TRACKSETUP:
    printf("\n\n**Invoking event TRACKSETUP**\n\n");
    current_state->onTRACKINGSETUP (*this);
   break;

  case am::events::RESET:
    printf("\n\n**Invoking event RESET**\n\n");
    current_state->onRESET(*this);
   break;

  case am::events::IDLE:  
    printf("\n\n**Invoking event IDLE**\n\n");
    current_state->onIDLE(*this);
   break;

  case am::events::STOP:   
    printf("\n\n**Invoking event STOP**\n\n");
    current_state->onRESET(*this);
   break;

  case am::events::DEBUG:
    printf("\n\n**Invoking event DEBUG**\n\n");
    current_state->onDEBUG(*this);
   break;

  case am::events::RESETPANTILT:
    printf("\n\n**Invoking event RESETPANTITLT**\n\n");
    current_state->onRESETPANTILT(*this);
   break;

  default:
   break;
  }
}
//###########################################################################
void am::attentive_search_t::run_thread()
{
  machine_timer.restart();
  ///
  workspace.reset( new matlab::matlab_engine_t);
  call_script("init_attention");

  unsigned long pause=50;

  while (running)
  {
    {
    boost::mutex::scoped_lock lock(process_guard);
    current_state->invoke(*this);
    pause = current_state->get_tick();
    }

  boost::thread::yield();
  Sleep(pause);
  }

}
//###########################################################################
void am::attentive_search_t::change_state(unit_state_t& next)
{
  boost::mutex::scoped_lock lock(process_guard);
  //printf("Switching State\n");
  current_state = &next;

}
//###########################################################################
void am::attentive_search_t::reset_devices()
{
  if(p3dx.get())
    p3dx->stop_robot();

  if(ptu.get())  
    ptu->set_pantilt(0,-15);
}
//###########################################################################
bool am::attentive_search_t::grab_data()
{
  //Images
  rgb = bumblebee.get_internal_color(all::sense::right_cam);
  xyz = bumblebee.get_internal_depth();
  //Splam
  splam->get_slam_data(splam_data);

  return true;
}
//###########################################################################
///
void am::attentive_search_t::push_data()
{    
  //RGB
  mxArray* mx_rgb = 
    matlab::buffer2array<typename matlab::traits<core::uint8_t> >::create_from_planar(
                            rgb.get()
                          , matlab::row_major
                          , bumblebee.height()
                          , bumblebee.width()
                          );
  //XYZ
  mxArray* mx_xyz =
    matlab::buffer2array<typename matlab::traits<core::single_t> >::create_from_planar(
                            xyz.get()
                          , matlab::row_major
                          , bumblebee.height()
                          , bumblebee.width()    
    );

   mxArray* mxsplam = splam_adapter.adapt_map(splam_data);
   mxArray* mxpose  = splam_adapter.adapt_pose(splam_data);

  //Push into Workspace
  workspace->put_array("rgb", mx_rgb);
  workspace->put_array("xyz", mx_xyz);
  workspace->put_scalar("elapsed", machine_timer.elapsed());
  //SOLO PER I TeST CON MAPPA COMPLETA!!!!
  //DECOMMENTA!
  workspace->put_array("MappaCur",mxsplam);
  workspace->put_array("doropose",mxpose);

   if (ptu.get())
   {
    float pan, tilt;

    //ptu->get_current_pantilt(pan,tilt);
    pan = ptu->get_pan();
    tilt = ptu->get_tilt();

    mxArray* mx_pantilt = pt_adapter(pan,tilt);
    workspace->put_array("pantilt",mx_pantilt);  
    mxDestroyArray(mx_pantilt);
   }

  //Clean
  mxDestroyArray(mx_rgb);
  mxDestroyArray(mx_xyz);
  mxDestroyArray(mxsplam);
  mxDestroyArray(mxpose);
};
//###########################################################################
bool am::attentive_search_t::grab_tracking_data()
{
  //Images
  workspace->command_line("grab_marlin");
  return true;
};
//###########################################################################
///
void am::attentive_search_t::push_tracking_data()
{    
//###########################################################################

  if(ptu.get())
  {
  float pan, tilt;
  ptu->get_current_pantilt(pan,tilt);  
  mxArray* mx_pantilt = pt_adapter(pan,tilt);
  workspace->put_array("pantilt",mx_pantilt);  
  mxDestroyArray(mx_pantilt);
  }
}
//###########################################################################
///
void am::attentive_search_t::call_script(const std::string& script)
{
  workspace->command_line(script);
}
//###########################################################################
///
int am::attentive_search_t::call_function(const std::string& script)
{
  return workspace->command_line_blocking(script);
}
//###########################################################################
///
void am::attentive_search_t::pt_move_to_screen_coord(const float& row, const float& col, double waitsec)
{
//#ifdef USEPANTILT
  if(ptu.get())
  {
  //
  float pan, tilt;
  //
  ptu->update_pantilt();
  ptu->get_current_pantilt(pan,tilt);
  //
  core::pantilt_angle_t delta;
  pinhole.pantilt_from_pixel(row, col, delta);
  //
  float nupan  = static_cast<float>(-delta.pan)  + pan;
  float nutilt = static_cast<float>(-delta.tilt) + tilt;

  //limit range
  //core::pantilt_angle_t clamped = ptu_limiter(nupan, nutilt);
  //printf("PANTILT: %f %f\n", nupan,nutilt);
  //

  ptu->set_pantilt(nupan, nutilt, waitsec);
      //printf("\nCentro %d : %d\n", (int)row, (int)col);
      printf("PTU COM %f : %f\n", nupan, nutilt);
  }
//#endif
}
//###########################################################################
/// 
void am::attentive_search_t::doro_delta_heading_from_screen(const float& col)
{
  if(p3dx.get())
  {
   double delta_pan = pinhole.delta_pan_from_pixel(col);
   p3dx->set_delta_heading(math::angle(-delta_pan,math::rad_tag));
  }
}
//###########################################################################
/// 
void am::attentive_search_t::doro_delta_heading(double deltadeg)
{
  if(p3dx.get())
  {
    p3dx->set_delta_heading(math::angle(deltadeg, math::deg_tag));
  }
}