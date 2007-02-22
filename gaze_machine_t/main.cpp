#include "alcor.apps/gaze/gaze_machine_thread_t.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/timer.hpp>

using namespace all;

int main()
{
    gaze::gaze_machine_thread_t gaze_machine;

    getchar();

    boost::timer time_spent;

	boost::thread gaze_thr
		(
		boost::bind(
        &gaze::gaze_machine_thread_t::run_thread
		    ,&gaze_machine) 
		);

    std::cout << "Check Everything is ok .......\nPress return to start" << std::endl;
    getchar();
    std::cout << "Running..." << std::endl;

     gaze_machine.cancel();

      std::cout << "Elapsed Time: " 
          << (time_spent.elapsed()) 
          << " Secs." << std::endl
          << "Gaze Samples: "
          << gaze_machine.nsamples()
          << std::endl;

      std::cout << "RATE: " 
          << ( gaze_machine.nsamples()/(time_spent.elapsed()) ) 
          << std::endl;

    getchar();
    
    return 0;
}