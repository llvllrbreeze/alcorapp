#ifndef exploring_machine_H_INCLUDED
#define exploring_machine_H_INCLUDED 
//---------------------------------------------------------------------------
#include "xpr_tags_inc.h"
#include "xpr_app_inc.h"
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
namespace all { namespace xpr {
//---------------------------------------------------------------------------
class exploring_machine : public fsm::state_machine<exploring_machine>
{
public:
  ///ctor
  exploring_machine();

  /**
   *  the thread loop.
   */
  void threadloop();

  /**
   *  Guards transitions.
   */
  boost::mutex process_guard;

  ///EVENTS
  ///
  struct reset_evt    : event<reset_evt> {};
  ///
  struct idle_evt     : event<idle_evt> {};
  ///
  struct explore_evt  : event<explore_evt> {};
  ///
  struct visit_evt    : event<visit_evt> {};
  ///
  struct resume_evt   : event<resume_evt> {};

  ///
  typedef exploring_machine self_t;

private:
  // state invariants
  //performed each time the state switch (not depending on the evt)
  void idled_state_invariant() const {}
  void exploring_state_invariant() const {}
  void observing_state_invariant() const {}
  void visting_state_invariant() const {}


  //[TRANSITION FUNCTIONS]
  ///
  bool go_explore  (explore_evt const&);
  ///
  bool go_explore  (resume_evt  const&);
  ///
  bool go_idle     (idle_evt  const&);
  bool go_idle     (reset_evt const&);
  ///
  bool go_visit    (visit_evt const&);

  // states (invariants are passed as non-type template arguments)
  typedef state<stag::IDLED, &self_t::idled_state_invariant> 
      idled;

  typedef state<stag::EXPLORING, &self_t::exploring_state_invariant> 
      exploring;

  typedef state<stag::OBSERVING, &self_t::observing_state_invariant> 
      observing;

  typedef state<stag::VISITING, &self_t::visting_state_invariant> 
      visiting;


  friend class fsm::state_machine<exploring_machine>;

  //NOTA
  //explore e observe  sono lo stesso stato praticamente
  //la tavola delle transizioni andrebbe  rivista perchè ridondante ..
  //ma lasciamola così 

  ///Transitions
  typedef mpl::list <
  //...
    transition  <idled     , explore_evt , observing ,  &self_t::go_explore>

  , transition  <exploring , reset_evt   , idled     ,  &self_t::go_idle>
  , transition  <exploring , idle_evt    , idled     ,  &self_t::go_idle>

  , transition  <observing , reset_evt   , idled     ,  &self_t::go_idle>
  , transition  <observing , idle_evt    , idled     ,  &self_t::go_idle>

  , transition  <exploring , visit_evt   , visiting  ,  &self_t::go_visit>
  , transition  <observing , visit_evt   , visiting  ,  &self_t::go_visit>

  , transition  <visiting  , reset_evt   , idled     ,  &self_t::go_idle>
  , transition  <visiting  , idle_evt    , idled     ,  &self_t::go_idle>
  , transition  <visiting  , resume_evt  , observing ,  &self_t::go_explore>

  > ::type transition_table;

  ///Initial State.
  typedef idled initial_state;

  ///Loop Callback
  typedef boost::function < void (void) > fire_callback_t;
  ///
  fire_callback_t fire_callback;


  //[Actual fire callbacks]
  ///
  void idled_cb();
  ///
  void exploring_cb();
  ///
  void observing_cb();
  ///
  void visiting_cb();

  ///
  void cancel() {running_ = false;};
  ///
  boost::shared_ptr<boost::thread> thisthread;

  ///
private:
  ///
  volatile bool running_;
  ///
  void taskreceived(int);

private:
  ///MATLAB
  boost::shared_ptr<matlab::matlab_engine_t> workspace;

  ///MAtlab Commands
  std::string fair_attention_com;
  ///
  std::string init_attention_com;


  ///Mobile Robot
  act::p3_gateway_ptr_t       p3at;
  ///The server if necessary
  act::p3_server_ptr_t        p3at_server;

  ///Bumblebee
  sense::bumblebee_driver_ptr_t bee;
  ///RGB
  core::uint8_sarr  rightim;
  ///Depth
  core::single_sarr depthim;

    //IMAGE STREAM
  all::core::memory_stream_source_sptr_t  stream_source_ptr;
  all::core::stream_server_t*             stream_server_ptr; 

  //

  ///PTU
  act::directed_perception_ptr_t ptu;

  ///TASK
  task_listener_sptr tasklistener;

  ///SPLAM
  all::splam::splam_client  splam;
  all::splam::splam_data    splamdata;

  ///Pinhole util
  math::pinhole_t pinhole;

};
//---------------------------------------------------------------------------
}}//all::xrp


#endif