#ifndef attention_events_t_H_INCLUDED
#define attention_events_t_H_INCLUDED
//##########################################################
#include "alcor2/core/i_serializable.h"
#include "alcor2/am/attentive_search_inc.h"
//##########################################################
namespace all { namespace am { 
//##########################################################
 struct attention_events_t: public all::core::i_serializable
{
  ///
  attention_events_t(events::tag _tag= events::IDLE): event_tag(_tag){};
  ///
  void import(void* pkt);
  ///
  void pack(void* pkt);
  ///
  events::tag event_tag;
};
//##########################################################
}}
//##########################################################
#endif //attention_events_t_H_INCLUDED