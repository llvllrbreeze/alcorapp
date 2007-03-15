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
struct p3_user
{
  p3_user()
    {
    std::string configname   = "config/p3_conf.ini";
    p3.reset(new act::p3_gateway());
    p3->open(configname);

    p3_server.reset(new act::p3_server_t(configname.c_str()));
    p3_server->set_gateway_ptr(p3);
    p3_server->run_async();
    }

  act::p3_server_ptr_t  p3_server;
  act::p3_gateway_ptr_t p3;

};

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
  p3_user       p3_server;
  p3_listener   p3_client;

  getchar();
  math::pose2d localized;

  localized.set_x1(1.2);
  localized.set_x2(0.2);
  localized.set_th(34.3, math::deg_tag);

  p3_client.p3->set_slam_localized(localized);

  //getchar();
  ////p3_client.p3->enable_wander_mode();
  //getchar();
  ////p3_client.p3->set_vel(150);
  ////getchar();  
  ////p3_client.p3->enable_stop_mode();
  //getchar();

  //printf("Distance Traveled so far: %f\n",p3_client.p3data.pose.getP().magnitude());
  //printf("Pose: x1: %.3f x2: %.3f theta: %.2f\n", 
  //  p3_client.p3data.pose.get_x1(),
  //  p3_client.p3data.pose.get_x2(), 
  //  p3_client.p3data.pose.getTh().deg());

  ////oppure
  //math::pose2d pose = p3_client.p3->get_odometry();

  //printf("\nPose: x1: %.3f x2: %.3f theta: %.2f\n", 
  //  pose.get_x1(),
  //  pose.get_x2(), 
  //  pose.getTh().deg());

  getchar();

  printf("enabling\n");
  p3_client.p3->enable_follow_mode();

  getchar();
  all::math::angle  dir(45,all::math::deg_tag);

  all::math::point2d target(10.0, dir);
  p3_client.p3->set_target_to_follow(target, 0);

  getchar();

  p3_client.p3->set_target_to_follow(target, 250);

  getchar();

  p3_client.p3->enable_stop_mode();

  getchar();
  return 0;
}