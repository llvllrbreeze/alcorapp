#include "exploring_machine.h"
//-----------------------------------------------------------------------++
///experimental
#include "alcor/core/pixel_traits.h"
#include "alcor/core/depth_image_utils.hpp"
//---------------------------------------------------------------------------
namespace all { namespace  xpr {
//---------------------------------------------------------------------------
  //ctor
  exploring_machine::exploring_machine()
  {
  //
  //fire_callback = boost::bind
  //  (&exploring_machine::idled_cb, this);

  //Task Listener
  tasklistener.reset(new task_listener("config/xrp_service.ini"));
  tasklistener->notify_evt = boost::bind(&exploring_machine::taskreceived, this, ::_1);
  tasklistener->run_async();

  //bumblebee
  bee.reset(new sense::bumblebee_driver_t());
  bee->open("config/bumblebeeB.ini");

  //PTU
  ptu.reset (new act::directed_perception_ptu_t);
  ptu->open("config/dpptu_conf.ini");

  //PINHOLE
  pinhole.focal = bee->focal();
  pinhole.ncols = bee->ncols();
  pinhole.nrows = bee->nrows();

 //P3AT
  p3at.reset(new act::p3_gateway()); 
  ///
  if (p3at->open("config/p3_conf.ini"))
        printf("Robot connected!\n");

  //STREAMING

  //Initiate thread
  thisthread.reset( 
      new boost::thread
      (
          boost::bind(&exploring_machine::threadloop, this) 
      )
    ); 
  }
//---------------------------------------------------------------------------
  ///
  void exploring_machine::taskreceived(int evt)
  {
    boost::mutex::scoped_lock lock(process_guard);

    switch (evt)
    {  
    case etag::RESET :
      process_event(exploring_machine::reset_evt() );
      break;

    case etag::IDLE :
      process_event(exploring_machine::idle_evt() );
      break;

    case etag::EXPLORE :
      process_event(exploring_machine::explore_evt() );
      break;

    case etag::RESUME :
      process_event(exploring_machine::resume_evt() );
      break;

    case etag::VISIT :
      process_event(exploring_machine::visit_evt() );
      break;

    default:
      break;
    }
  }
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void exploring_machine::threadloop()
  {
  ///
  workspace.reset( new matlab::matlab_engine_t);
  //workspace->command_line("cd tracking");
  //workspace->command_line("init_tracking");

    while (running_)
    {
      {
        boost::mutex::scoped_lock lock(process_guard);
        if(!fire_callback.empty())
              fire_callback();    
      }
    boost::thread::yield();    
    core::BOOST_SLEEP(50);
    }
  }
//---------------------------------------------------------------------------
  //[FIRE callbacks]
//---------------------------------------------------------------------------
  //idled
  void exploring_machine::idled_cb()
  {
  }

  ///
  void exploring_machine::exploring_cb()
  {
  }

  ///
  void exploring_machine::observing_cb()
  {
  }

  ///
  void exploring_machine::visiting_cb()
  {
  }
//---------------------------------------------------------------------------
  //[TRANSITION FUNCTIONS]
  //
  bool exploring_machine::go_explore (explore_evt const&)
  {
    return true;
  }
  ///
  bool exploring_machine::go_explore  (resume_evt  const&)
  {
    return true;
  }
  //
  bool exploring_machine::go_idle    (idle_evt const&)
  {
    return true;
  }
  //
  bool exploring_machine::go_idle    (reset_evt const&)
  {
    return true;
  }
  //
  bool exploring_machine::go_visit   (visit_evt const&)
  {
    return true;
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}}//all::xpr
//---------------------------------------------------------------------------