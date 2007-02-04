#include "attention_events_t.h"
#include "ArNetPacket.h"

using namespace all::am;

void attention_events_t::import(void* pkt)
{
  ArNetPacket* packet = static_cast<ArNetPacket*>(pkt);

  event_tag	= static_cast<events::tag>	
      (packet->bufToByte4());
}


void attention_events_t::pack(void* pkt)
{
  ArNetPacket* packet = static_cast<ArNetPacket*>(pkt);
  packet->byte4ToBuf(static_cast<int>(event_tag));
  packet->finalizePacket();
}
