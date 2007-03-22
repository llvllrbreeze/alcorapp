#ifndef tracking_machine_H_INCLUDED
#define tracking_machine_H_INCLUDED 
//---------------------------------------------------------------------------
#include "trm_tags_inc.h"
#include "trm_app_inc.h" 
//---------------------------------------------------------------------------
#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */ 
//---------------------------------------------------------------------------
#include "alcor.extern/statemachine/state_machine.hpp" 
//---------------------------------------------------------------------------
#include <boost/mpl/list.hpp> 
//---------------------------------------------------------------------------
#include <iostream>
//---------------------------------------------------------------------------
#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <boost\thread\thread.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\thread\mutex.hpp> 
//---------------------------------------------------------------------------
namespace mpl = boost::mpl;
//---------------------------------------------------------------------------
namespace all {
namespace trm {
//---------------------------------------------------------------------------
class tracking_machine : public fsm::state_machine<tracking_machine>
      //, public boost::enable_shared_from_this<tracking_machine>

{
public:
  ///
  tracking_machine();
  ///
  ~tracking_machine();

  void threadloop();


  boost::mutex process_guard;

  struct setup_event : event<setup_event> {};
  struct reset_event : event<reset_event> {};

  struct track_event : event<track_event> {};
  struct idle_track_event : event<idle_track_event> {};

  struct resume_event : event<resume_event> {};

  struct fail_event : event<fail_event> {};
  struct recover_event : event<recover_event> {};


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
  typedef state<stag::IDLED, &self_t::idled_state_invariant> idled;
  typedef state<stag::SETUP, &self_t::setup_state_invariant> setup;
  typedef state<stag::IDLED_TRACKING, &self_t::idled_tracking_invariant> idled_tracking;
  typedef state<stag::TRACKING, &self_t::tracking_state_invariant> tracking;
  typedef state<stag::FAILED, &self_t::fail_state_invariant> failed;
  typedef state<stag::RECOVERING, &self_t::recover_state_invariant> recovering;

  /// transition functions
  ///START_SETUP
  bool start_setup (setup_event const&);
  ///IDLE TRACKING
  bool go_idle_tracking (idle_track_event const&);
  ///RESET
  bool go_reset (reset_event const&);
  ///START TRACKING
  bool start_tracking (track_event const&);
  ///RESUME TRACKING
  bool resume_tracking (resume_event const&) { return true;};
  ///FAIL
  bool go_fail (fail_event const&);
  ///RECOVER
  bool go_recover (recover_event const&) { return true;};

  ///Loop Callback
  typedef boost::function < void (void) > fire_callback_t;
  fire_callback_t fire_callback;

  friend class fsm::state_machine<tracking_machine>;

  ///Transitions
  typedef mpl::list <
  //...
  transition<idled , setup_event , setup , &self_t::start_setup>
  , transition<idled , reset_event , idled , &self_t::go_reset>

  , transition<setup , idle_track_event , idled_tracking , &self_t::go_idle_tracking> //ad hoc?
  , transition<setup , reset_event , idled , &self_t::go_reset> //ad hoc?

  ///can only reset if failed ...
  , transition<idled_tracking , track_event , tracking , &self_t::start_tracking>
  , transition<idled_tracking , resume_event , tracking , &self_t::resume_tracking >
  , transition<idled_tracking , reset_event , idled , &self_t::go_reset >
  , transition<idled_tracking , setup_event , setup , &self_t::start_setup >

  ///
  , transition<tracking , idle_track_event , idled_tracking , &self_t::go_idle_tracking >
  , transition<tracking , reset_event , idled , &self_t::go_reset >
  , transition<tracking , fail_event , failed , &self_t::go_fail >
  ///
  , transition<failed , reset_event , idled , &self_t::go_reset >
  , transition<failed , recover_event , recovering , &self_t::go_recover >
  , transition<failed , idle_track_event , idled_tracking , &self_t::go_idle_tracking >

  ///
  , transition<recovering , reset_event , idled , &self_t::go_reset >
  , transition<recovering , fail_event , failed , &self_t::go_fail >
  , transition<recovering , resume_event , tracking , &self_t::resume_tracking >
  , transition<recovering , idle_track_event , idled_tracking , &self_t::go_idle_tracking >

  > ::type transition_table;

  ///
  typedef idled initial_state;

  ///
  void cancel() {running_ = false;};
  ///
  boost::shared_ptr<boost::thread> thisthread;


private:
  ///
  volatile bool running_;
  ///
  void idle_cb();
  ///
  void tracking_cb();
  ///
  void idle_tracking_cb();
  ///calls setup script
  void setup_cb();
  ///
  void failed_cb();
  ///
  void recovering_cb() {};

private: //routines
  ///
  void move_ptu_to_screen_rc(float row, float col, double waitsec = -1);
  ///
  void taskreceived(int);
  ///
  void setup_roi(int r, int c, int h, int w, int scala);

private:
  ///MATLAB
  boost::shared_ptr<matlab::matlab_engine_t> workspace;
  ///DORO CLIENT
  //act::p3_gateway_ptr_t p3dx;
  act::p3_client_ptr_t p3dx;

  ///Bumblebee
  sense::bumblebee_sptr bee;
  ///PTU
  act::directed_perception_sptr ptu;

  //EVENTS
  boost::shared_ptr<all::trm::task_listener> tasklistener;
  ///Pinhole util
  math::pinhole_t pinhole;

  //VIEW
  //boost::shared_ptr<cimglib::CImgDisplay>          rgb_win;
  //boost::shared_ptr<cimglib::CImg<core::uint8_t> > rgb_cimg;

  //IMAGE STREAM
  all::core::memory_stream_source_sptr_t  stream_source_ptr;
  all::core::stream_server_t*             stream_server_ptr; 

  //Tracking Data
  //ROI (setup)
  int r_roi;
  int c_roi;
  int h_roi;
  int w_roi;
  //
  double scala_resize;

  //target w/respect to the robot ref system
  double glo_theta_target;
  //---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------
}
} //all::trm
//---------------------------------------------------------------------------
#endif //tracking_machine_H_INCLUDED
