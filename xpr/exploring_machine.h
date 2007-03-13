#pragma once 
//---------------------------------------------------------------------------
#include "alcor.apps/xpr/xpr_tags_inc.h"
#include "alcor.apps/xpr/xpr_app_inc.h"
//---------------------------------------------------------------------------
namespace mpl = boost::mpl;
//---------------------------------------------------------------------------
namespace all { namespace xpr {
//---------------------------------------------------------------------------
class exploring_machine : public fsm::state_machine<exploring_machine>
{
public:
  ///ctor
  tracking_machine();

  /**
   *  the thread loop.
   */
  void threadloop();

  /**
   *  Guards transitions.
   */
  boost::mutex process_guard;

  ///
  struct init_evt : event<init_evt> {};
  ///
  struct reset_evt : event<reset_evt> {};
  ///
  struct explore_evt : event<explore_evt> {};
  ///
  struct idle_explore_evt : event<idle_explore_evt> {};
  ///
  struct resume_evt : event<resume_evt> {};
  ///
  struct fail_evt : event<fail_evt> {};
  ///
  typedef exploring_machine self_t;

private:
   
 //IDLED     
 //EXPLORING 
 //OBSERVING 
 //VISITING  
 //FAILED    

  // state invariants
  void idled_state_invariant() const {}
  void exploring_state_invariant() const {}
  void observing_state_invariant() const {}
  void visting_state_invariant() const {}
  void failed_state_invariant() const {}

  // states (invariants are passed as non-type template arguments)
  typedef state<stag::IDLED, &self_t::idled_state_invariant> 
      idled;

  typedef state<stag::EXPLORING, &self_t::exploring_state_invariant> 
      exploring;

  typedef state<stag::OBSERVING, &self_t::observing_state_invariant> 
      observing;

  typedef state<stag::VISITING, &self_t::visting_state_invariant> 
      visiting;

  typedef state<stag::FAILED, &self_t::fail_state_invariant> 
      failed;

  friend class fsm::state_machine<exploring_machine>;

  ///Transitions
  //IDLE   
  //EXPLORE
  //RESUME 
  //VISIT  
  //FAIL   

  ///Transitions
  typedef mpl::list <
  //...
    transition  <idled , explore_evt  , observing ,  &self_t::go_explre>

  , transition  <exploring , idle_evt     , idled     ,  &self_t::go_idle>
  , transition  <observing , idle_evt     , idled     ,  &self_t::go_idle>

  > ::type transition_table;

  ///Initial State.
  typedef idled initial_state;

  bool go_explore (explore_evt const&);
  bool go_idle    (idle_evt const&);


};
//---------------------------------------------------------------------------
}}//all::xrp