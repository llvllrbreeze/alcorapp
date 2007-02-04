#ifndef gaze_machine_thread_t_H_INCLUDED
#define gaze_machine_thread_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include "alcor.apps/gaze/gaze_machine_t.h"
#include <boost/thread/thread.hpp>
#include <windows.h>
//-------------------------------------------------------------------------++
namespace all { namespace gaze{

class gaze_machine_thread_t
{
public:
    gaze_machine_thread_t():running_(true)
    {
        gaze_ = all::gaze::create_gaze_machine();
        gaze_->print_welcome();
    }

    void run_thread()
    {
        if(gaze_->start_machine())
        {

            while (running_)
            {
                gaze_->sample_gaze();
                boost::thread::yield();
                ::Sleep(75);
            }
        }
        else
            std::cout << "devices not started!" << std::endl;

    }

    int nsamples() const {return gaze_->nsamples();};

    void cancel(){running_=false;};

private:
    all::gaze::sp_gaze_t gaze_;
    bool running_;
};
//-------------------------------------------------------------------------++
}}
#endif //gaze_machine_thread_H_INCLUDED