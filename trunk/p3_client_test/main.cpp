#include "alcor/act/p3_server_t.h"
#include "alcor/act/p3_client_t.h"
//-----------------------------------------------
#ifdef _MSC_VER
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
//-----------------------------------------------
#elif __GNUC__

#endif

#define ARIA_STATIC
//-----------------------------------------------
using namespace all;

//#########################################################
class p3_listener
{
public:
  ///ctor
  p3_listener()
  {
    std::string configname   = "config/p3_conf.ini";

    p3.reset(new act::p3_client_t(configname.c_str()));
    //opzionale ... 
    p3->set_user_callback(boost::bind(&p3_listener::update_pose,this,_1) );

    p3->run_async();
  }

private:
  ///callback invocato dal server per aggiornare la posa.
  void update_pose(const math::pose2d& pose)
  {
    p3data.pose = pose;
  }

  ///puntatore al client del robot
  act::p3_client_ptr_t  p3;

  ///dati che ricevo dal server del robot
  act::p3_server_data_t p3data;
};

//#########################################################
int main ()
{
  ///listener (client) del robot
  p3_listener   p3_client;

  ///posizione
  all::math::point2d target;

  ///  sta fermo .. comne azione.
  p3_client.p3->enable_stop_mode();

  getchar();

  ///rendo attiva una azione (o comportamento)
  printf("enabling goto\n");
  p3_client.p3->enable_goto_mode();

  getchar();
  ///setto un target relativo
  target.set(1.0, math::angle(0, math::deg_tag));
  p3_client.p3->set_relative_goto(target, 200);

  getchar();
    printf("-25\n");
  target.set(1.0, math::angle(-25, math::deg_tag));
  p3_client.p3->set_relative_goto(target, 100);

  getchar();
      printf("0.0\n");
  target.set(1.0, math::angle(0.0, math::deg_tag));
  p3_client.p3->set_relative_goto(target, 100);

  getchar();
      printf("50.0\n");
  target.set(1.0, math::angle(50.0, math::deg_tag));
  p3_client.p3->set_relative_goto(target, 100);

  getchar();
  p3_client.p3->enable_stop_mode();

   getchar(); 

   p3_client.p3->stop();


  getchar();
  return 0;
}