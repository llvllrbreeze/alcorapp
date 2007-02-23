#include "alcor/sense/bumblebee_ipc_t.h"
#include "alcor/core/core.h"

#include <string>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

using namespace all;

int main(int argc, char* argv[])
	{
  //
  std::string config;
  //
  sense::bumblebee_ipc_t ipc_grabber;

  if(argc>1)
    config = argv[1];
  else
   config = "config/bumblebeeA.ini";

  boost::shared_ptr<sense::bumblebee_driver_t> 
    beesp(new sense::bumblebee_driver_t());

  if (!beesp->open(config))
  {  
    return 1;
  }    

  ipc_grabber.assign_bumblebee(beesp);
  ipc_grabber.open();    
  getchar();
  ipc_grabber.cancel();


  return 0;
	}