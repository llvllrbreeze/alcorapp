/*

    Copyright David Abrahams 2003-2004
    Copyright Aleksey Gurtovoy 2003-2004

    Distributed under the Boost Software License, Version 1.0. 
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
            
    This file was automatically extracted from the source of 
    "C++ Template Metaprogramming", by David Abrahams and 
    Aleksey Gurtovoy.

    It was built successfully with GCC 3.4.2 on Windows using
    the following command: 

        g++ -I..\..\boost_1_32_0  -o%TEMP%\metaprogram-chapter11-example16.exe example16.cpp
        

*/

#ifndef __MPL_STATE_MACHINE_HPP_INCLUDED__
#define __MPL_STATE_MACHINE_HPP_INCLUDED__

#include <boost/mpl/fold.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/type_traits/is_same.hpp>
#include <vector>
#include <ctime>
#include <boost/mpl/vector.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
namespace mpl = boost::mpl;
using namespace mpl::placeholders;

#include <cassert>

template<
    class Transition
  , class Next
>
struct event_dispatcher
{
    typedef typename Transition::fsm_t fsm_t;
    typedef typename Transition::event event;

    static int dispatch(
        fsm_t& fsm, int state, event const& e)
    {
        if (state == Transition::current_state)
        {
            Transition::execute(fsm, e);
            return Transition::next_state;
        }
        else // move on to the next node in the chain.
        {
            return Next::dispatch(fsm, state, e);
        }
    }
};



template <class Derived> class state_machine;

struct default_event_dispatcher
{
    template<class FSM, class Event>
    static int dispatch(
        state_machine<FSM>& m, int state, Event const& e)
    {
        return m.call_no_transition(state, e);
    }
};


    template<class Table, class Event>
    struct generate_dispatcher;

template<class Derived>
class state_machine
{
    // ...
 protected:
    template<
        int CurrentState
      , class Event
      , int NextState
      , void (Derived::*action)(Event const&)
    >
    struct row
    {
        // for later use by our metaprogram
        static int const current_state = CurrentState;
        static int const next_state = NextState;
        typedef Event event;
        typedef Derived fsm_t;

        // do the transition action.
        static void execute(Derived& fsm, Event const& e)
        {
            (fsm.*action)(e);
        }
    };

    
    friend class default_event_dispatcher;
    
    template <class Event>
    int call_no_transition(int state, Event const& e)
    {
        return static_cast<Derived*>(this)  // CRTP downcast
                   ->no_transition(state, e);
    }
    // 
public:

template<class Event>
int process_event(Event const& evt)
{
    // generate the dispatcher type.
    typedef typename generate_dispatcher<
        typename Derived::transition_table, Event
    >::type dispatcher;

    // dispatch the event.
    this->state = dispatcher::dispatch(
        *static_cast<Derived*>(this)        // CRTP downcast
      , this->state
      , evt
    );

    // return the new state
    return this->state;
}

// ...
 protected:
    state_machine()
      : state(Derived::initial_state)
    {
    }

 private:
    int state;
// ...

// ...
 public:
    template <class Event>
    int no_transition(int state, Event const& e)
    {
        printf("no event\n");
        assert(false);
        return state;
    }
// ...
////
  };


// get the Event associated with a transition.
template <class Transition>
struct transition_event
{
    typedef typename Transition::event type;
};

template<class Table, class Event>
struct generate_dispatcher
  : mpl::fold<
        mpl::filter_view<   // select rows triggered by Event
            Table
          , boost::is_same<Event, transition_event<_1> >
        >
      , default_event_dispatcher
      , event_dispatcher<_2,_1> 
    >
{};


#endif //__MPL_STATE_MACHINE_HPP_INCLUDED__