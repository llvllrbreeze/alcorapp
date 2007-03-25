#ifndef gaze_machine_thread_t_H_INCLUDED
#define gaze_machine_thread_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include "alcor.apps/gaze/gaze_machine_t.h"
#include <boost/thread/thread.hpp>
//-------------------------------------------------------------------------++
namespace all { namespace gaze{

class gaze_machine_thread_t
{
public:
    gaze_machine_thread_t():running_(true),elapsed_(0)
    {

    }

    void run_thread()
    {      
      
      gaze_.reset(new gaze_machine_t);
      gaze_->print_welcome();

      //int device in the thread loop!
        if(gaze_->start_machine())
        {
            gaze_->start_timing();

            while (running_)
            {
                gaze_->sample_gaze();
                boost::thread::yield();
                all::core::BOOST_SLEEP(50);
            }
            printf("Thread Canceled\n");
            elapsed_=gaze_->elapsed();
        }
        else
            printf("devices not started!\n");  

    }

    int     nsamples()  const {return gaze_->nsamples();};
    double  elapsed()   const {return elapsed_;};

    void cancel(){running_=false;};

private:
    gaze::gaze_machine_ptr gaze_;
    volatile bool running_;
    double elapsed_;
};
//-------------------------------------------------------------------------++
}}
#endif //gaze_machine_thread_H_INCLUDED