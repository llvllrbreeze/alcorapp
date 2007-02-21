#include "alcor.apps\trm\tracking_machine.h"

using namespace all;

int main()
{
  trm::tracking_machine machine;
  getchar();
  machine.process_event(trm::tracking_machine::setup_event() );

  getchar();
  machine.process_event(trm::tracking_machine::reset_event() );
  
  return 0;
}