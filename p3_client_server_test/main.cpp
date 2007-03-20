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

//#########################################################
struct p3_listener
{
  p3_listener()
  {
    std::string configname   = "config/p3_conf.ini";

    p3.reset(new act::p3_client_t(configname.c_str()));
    //opzionale ... 
    p3->set_user_callback(boost::bind(&p3_listener::update_pose,this,_1) );

    p3->run_async();
  }

  void update_pose(const math::pose2d& pose)
  {
    p3data.pose = pose;
  }

  act::p3_client_ptr_t  p3;
  act::p3_server_data_t p3data;
};

//#########################################################
int main ()
{
  p3_listener   p3_client;
  all::math::point2d target;



  p3_client.p3->enable_stop_mode();

  getchar();

  printf("enabling goto\n");
  p3_client.p3->enable_goto_mode();

  getchar();
  target.set(4.5, math::angle(0, math::deg_tag));
  p3_client.p3->set_relative_goto(target, 200);

  getchar();
  target.set(1.5, math::angle(+20, math::deg_tag));
  p3_client.p3->set_relative_goto(target, 100);

  getchar();
  target.set(2.5, math::angle(0.0, math::deg_tag));
  p3_client.p3->set_relative_goto(target, 100);

  getchar();
  p3_client.p3->enable_stop_mode();
   getchar(); 

   p3_client.p3->stop();


  getchar();
  return 0;
}