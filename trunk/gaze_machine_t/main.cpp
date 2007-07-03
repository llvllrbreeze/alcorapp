#include "alcor.apps/gaze/gaze_machine_t.h"

using namespace all;

int main()
{
  //
  gaze::gaze_machine_t gaze_machine;
  printf("\nPress Return to start\n");
  getchar();
  gaze_machine.enable_views(false);
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