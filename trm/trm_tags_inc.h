#pragma once //trm_states_inc
#include <boost/config.hpp>
//---------------------------------------------------------------------------
namespace all { namespace trm {
//---------------------------------------------------------------------------
//idled;
//setup;
//idled_tracking;
//tracking;
//failed;
//recovering;

struct stag
{
  BOOST_STATIC_CONSTANT(long, IDLED = 0);
  BOOST_STATIC_CONSTANT(long, SETUP = 1);
  BOOST_STATIC_CONSTANT(long, IDLED_TRACKING = 2);
  BOOST_STATIC_CONSTANT(long, TRACKING = 3);
  BOOST_STATIC_CONSTANT(long, FAILED = 4);
  BOOST_STATIC_CONSTANT(long, RECOVERING = 5);
};


  //struct setup_event       
  //struct reset_event       
  //struct track_event       
  //struct idle_track_event 
  //struct resume_event      
  //struct fail_event        
  //struct recover_event     

struct etag
{
  BOOST_STATIC_CONSTANT(int,   SETUP = 0);
  BOOST_STATIC_CONSTANT(int,   RESET = 1);
  BOOST_STATIC_CONSTANT(int,   TRACK = 2);
  BOOST_STATIC_CONSTANT(int,   IDLETRACK = 3);
  BOOST_STATIC_CONSTANT(int,   RESUME = 4);
  BOOST_STATIC_CONSTANT(int,   FAIL = 5);
  BOOST_STATIC_CONSTANT(int,   RECOVER = 6);
};
//---------------------------------------------------------------------------
}}
//---------------------------------------------------------------------------