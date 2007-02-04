#ifndef attentive_search_t_H_INCLUDED 
#define attentive_search_t_H_INCLUDED

#define XODOLOG

#include <windows.h>
#include <cmath>
#include <fstream>
//-----------------------------------------------------------------------++
#include "alcor/act/p3dx_client_t.h"
//-----------------------------------------------------------------------++
#include "alcor/matlab/matlab_engine_t.h"
//-----------------------------------------------------------------------++
#include "alcor.apps/am/am_event_listener_t.h"
#include "alcor.apps/am/attentive_search_inc.h"
#include "alcor.extern/Singleton/singleton.hpp"
//-----------------------------------------------------------------------++
//device
#include "alcor/sense/bumblebee_ipc_recv_t.h"
#include "alcor/act/directed_perception_ptu_t.h"
#include "alcor/act/pantilt_control_loop_t.h"
//-----------------------------------------------------------------------++
#include "alcor.apps/splam/splam_client_t.h"
//-----------------------------------------------------------------------++
//UTILS
#include "alcor.apps\slam\slam_data_mx_adapter_t.hpp"
#include "alcor\matlab\pantilt_mx_adapter.hpp"
#include "alcor/act/pan_tilt_limiter_t.hpp"
#include "alcor/math/pinhole_utils.hpp"
#include "alcor/math/metric_utils.h"
//-----------------------------------------------------------------------++
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include<boost/thread/xtime.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/timer.hpp>
#include <boost/bind.hpp>
//-----------------------------------------------------------------------++
#include <iostream>
using std::cout;
using std::endl;
//-----------------------------------------------------------------------++
#define USEPANTILT
//namespace 
//{
//  static boost::xtime to_xtime(long milliseconds){
//  boost::xtime ret;
//  ret.sec = td.totalSeconds();
//  ret.nsec= td.fractional_seconds()*1000;
//  return ret;
//
//}
//-----------------------------------------------------------------------++
namespace all { namespace am {
//-----------------------------------------------------------------------++
  class attentive_search_t : public all::core::i_observer
{
public:
  ///
  attentive_search_t();

  ~attentive_search_t()
  {
#ifdef XODOLOG
    xlog.close();
#endif
  };
  
  ///
  void init_ptu();
//###########################################################################
#include "detail/unit_state_t.h"
friend unit_state_t;
//###########################################################################
#include "detail/idle_mode_t.h"
friend idle_mode_t;
//###########################################################################
#include "detail/search_mode_t.h"
friend search_mode_t;
friend attentive_loop_t;
friend nav_loop_t;
//###########################################################################
#include "detail/tracking_mode_t.h"
friend tracking_mode_t;
friend tracking_setup_t;
friend tracking_loop_t;
friend idle_track_t;
//###########################################################################
#include "detail/debug_mode_t.h"
friend debug_mode_t;
friend show_images_t;
friend idle_debug_t;
friend init_track_t;
//###########################################################################
public:
  //IDLE
  typedef Dot::Singleton<listen_t>	              listen;

  //SEARCH
  typedef Dot::Singleton<attentive_loop_t>	      attentive_loop;
  typedef Dot::Singleton<idle_search_t>	          idle_search;
  typedef Dot::Singleton<nav_loop_t>	            nav_loop;

  //DEBUG
  typedef Dot::Singleton<show_images_t>	          show_images;
  typedef Dot::Singleton<idle_debug_t>            idle_debug;

  //TRACK
  typedef Dot::Singleton<idle_track_t>            idle_track;
  typedef Dot::Singleton<init_track_t>            init_track;
  typedef Dot::Singleton<tracking_setup_t>        tracking_setup;
  typedef Dot::Singleton<tracking_loop_t>         tracking_loop;
//###########################################################################

public:
  //-----------------------------------------------------------------------
  ///The main loop....
  void  run_thread();
  ///stop looping
  void  cancel_thread() {running = false;};
  ///called when ...
  void  process_event(events::tag);
  ///utility to change the current stare pointer.
  void  change_state(unit_state_t&);

///////////////////////////////////////////////////////////////////////////
  //tools
public:
  ///ATTENTIVE SEARCH
  void reset_devices();
	///
  bool grab_data();  
  ///
  void push_data();

  ///TRACKING
   ///
  bool grab_tracking_data();
  ///
  void push_tracking_data();
  ///
  void pre_tracking_ini();

  //MATLAB
  ///
  void call_script(const std::string&);
  ///
  int call_function(const std::string&); 
  ///
  double read_flag(const std::string&);
  ///
  void pt_move_to_screen_coord(const float& row, const float& col, double waitsec = -1);
  /// 
  void doro_delta_heading_from_screen(const float& col);
  ///
  void doro_delta_heading(double delta_degree);


 public:   

  boost::mutex process_guard;
   ///
  void get_notified(void*);

	///Pointer to the current state instance.
	unit_state_t* current_state;

 private:
    ///
   boost::shared_ptr<boost::thread> machine_thr;

  //DATA
  ///dati BUMBLEBEE
  boost::shared_array<all::core::uint8_t> rgb;
  ///
  boost::shared_array<all::core::single_t> xyz;
  ///dati SPLAM
  SlamData splam_data; 

  ///Event Listener ..
  std::auto_ptr<am_event_listener_t> 
      evt_listener;
   
  ///MATLAB
  std::auto_ptr<all::matlab::matlab_engine_t> workspace;
  ///BUMBLEBEE
  all::sense::bumblebee_ipc_recv_t  bumblebee;

  ///PTU
  boost::shared_ptr<all::act::directed_perception_ptu_t> ptu;
  boost::shared_ptr<all::act::pantilt_control_loop_t> pt_control_loop_sptr;

  ///DORO CLIENT
  p3dx_client_sptr p3dx;
  ///SPLAM CLIENT
  boost::shared_ptr<splam_client_t>      splam;
  
  ///UTILS
  ///slam adapter for matlab
  slam_data_mx_adapter_t      splam_adapter;  
  ///Pihole util
  all::math::pinhole_t        pinhole;  
  ///PTU limiter
  //all::act::pan_tilt_limiter_t ptu_limiter;

  ///
  all::matlab::pantilt_mx_adapter_t pt_adapter;
  ///
  boost::timer machine_timer;
  ///Loop stop
  bool running;
  ///counter...
  int m_count;

  ///
  std::ofstream xlog;
  };

//###########################################################################
}}
//###########################################################################
#endif //attentive_search_t_H_INCLUDED