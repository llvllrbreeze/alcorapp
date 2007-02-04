#ifndef am_dispatcher_t_H_INCLUDED
#define am_dispatcher_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include "alcor2/am/attentive_search_inc.h"
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------------++
#pragma comment (lib, "am_dispatcher_t.lib")
//-------------------------------------------------------------------------++
namespace all { namespace am {
//-------------------------------------------------------------------------++
  struct am_dispatcher_t
  {
    ///
    am_dispatcher_t(){};
    ///
	  virtual void run_server() = 0;
    ///
	  virtual void stop_server() = 0;
    ///
    virtual void dispatch_event(events::tag) = 0;

  };
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
namespace all { namespace am {
//-------------------------------------------------------------------------++
///
boost::shared_ptr<am_dispatcher_t> 
    create_dispatcher();

typedef boost::shared_ptr<am_dispatcher_t> am_dispatcher;
//-------------------------------------------------------------------------++
}}
#endif //am_dispatcher_t_H_INCLUDED