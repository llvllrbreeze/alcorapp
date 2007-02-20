#include "alcor/act/p3dx_gateway.h"

using namespace all;


int main()
{
  all::act::p3dx_gateway p3dx;

  p3dx.serial_connect();

  p3dx.stop_running();

  return 0;
}