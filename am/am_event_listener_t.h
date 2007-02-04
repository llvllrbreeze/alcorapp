#ifndef am_event_listener_t_H_INCLUDED
#define am_event_listener_t_H_INCLUDED

#pragma comment(lib,"am_event_listener_t.lib")

#include "alcor/core/i_observer.h"
//#include <boost/shared_ptr.hpp>
#include <memory>
#include "alcor.apps/am/attentive_search_inc.h"

namespace all { namespace am {

struct am_event_listener_t : public all::core::i_observable
{
am_event_listener_t(){};

//virtual void start()=0;
virtual events::tag get_event() const = 0;

};

}}//namespaces


namespace all {  namespace am {

  std::auto_ptr<am_event_listener_t> 
    create_listener();

}}//namespaces

#endif //am_event_listener_t_H_INCLUDED