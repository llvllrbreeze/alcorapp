#ifndef attentive_search_t_H_INCLUDED 
#define attentive_search_t_H_INCLUDED
//-----------------------------------------------------------------------++
#include "alcor2/act/p3dx_client_t.h"
//-----------------------------------------------------------------------++
#include "alcor2/matlab/matlab_engine_t.h"
//-----------------------------------------------------------------------++
#include "alcor2/am/attentive_search_inc.h"
#include "alcor2/core/Singleton/singleton.hpp"
#include "alcor2/am/am_event_listener_t.h"
//-----------------------------------------------------------------------++
//device
#include "alcor2/sense/bumblebee_ipc_recv_t.h"
#include "alcor2/act/ltiDirectedPerceptionPTU.h"
//-----------------------------------------------------------------------++
#include "alcor2/splam/splam_client_t.h"
//-----------------------------------------------------------------------++
//UTILS
#include "alcor2/act/pan_tilt_limiter_t.hpp"
#include "alcor2/math/pinhole_utils.hpp"
#include "alcor\slam\slam_data_mx_adapter_t.hpp"
#include "alcor2\matlab\pantilt_mx_adapter.hpp"
//-----------------------------------------------------------------------++
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/timer.hpp>
//-----------------------------------------------------------------------++
#include <iostream>
using std::cout;
using std::endl;
//-----------------------------------------------------------------------++
namespace all { namespace am {
//-----------------------------------------------------------------------++
  class attentive_search_t : public all::core::i_observer
{
public:
  ///
  attentive_search_t();
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
//###########################################################################
#include "detail/debug_mode_t.h"
friend debug_mode_t;
friend show_images_t;
friend idle_debug_t;
//###########################################################################
public:
	///IDLE
  typedef Dot::Singleton<listen_t>	              listen;
  typedef Dot::Singleton<attentive_loop_t>	      attentive_loop;
  typedef Dot::Singleton<idle_search_t>	          idle_search;
  typedef Dot::Singleton<show_images_t>	          show_images;
  typedef Dot::Singleton<idle_debug_t>            idle_debug;


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
  ///
  void reset_devices();
	///
  bool grab_data();  
  ///
  void push_data();
  ///
  void call_script(const std::string&);
  ///
  void call_function(const std::string&); 
  ///
  double read_flag(const std::string&);
  ///

  ///

  ///
  void pt_move_to_screen(const float& row, const float& col);
  /// 
  void doro_delta_heading_from_screen(const float& col);


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
  boost::shared_array<all::core::uint8_t::type> rgb;
  ///
  boost::shared_array<all::core::single_t::type> xyz;
  ///dati SPLAM
  SlamData splam_data; 

  ///Event Listener ..
  boost::shared_ptr<am_event_listener_t> 
      evt_listener;
   
  ///MATLAB
  std::auto_ptr<all::matlab::matlab_engine_t> workspace;
  ///BUMBLEBEE
  all::sense::bumblebee_ipc_recv_t  bumblebee;

  ///PTU
#ifdef USEPANTILT
  lti::directedPerceptionPTU        ptu;
#endif

  ///DORO CLIENT
  auto_ptr<all::act::p3dx_client_t> p3dx;
  ///SPLAM CLIENT
  auto_ptr<splam_client_t>          splam;
  
  ///UTILS
  ///slam adapter for matlab
  slam_data_mx_adapter_t splam_adapter;  
  ///Pihole util
  all::math::pinhole_t pinhole;  
  ///PTU limiter
  all::act::pan_tilt_limiter_t ptu_limiter;
  ///
  all::matlab::pantilt_mx_adapter_t pt_adapter;
  ///
  boost::timer machine_timer;
  ///Loop stop
  bool running;
  ///counter...
  int m_count;
  };

//###########################################################################
}}
//###########################################################################
#endif //attentive_search_t_H_INCLUDED