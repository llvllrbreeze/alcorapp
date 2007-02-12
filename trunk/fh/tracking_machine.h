#ifndef tracking_machine_H_INCLUDED
#define tracking_machine_H_INCLUDED
//-----------------------------------------------------------------------++
//
#include "alcor/act/p3dx_client_t.h"
#include "alcor/matlab/matlab_engine_t.h"
//device
#include "alcor/sense/bumblebee_ipc_recv_t.h"
#include "alcor/act/directed_perception_ptu_t.h"
#include "alcor/act/pantilt_control_loop_t.h"
//UTILS
#include "alcor.apps\slam\slam_data_mx_adapter_t.hpp"
#include "alcor\matlab\pantilt_mx_adapter.hpp"
#include "alcor/act/pan_tilt_limiter_t.hpp"
#include "alcor/math/pinhole_utils.hpp"
#include "alcor/math/metric_utils.h"
//-----------------------------------------------------------------------++
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include<boost/thread/xtime.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/timer.hpp>
#include <boost/bind.hpp>
//-----------------------------------------------------------------------++
#include "alcor.extern/Singleton/singleton.hpp"
//-----------------------------------------------------------------------++
namespace all { namespace fh {


  class tracking_machine
  {

  public:
    tracking_machine();
    ~tracking_machine();

  public:
    //-----------------------------------------------------------------------
    ///The main loop....
    void  run_thread();
    ///stop looping
    void  cancel_thread() {running = false;};
    ///called when ...
    void  process_event(events::tag);
    ///utility to change the current stare pointer.
    void  change_state(unit_state_t&);

  private:
    ///
    boost::shared_ptr<boost::thread> machine_thr;

    ///MATLAB
    boost::shared_ptr<all::matlab::matlab_engine_t> workspace;
    ///BUMBLEBEE
    all::sense::bumblebee_ipc_recv_t  bumblebee;
    ///PTU
    boost::shared_ptr<all::act::directed_perception_ptu_t> ptu;
    boost::shared_ptr<all::act::pantilt_control_loop_t> pt_control_loop_sptr;
    ///DORO CLIENT
    p3dx_client_sptr p3dx;
    ///Loop control
    volatile bool running;
    ///counter...
    int m_count;

  };


//-----------------------------------------------------------------------++
}};
//-----------------------------------------------------------------------++

#endif //tracking_machine_H_INCLUDED