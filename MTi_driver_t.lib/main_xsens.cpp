///////////////////////////////////////////////////////////////////
#include "alcor/sense/xsens_mti_driver_t.h"
#include "alcor/core/core.h"
///////////////////////////////////////////////////////////////////
using namespace all;
#define _STRNG(_some_c_str_) std::string(_some_c_str_)
///////////////////////////////////////////////////////////////////
int main()
{
  sense::xsens_mti_driver_t mti;

  if (mti.open( _STRNG("config/mti_conf.ini") ))
    printf("MTi succesfully opened\n");
  
  getchar();

  mti.run_mti();

  getchar();

  mti.stop_mti();

  core::SLEEP_MSECS(250);

  return 0;
}