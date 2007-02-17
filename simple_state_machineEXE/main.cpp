#include "mpl_state_machine.hpp"  


struct generic_event {};

struct explore : public generic_event {};

struct reset : public generic_event {};

struct stop : public generic_event {};

struct idle : public generic_event {};


// concrete FSM implementation 
class mission : public state_machine<mission>
{
 private:
    // the list of FSM states
    enum states {
        Idle, Exploring, Stopped
      , initial_state = Idle
    };

    //actions
    void start_exploring  (explore const&);
    void resume_exploring (explore const&);

    void go_idle            (idle const&);
    void go_reset           (reset const&);
    void go_stop            (stop const&);
    
    friend class state_machine<mission>;

    typedef mission m; // makes transition table cleaner

    // transition table
    struct transition_table : mpl::vector6<

    //    Start     Event         Next      Action
    //  +-----------+-------------+-------------+----------------------+
    row<  Idle      , explore     , Exploring   ,&m::start_exploring  >,
    //  +-----------+-------------+-------------+----------------------+
    row<  Exploring , idle        , Idle        ,&m::go_idle          >,
    row<  Exploring , reset       , Idle        ,&m::go_reset         >,
    row<  Exploring , stop        , Stopped     ,&m::go_stop          >,
    //  +-----------+-------------+-------------+----------------------+
    row<  Stopped   , explore     , Exploring   ,&m::resume_exploring  >,
    row<  Stopped   , idle        , Idle        ,&m::go_idle           >
    //  +-----------+-------------+-------------+----------------------+
    > {};

    int somedata;
    void somemethod(){};
};

void mission::start_exploring  (explore const&) {printf("start_exploring\n");};
void mission::resume_exploring (explore const&){printf("resume_exploring\n");};

void mission::go_idle           (idle const&){printf("go_idle\n");};
void mission::go_reset          (reset const&){printf("go_reset\n");};
void mission::go_stop           (stop const&){printf("go_stop\n");};



int main()
{
    mission m;                      // An instance of the FSM

    m.process_event( explore()  );  
    m.process_event( reset()    );  
    m.process_event( idle()     );
    m.process_event( stop()     );
    m.process_event( explore()  );
    m.process_event( stop()     );
    m.process_event( explore()  );
    m.process_event( reset()    );

    return 0;
}
