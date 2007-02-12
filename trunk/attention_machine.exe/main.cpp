
#include "alcor.apps/am/attentive_search_t.h"

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

using namespace all;

int main()
    {
    am::attentive_search_t machine;
  
    getchar(); 

    machine.cancel_thread();
    return 0;
    }