#include "alcor.extern/statemachine/state_machine.hpp"
#include <boost/mpl/list.hpp>

#include <iostream>

#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <boost\thread\thread.hpp>
#include <boost\shared_ptr.hpp>

#include <windows.h>

namespace mpl = boost::mpl;

// concrete FSM implementation 
class mission : public fsm::state_machine<mission>
{
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
    //printf("fire\n");
    fire();    
    boost::thread::yield();    
    Sleep(500);
    }

  }

  struct explore_event  : event<explore_event>{};
  struct reset_event : event<reset_event> {};
  struct stop_event : event<stop_event>{};
  struct idle_event  : event<idle_event>{};

  typedef mission self_t;

    // state invariants
  void idled_state_invariant() const {}
  void stopped_state_invariant() const {}
  void exploring_state_invariant() const {}


  // states (invariants are passed as non-type template arguments)
  typedef state<0, &self_t::idled_state_invariant>      idled;
  typedef state<1, &self_t::stopped_state_invariant>    stopped;
  typedef state<2, &self_t::exploring_state_invariant>  exploring;


 //private:

    // transition functions
    bool start_exploring  (explore_event const&);
    bool resume_exploring (explore_event const&);
    bool go_idle            (idle_event const&);
    bool go_reset           (reset_event const&);
    bool go_stop            (stop_event const&);

    //per-state stuff
    typedef boost::function< void (void) > fire_func;

    fire_func fire;

    friend class fsm::state_machine<mission>;
    typedef mpl::list<
          transition<idled    , explore_event,  exploring, &self_t::start_exploring>

        , transition<exploring, idle_event ,  idled,   &self_t::go_idle>
        , transition<exploring, reset_event,  idled,   &self_t::go_reset> 
        , transition<exploring, stop_event ,  stopped, &self_t::go_stop> 

        , transition<stopped  , stop_event ,  stopped, &self_t::go_stop>
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
    this->fire = boost::bind(&mission::exploring_cb,this);
    return true;
  };

bool mission::resume_exploring (explore_event const&)
{
  printf("resume_exploring\n");
  fire = boost::bind(&mission::exploring_cb,this);
  return true;
};

bool mission::go_idle (idle_event const&)
{
  printf("go_idle\n");
  fire = boost::bind(&mission::idle_cb,this);
  return true;
};

bool mission::go_reset    (reset_event const&)
{
  printf("go_reset\n");
  fire = boost::bind(&mission::reset_cb,this);
    return true;
};

bool mission::go_stop     (stop_event const&)
{
  printf("go_stop\n");
  fire = boost::bind(&mission::stopped_cb, this);
    return true;

};
/////////////////////////////////////////////////////////////////////
//    //
void mission::idle_cb()
{
  printf("idled\n");
};
//
void mission::exploring_cb()
{
    printf("exploring_cb\n");
};

void mission::stopped_cb()
{
    printf("idled\n");
};

void mission::reset_cb()
{
    printf("stopped_cb\n");
};

//###################################################################

int main()
{
    mission m;                      // An instance of the FSM

    m.process_event( mission::explore_event() );

    getchar();

    m.process_event( mission::stop_event()    );

    getchar();

    m.process_event( mission::explore_event()    );
    
    getchar();
    m.process_event( mission::reset_event()     );
 
    getchar();
    m.process_event( mission::explore_event() );
    
    getchar();
    m.process_event( mission::idle_event()     );

    getchar();

    m.cancel();
    return 0;
}
