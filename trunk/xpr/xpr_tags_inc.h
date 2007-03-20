#pragma once //trm_states_inc
#include <boost/config.hpp>
//---------------------------------------------------------------------------
namespace all { namespace xpr {
//---------------------------------------------------------------------------
//idled;
//setup;
//idled_tracking;
//tracking;
//failed;
//recovering;
///STATEs
struct stag
{
  BOOST_STATIC_CONSTANT(long, IDLED     = 0);
  BOOST_STATIC_CONSTANT(long, EXPLORING = 1);
  BOOST_STATIC_CONSTANT(long, OBSERVING = 2);
  BOOST_STATIC_CONSTANT(long, VISITING  = 3);
  BOOST_STATIC_CONSTANT(long, FAILED    = 4);
};

///EVENTs
struct etag
{
  BOOST_STATIC_CONSTANT(int,   IDLE       = 0);
  BOOST_STATIC_CONSTANT(int,   EXPLORE    = 1);
  BOOST_STATIC_CONSTANT(int,   RESUME     = 2);
  BOOST_STATIC_CONSTANT(int,   VISIT      = 3);
  BOOST_STATIC_CONSTANT(int,   FAIL       = 4);
  BOOST_STATIC_CONSTANT(int,   RESET      = 5);
};
//---------------------------------------------------------------------------
}}
//---------------------------------------------------------------------------