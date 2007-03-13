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

};
//---------------------------------------------------------------------------
}}//all::xrp