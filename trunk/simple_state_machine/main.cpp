#include "alcor.extern/statemachine/state_machine.hpp"
#include <boost/mpl/list.hpp>

#include <iostream>

#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <boost\thread\thread.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\thread\mutex.hpp>

#include <windows.h>

namespace mpl = boost::mpl;

// concrete FSM implementation 
class mission : public fsm::state_machine<mission>
{
public:
  boost::mutex process_guard;
  static int cycle;

public:
  mission():running_(true)
  {
        ////
    fire = boost::bind
      (&mission::idle_cb, this);

    thisthread.reset( 
        new boost::thread
        (
            boost::bind(&mission::loop_thread, this) 
        )
      );  

  }

  void loop_thread()
  {

    while (running_)
    {

      {
        boost::mutex::scoped_lock lock(process_guard);
        fire();    
      }

    boost::thread::yield();    
    Sleep(50);
    }

  }

  struct explore_event  : event<explore_event>{};
  struct resume_event  : event<resume_event>{};
  
  struct reset_event : event<reset_event> {};
  struct stop_event : event<stop_event>{};
  struct idle_event  : event<idle_event>{};

  struct fail_event  : event<fail_event>{};

  typedef mission self_t;

    // state invariants
  void idled_state_invariant() const {printf("idled_state_invariant\n");}
  void stopped_state_invariant() const {printf("stopped_state_invariant\n");}
  void exploring_state_invariant() const {printf("exploring_state_invariant\n");}

  void fail_state_invariant() const {printf("fail_state_invariant\n");}
  

  // states (invariants are passed as non-type template arguments)
  typedef state<0, &self_t::idled_state_invariant>      idled;
  typedef state<1, &self_t::stopped_state_invariant>    stopped;
  typedef state<2, &self_t::exploring_state_invariant>  exploring;
  typedef state<3, &self_t::fail_state_invariant>       failed;

 //private:

    // transition functions
    bool start_exploring  (explore_event const&);
    bool resume_exploring (explore_event const&);
    bool go_idle            (idle_event const&);
    bool go_reset           (reset_event const&);
    bool go_stop            (stop_event const&);

    //FAIL
    bool go_fail            (fail_event const&);
    bool recover_fail       (reset_event const&);

    //per-state stuff
    typedef boost::function< void (void) > fire_func;

    fire_func fire;

    friend class fsm::state_machine<mission>;
    typedef mpl::list<
          transition<idled    , explore_event,  exploring, &self_t::start_exploring>
        //can only reset if failed ...
        , transition<failed   , reset_event  ,  idled    , &self_t::recover_fail>

        , transition<exploring, idle_event ,  idled,   &self_t::go_idle>
        , transition<exploring, reset_event,  idled,   &self_t::go_reset> 
        , transition<exploring, stop_event ,  stopped, &self_t::go_stop> 
        , transition<exploring, fail_event ,  failed,  &self_t::go_fail> 

        , transition<stopped  , idle_event ,  idled  , &self_t::go_idle>
        , transition<stopped  , reset_event , idled  , &self_t::go_reset>
        , transition<stopped  , explore_event , exploring  , &self_t::resume_exploring> 
        >::type transition_table;

    typedef idled initial_state;

    void cancel() {running_ = false;};

private:
    int somedata;
    void somemethod(){};
    //
    void idle_cb();
    void exploring_cb();
    void stopped_cb();
    void reset_cb();

    boost::shared_ptr<boost::thread> thisthread;

    volatile bool running_;
};

//###################################################################
///
bool mission::start_exploring  (explore_event const&) 
{
  printf("start_exploring\n");
  {
  boost::mutex::scoped_lock lock(process_guard);
  fire = boost::bind(&mission::exploring_cb,this);
  }
  return true;
};

bool mission::resume_exploring (explore_event const&)
{
  printf("resume_exploring\n");
    {
  boost::mutex::scoped_lock lock(process_guard);
  fire = boost::bind(&mission::exploring_cb,this);
      }
  return true;
};

bool mission::go_idle (idle_event const&)
{
  printf("go_idle\n");
    {
  boost::mutex::scoped_lock lock(process_guard);
  fire = boost::bind(&mission::idle_cb,this);
      }
  return true;
};

bool mission::go_reset    (reset_event const&)
{
  printf("go_reset\n");
  {
  boost::mutex::scoped_lock lock(process_guard);
  fire = boost::bind(&mission::reset_cb,this);
  }
  return true;
};

bool mission::go_stop     (stop_event const&)
{
  printf("go_stop\n");

    {
  boost::mutex::scoped_lock lock(process_guard);
  fire = boost::bind(&mission::stopped_cb, this);
    }
  return true;

};

//Failure management
bool mission::go_fail (fail_event const&)
{
printf("Failed\n");
    {
  boost::mutex::scoped_lock lock(process_guard);
  fire = boost::bind(&mission::idle_cb, this);
    }  
  return true;
}

bool mission::recover_fail (reset_event const&)
{
  printf("\nRecovering\n");
    {
  boost::mutex::scoped_lock lock(process_guard);
  fire = boost::bind(&mission::idle_cb, this);
    }
  return true;
}

/////////////////////////////////////////////////////////////////////
//    //
void mission::idle_cb()
{
//printf("idled\n");
};
//
void mission::exploring_cb()
{
printf("exploring_cb\n");
 mission::cycle++;
 if(mission::cycle > 100){
  process_event( mission::fail_event() );
  mission::cycle = 0;
 }
};

void mission::stopped_cb()
{
//printf("idled\n");
};

void mission::reset_cb()
{
//printf("stopped_cb\n");
};

//###################################################################
int mission::cycle = 0;


int main()
{
    mission m;                      // An instance of the FSM
  {
  boost::mutex::scoped_lock lock(m.process_guard);
  printf("explore_event\n");
    m.process_event( mission::explore_event() );
  }

    getchar();
  {
  boost::mutex::scoped_lock lock(m.process_guard);
    printf("stop_event\n");
    m.process_event( mission::stop_event()    );
  }

    getchar();
  {
  boost::mutex::scoped_lock lock(m.process_guard);
    printf("explore_event\n");
    m.process_event( mission::explore_event()    );
   }   

    getchar();  
    {
  boost::mutex::scoped_lock lock(m.process_guard);
    printf("reset_event\n");
    m.process_event( mission::reset_event()     );
   }

    getchar();
    {
  boost::mutex::scoped_lock lock(m.process_guard);
    printf("explore_event\n");
    m.process_event( mission::explore_event() );
       }

    getchar();    
    {
  boost::mutex::scoped_lock lock(m.process_guard);
    printf("idle_event\n");
    m.process_event( mission::idle_event()     );
       }
    getchar();  
    {
  boost::mutex::scoped_lock lock(m.process_guard);
    printf("reset_event\n");
    m.process_event( mission::reset_event()     );
   }

    getchar();
    {
  boost::mutex::scoped_lock lock(m.process_guard);
    printf("cancel\n");
    m.cancel();
    }

    return 0;
}
