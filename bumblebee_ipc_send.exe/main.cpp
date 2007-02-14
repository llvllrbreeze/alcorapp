#include "alcor/sense/bumblebee_ipc_t.h"
#include "alcor/core/core.h"

#include <string>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

using namespace all;

void main(int argc, char* argv[])
	{
  //
  std::string config;
  //
  sense::bumblebee_ipc_t ipc_grabber;

  if(argc>1)
    config = argv[1];
  else
   config = "config/bumblebeeA.ini";

  if(  ipc_grabber.open(config) )
  {
    //Thread has started.
    getchar();  
    //
    ipc_grabber.cancel();
  }
  else
    printf("Quitting!\n");


  return;
	}