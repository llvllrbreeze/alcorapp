#include "alcor/act/p3_gateway.h"
#include "alcor/act/p3_server_t.h"
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

struct p3_server
{
  p3_server()
    {
    std::string configname   = "config/p3_conf.ini";
    p3.reset(new act::p3_gateway());
    p3->open(configname);

    p3_server_.reset(new act::p3_server_t(configname.c_str()));
    p3_server_->set_gateway_ptr(p3);
    p3_server_->run_async();
    }

  act::p3_server_ptr_t  p3_server_;
  act::p3_gateway_ptr_t p3;

};

//#########################################################
int main ()
{ 

  p3_server p3;

  getchar();
  return 0;
}