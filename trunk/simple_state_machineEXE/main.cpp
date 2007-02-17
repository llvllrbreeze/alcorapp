#include "state_machine.hpp"
#include <boost/mpl/list.hpp>

#include <iostream>

#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>


namespace mpl = boost::mpl;

// concrete FSM implementation 
class mission : public fsm::state_machine<mission>
{
public:
  mission()
  {
    ////
    fire = boost::bind
      (&mission::idle_cb, this);
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
    boost::function< void (void) > fire;

    //
    void idle_cb();
    void exploring_cb();
    void stopped_cb();
    void reset_cb();


    friend class fsm::state_machine<mission>;
    typedef mpl::list<
          transition<idled    , explore_event,  exploring, &mission::start_exploring>

        , transition<exploring, idle_event ,  idled,   &mission::go_idle>
        , transition<exploring, reset_event,  idled,   &mission::go_reset> 
        , transition<exploring, stop_event ,  stopped, &mission::go_stop> 

        , transition<stopped  , stop_event ,  stopped, &mission::go_stop>
        , transition<stopped  , idle_event ,  idled  , &mission::go_idle>
        , transition<stopped  , reset_event , idled  , &mission::go_reset>
        >::type transition_table;

    typedef idled initial_state;

    int somedata;
    void somemethod(){};
};

//###################################################################
///
bool mission::start_exploring  (explore_event const&) 
  {
    printf("start_exploring\n");
    fire = boost::bind(&mission::exploring_cb,this);
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
};
//
void mission::exploring_cb()
{
};

void mission::stopped_cb()
{
};

void mission::reset_cb()
{
};

//###################################################################

int main()
{
    mission m;                      // An instance of the FSM

    m.process_event( mission::explore_event() );  
    m.process_event( mission::stop_event()    );  
    m.process_event( mission::reset_event()     );
 
    m.process_event( mission::explore_event() );
    m.process_event( mission::idle_event()     );

    return 0;
}
