#include "alcor/act/directed_perception_ptu_server_t.h"

using namespace all;

int main (int argc, char* argv[])
{
  act::directed_perception_sptr ptu (new act::directed_perception_ptu_t);
  ptu->open("config/dpptu_conf.ini");

  printf("ptuserver\n");
  act::directed_perception_ptu_server_t ptuserver("config/ptu_server.ini");

  printf("ptuserver.assign_ptu\n");
  ptuserver.assign_ptu(ptu->getshared());

  ptuserver.run_async();
  getchar();

return 0;
}