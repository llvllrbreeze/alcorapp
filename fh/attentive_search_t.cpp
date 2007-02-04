#include "attentive_search_t.h"
#include "alcor2/core/config_parser_t.h"
//-----------------------------------------------------------------------++
#include <boost/thread/thread.hpp>
#include <boost\timer.hpp>
//-----------------------------------------------------------------------++

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
  evt_listener->start();

  ///
  bumblebee.open_rgb();
  bumblebee.open_xyz();
  bumblebee.use_internal_buffers();
  //adesso hard coded .. ma poi
  //saranno letti direttamente da ipc_bumblebee_recv_t...
  pinhole.focal = 253.07;
  pinhole.ncols = bumblebee.width();
  pinhole.nrows = bumblebee.height();

  ///
#ifdef USEPANTILT
  init_ptu();
#endif

  ///
  p3dx = act::create_p3dx_client();
  p3dx->start();

  ///
  splam.reset(new splam_client_t);
  ///
  machine_thr.reset(
    new boost::thread 
    (
    boost::bind(
    &am::attentive_search_t::run_thread
    , this) 
    )
    );

}
//###########################################################################
void am::attentive_search_t::init_ptu()
{
#ifdef USEPANTILT
  using namespace lti;

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
  //
  //using namespace boost::tuples;
  //
  ptu_limiter.set_range(std::make_pair(minpan,maxpan)
                       ,std::make_pair(mintilt,maxtilt));

  //
  int com = config.as_int("ptu.port",8);  
  lti::serial::parameters port_params;
  port_params.receiveTimeout = 150;

  //
  switch(com)
  {
  case 5:
    port_params.port = serial::parameters::Com5;
    break;
  case 6:
    port_params.port = serial::parameters::Com6;
    break;
  case 7:
    port_params.port = serial::parameters::Com7;
    break;
  case 8:
    port_params.port = serial::parameters::Com8;
    break;
  default:
    port_params.port = serial::parameters::Com8;
    break;
  }

  //
  int panspeed  = config.as_int("ptu.panvelstep",1000);
  int tiltspeed = config.as_int("ptu.titlvelstep",1000);

  //
  lti::directedPerceptionPTU::parameters par;
  par.connectionToPTU.setParameters(port_params);

  //ptu.reset(new lti::directedPerceptionPTU);
  ///
  ptu.setParameters(par);

  if(ptu.initialize())
    {
      ptu.setAngleFormat(lti::panTiltUnit::parameters::Degrees);
      ptu.setPanSpeed(panspeed);
      ptu.setTiltSpeed(tiltspeed);
    }
#endif
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

  unsigned long pause=100;

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
  p3dx->stop_robot();
#ifdef USEPANTILT
  ptu.setPanTilt(0,0);
  ptu.awaitPosCommandCompletion();
#endif
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
    matlab::buffer2array<matlab::detail::uint8_t>::create_from_planar(
                            rgb.get()
                          , matlab::row_major
                          , bumblebee.height()
                          , bumblebee.width()
                          );
  //XYZ
  mxArray* mx_xyz =
    matlab::buffer2array<matlab::detail::single_t>::create_from_planar(
                            xyz.get()
                          , matlab::row_major
                          , bumblebee.height()
                          , bumblebee.width()    
    );

  //SPLAM
  //
  mxArray* mx_splam_map =
    splam_adapter.adapt_map(splam_data);

  mxArray* mx_splam_pose =
    splam_adapter.adapt_pose(splam_data);

  //
  double elapsed = machine_timer.elapsed();
  mxArray* mxelapsed = mxCreateDoubleMatrix(1,1,mxREAL);
  memcpy(mxGetPr(mxelapsed), &elapsed,  sizeof(double));   


#ifdef USEPANTIL
  float pan, tilt;
  ptu.getCurrentPanTilt(pan,tilt);

  mxArray* mx_pantilt = pt_adapter(pan,tilt);
#endif

  //Push into Workspace
  workspace->put_array("rgb", mx_rgb);
  workspace->put_array("xyz", mx_xyz);
  workspace->put_array("MappaX", mx_splam_map);
  workspace->put_array("doropose", mx_splam_pose);
  workspace->put_array("elapsed", mxelapsed);

#ifdef USEPANTIL
  workspace->put_array("pantilt",mx_pantilt);
#endif

  //Clean
  mxDestroyArray(mx_rgb);
  mxDestroyArray(mx_xyz);
  mxDestroyArray(mx_splam_map);
  mxDestroyArray(mx_splam_pose);
  mxDestroyArray(mxelapsed);

#ifdef USEPANTIL
  mxDestroyArray(mx_pantilt);
#endif
}
//###########################################################################
///
void am::attentive_search_t::call_script(const std::string& script)
{
  workspace->command_line(script);
}
//###########################################################################
///
void am::attentive_search_t::pt_move_to_screen(const float& row, const float& col)
{
#ifdef USEPANTILT
  //
  float pan, tilt;
  ptu.getCurrentPanTilt(pan,tilt);
  //
  math::pantilt_angle_t delta = pinhole.pantilt_from_pixel(row, col);
  //
  double nupan  = -delta.first  + pan;
  double nutilt = -delta.second + tilt;

  //limit range
  core::pantilt_angle_t clamped = ptu_limiter(nupan, nutilt);

  //
  ptu.setPanTilt(clamped.first, clamped.second);
  //
	ptu.awaitPosCommandCompletion();
  //
  boost::timer quit_timer;
  quit_timer.restart();
  while ( !ptu.isPTUidle())// && quit_timer.elapsed()<200) 
    {if(quit_timer.elapsed() > 1.0) break;}
#endif
}
//###########################################################################
/// 
void am::attentive_search_t::doro_delta_heading_from_screen(const float& col)
{
   double delta_pan = pinhole.deltapan_from_pixel(col);
   p3dx->set_delta_heading(math::angle(-delta_pan,math::radT));
}