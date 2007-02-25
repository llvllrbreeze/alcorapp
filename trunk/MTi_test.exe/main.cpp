#include "alcor/sense/MTi_driver_t.h"
#pragma comment (lib, "MTi_driver_t.lib")
using namespace all;

int main()
{
  all::sense::MTi_driver_t mti;
  if ( mti.open(std::string("config/mti_config.ini") ))
  {
    printf("MTi opened!\n");
    //
    printf("Resetting Heading\n");
    mti.reset(sense::tags::heading_reset);
    mti.reset(sense::tags::object_reset);
    //
    math::rpy_angle_t rpy = mti.get_euler();

    rpy = mti.get_euler();

    rpy = mti.get_euler();

    rpy.print();
  }
  else
    printf("Error :( \n");

  return 0;
}