#include "alcor.apps/gaze/gaze_machine_VI_t.h"

using namespace all;

int main(int argc, char* argv[])
{
  //
  gaze::gaze_machine_VI_t gaze_machine;
  printf("\nPress Return to start\n");
  getchar();

  if (argc > 1)
  {
    char* mode = argv[1];
    if     (strcmp("log",mode)==0)    gaze_machine.run_machine(gaze::binlog);
    else if(strcmp("show",mode)==0)   gaze_machine.run_machine(gaze::show);
    else if(strcmp("calib",mode)==0)  gaze_machine.run_machine(gaze::show);
  }
  else                              
    gaze_machine.run_machine(gaze::show);

  printf("\nPress Return to stop\n");
  getchar();
  gaze_machine.cancel();

  std::cout << "Elapsed Time: " 
      << (gaze_machine.elapsed()) 
      << " Secs." << std::endl
      << "Gaze Samples: "
      << gaze_machine.nsamples()
      << std::endl;

  std::cout << "RATE: " 
    << ( gaze_machine.nsamples()/(gaze_machine.elapsed() ) ) 
      << std::endl;

  getchar();
    
    return 0;
}