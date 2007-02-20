#include "tracking_machine.h"

//---------------------------------------------------------------------------
namespace all { namespace trm {
//---------------------------------------------------------------------------

tracking_machine::tracking_machine():running_(true)
{
    ////
  fire_callback = boost::bind
    (&tracking_machine::idle_cb, this);

  thisthread.reset( 
      new boost::thread
      (
          boost::bind(&tracking_machine::threadloop, this) 
      )
    );  
}
//---------------------------------------------------------------------------
void tracking_machine::threadloop()
  {
    while (running_)
    {
      {
        boost::mutex::scoped_lock lock(process_guard);
        fire_callback();    
      }
    boost::thread::yield();    
    Sleep(50);
    }
  }
//---------------------------------------------------------------------------
}}//all::trm
//---------------------------------------------------------------------------