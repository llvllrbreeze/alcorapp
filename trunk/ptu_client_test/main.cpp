#include "alcor/act/directed_perception_client_t.h"

using namespace all;

int main (int argc, char* argv[])
{
  all::act::directed_perception_client_t ptu("config/ptu_server.ini");
  ptu.run_async();
  getchar();
  ptu.set_pantilt(0,0);
  getchar();
  ptu.set_pantilt(90,10);
  getchar();
  float pan,tilt;
  ptu.get_pantilt(pan,tilt);
  printf("Pan: %f Tilt: %f\n",pan,tilt);
  getchar();
  printf("Pan: %f Tilt: %f\n",pan,tilt);
  getchar();
  return 0;
}