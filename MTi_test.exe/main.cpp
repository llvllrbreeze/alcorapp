#include "alcor/sense/MTi_driver_t.h"

using namespace all;

int main()
{
  all::sense::MTi_driver_t mti;
  std::string  afile("config/mti_config.ini");
  if ( mti.open( afile ))
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