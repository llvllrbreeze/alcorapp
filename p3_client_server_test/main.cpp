#include "alcor/act/p3_server_t.h"
#include "alcor/act/p3_client_t.h"
//-----------------------------------------------
#pragma comment (lib,"p3_gateway.lib")
//-----------------------------------------------
#pragma comment (lib,"iniWrapper.lib")
//-----------------------------------------------
#pragma comment (lib,"client_server_t.lib")
#pragma comment (lib,"p3_client_server.lib")
//-----------------------------------------------
#pragma comment (lib,"AriaStatic.lib")
#pragma comment (lib,"ArNetworkingStatic.lib")
#define ARIA_STATIC
//-----------------------------------------------
using namespace all;

struct p3_user
{
  p3_user();

  act::p3_server_ptr_t  p3_server;
  act::p3_gateway_ptr_t p3;

};

p3_user::p3_user()
{
  std::string configname   = "config/p3_conf.ini";
  p3.reset(new act::p3_gateway());
  p3->open(configname);
  
  p3_server.reset(new act::p3_server_t(configname.c_str()));
  p3_server->set_gateway_ptr(p3);
  p3_server->run_async();
}


struct p3_listener
{
  p3_listener()
  {
    std::string configname   = "config/p3_conf.ini";
    p3.reset(new act::p3_client_t(configname.c_str()));
    p3->run_async();
  }

  act::p3_client_ptr_t  p3;
  act::p3_server_data_t p3data;
};


int main ()
{
  p3_user       p3_server;
  p3_listener   p3_client;
  getchar();
  p3_client.p3->enable_stop_mode();
  getchar();
  p3_client.p3->set_vel(150);
  getchar();
  p3_client.p3->enable_stop_mode();
  getchar();
  return 0;
}