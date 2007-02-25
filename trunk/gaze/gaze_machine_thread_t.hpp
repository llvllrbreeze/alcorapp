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
        gaze_ = all::gaze::create_gaze_machine();
        gaze_->print_welcome();
    }

    void run_thread()
    {
      //int device in the thread loop!
        if(gaze_->start_machine())
        {
            while (running_)
            {
                gaze_->sample_gaze();
                boost::thread::yield();
                all::core::BOOST_SLEEP(75);
            }
            elapsed_=gaze_->elapsed();
        }
        else
            printf("devices not started!\n");  

    }

    int nsamples() const {return gaze_->nsamples();};
    double elapsed()const {return elapsed_;};

    void cancel(){running_=false;};

private:
    gaze::gaze_machine_ptr gaze_;
    volatile bool running_;
    double elapsed_;
};
//-------------------------------------------------------------------------++
}}
#endif //gaze_machine_thread_H_INCLUDED