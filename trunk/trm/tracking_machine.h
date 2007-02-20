#ifndef tracking_machine_H_INCLUDED
#define tracking_machine_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor.extern/statemachine/state_machine.hpp"
//---------------------------------------------------------------------------
#include <boost/mpl/list.hpp>
//---------------------------------------------------------------------------
#include <iostream>
#include <memory>
//---------------------------------------------------------------------------
#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <boost\thread\thread.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\thread\mutex.hpp>
#include <boost\enable_shared_from_this.hpp>
//---------------------------------------------------------------------------
#include "trm_app_inc.h"
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
namespace mpl = boost::mpl;
//---------------------------------------------------------------------------
namespace all { namespace trm {
//---------------------------------------------------------------------------
  class tracking_machine :    public fsm::state_machine<tracking_machine>
                            , public boost::enable_shared_from_this<tracking_machine>
                            //, public
  {
  public:
    ///
  tracking_machine();

  void threadloop();


  boost::mutex process_guard;

  struct setup_event        : event<setup_event>{};
  struct reset_event        : event<reset_event> {};

  struct track_event        : event<track_event> {};
  struct idle_track_event   : event<idle_track_event>{};

  struct resume_event       : event<resume_event>{};
  
  struct fail_event         : event<fail_event>{};
  struct recover_event      : event<recover_event>{};


  typedef tracking_machine self_t;

  private:
  // state invariants
  void idled_state_invariant() const {printf("idled_state_invariant\n");}
  void setup_state_invariant() const {printf("setup_state_invariant\n");}

  void idled_tracking_invariant() const {printf("idled_tracking_invariant\n");}
  void tracking_state_invariant() const {printf("exploring_state_invariant\n");}

  void fail_state_invariant() const {printf("fail_state_invariant\n");}
  void recover_state_invariant() const {printf("recover_state_invariant\n");}

  // states (invariants are passed as non-type template arguments)
  typedef state<0, &self_t::idled_state_invariant>      idled;
  typedef state<1, &self_t::setup_state_invariant>      setup;
  typedef state<2, &self_t::idled_tracking_invariant>   idled_tracking;
  typedef state<3, &self_t::tracking_state_invariant>   tracking;
  typedef state<4, &self_t::fail_state_invariant>       failed;
  typedef state<5, &self_t::recover_state_invariant>    recovering;

  /// transition functions
  ///START_SETUP
  bool start_setup      (setup_event const&);
  ///IDLE TRACKING
  bool go_idle_tracking (idle_track_event const&);
  ///RESET
  bool go_reset         (reset_event const&);
  ///START TRACKING
  bool start_tracking   (track_event const&);
  ///RESUME TRACKING
  bool resume_tracking  (resume_event const&){return true;};
  ///FAIL
  bool go_fail          (fail_event const&){return true;};
  ///RECOVER
  bool go_recover       (recover_event const&){return true;};

  ///Loop Callback
  typedef boost::function< void (void) >  fire_callback_t;
  fire_callback_t fire_callback;

  friend class fsm::state_machine<tracking_machine>;

  ///Transitions
  typedef mpl::list<
  //...
  transition<idled  , setup_event        ,  setup          , &self_t::start_setup>
, transition<idled  , reset_event        ,  idled          , &self_t::go_reset>

, transition<setup  , idle_track_event   ,  idled_tracking , &self_t::go_idle_tracking>//ad hoc?
, transition<setup  , reset_event   ,  idled , &self_t::go_reset>//ad hoc?

///can only reset if failed ...
, transition<idled_tracking   , track_event   ,  tracking   , &self_t::start_tracking>
, transition<idled_tracking   , resume_event  ,  tracking   , &self_t::resume_tracking >
, transition<idled_tracking   , reset_event   ,  idled      , &self_t::go_reset >
, transition<idled_tracking   , setup_event   ,  setup      , &self_t::start_setup >

///
, transition<tracking , idle_track_event ,  idled_tracking , &self_t::go_idle_tracking >
, transition<tracking , reset_event ,   idled , &self_t::go_reset >
, transition<tracking , fail_event ,   failed , &self_t::go_fail >
///
, transition<failed , reset_event       , idled          , &self_t::go_reset  >
, transition<failed , recover_event     , recovering     , &self_t::go_recover >
, transition<failed , idle_track_event  , idled_tracking , &self_t::go_idle_tracking >

///
, transition<recovering , reset_event      , idled    , &self_t::go_reset >
, transition<recovering , fail_event       , failed     , &self_t::go_fail >
, transition<recovering , resume_event     , tracking , &self_t::resume_tracking >
, transition<recovering , idle_track_event , idled_tracking , &self_t::go_idle_tracking >

  >::type transition_table;

  ///
  typedef idled initial_state;

  ///
  void cancel() {running_ = false;};
  ///
  boost::shared_ptr<boost::thread> thisthread;
  ///
  volatile bool running_;

private:
  ///
  void taskreceived(int);
    //
  void idle_cb();
  void tracking_cb(){};
  void idle_tracking_cb();
  ///calls setup script
  void setup_cb();
  void failed_cb(){};
  void recovering_cb(){};

private:
  ///MATLAB
  boost::shared_ptr<matlab::matlab_engine_t> workspace;
  ///DORO CLIENT
  act::p3dx_client_sptr p3dx;
  ///
  sense::bumblebee_sptr bee;
  ///
  act::directed_perception_sptr ptu;
  //
  boost::shared_ptr<all::trm::task_listener> tasklistener;
  ///Pihole util
  math::pinhole_t pinhole;  
//---------------------------------------------------------------------------
  };
//---------------------------------------------------------------------------
}}//all::trm
//---------------------------------------------------------------------------
#endif //tracking_machine_H_INCLUDED