#ifndef pilota_base_client_HPP_INCLUDED
#define pilota_base_client_HPP_INCLUDED
//-------------------------------------------------------------------
#include "alcor/act/directed_perception_client_t.h"
#include "alcor/act/p3_client_t.h"
//-------------------------------------------------------------------
class pilota_base_client
{
public:
  ///
  pilota_base_client();

  ///ptu
  void set_pantilt(float , float);
  void reset();

  ///p3
  ///mmperserc? ... controllare..
  void set_vel(float);
  ///gradi .. relativi
  void set_dheading(float);
  ///stops everything!
  void stop();

  ///p3 actions!
  ///wandering
  void enable_wander();
  ///todo
  void enable_wander_special();

  //trigger (take a  picture)
  void trigger_operation();

  all::act::directed_perception_client_sptr ptu;
  all::act::p3_client_ptr_t p3;
};
//-------------------------------------------------------------------
//
inline pilota_base_client::pilota_base_client()
{
  //p3
  p3.reset(new all::act::p3_client_t("config/p3at_conf.ini")); 
  //if (p3dx->open("config/p3_conf.ini"))
  p3->run_async();
  printf("Robot connected!\n");

  //ptu
  ptu.reset(new all::act::directed_perception_client_t("config/ptu_server.ini"));
  ptu->run_async();
  printf("Ptu connected!\n");
}
//-------------------------------------------------------------------
inline void pilota_base_client::set_pantilt(float pan, float tilt)
{
  ptu->set_pantilt(pan, tilt);
}
//-------------------------------------------------------------------
inline void pilota_base_client::reset()
{
  ptu->reset();
}
//-------------------------------------------------------------------
inline void pilota_base_client::set_vel(float vel)//mmpersec
{
  p3->set_vel(vel);
}
//-------------------------------------------------------------------
inline void pilota_base_client::set_dheading(float dheading)
{
  p3->set_dheading(dheading);
}
//-------------------------------------------------------------------
inline void pilota_base_client::stop()
{
  p3->enable_stop_mode();
}
//-------------------------------------------------------------------
inline void pilota_base_client::enable_wander()
{
  p3->enable_wander_mode();
}
//-------------------------------------------------------------------
inline void pilota_base_client::enable_wander_special()
{

}
//-------------------------------------------------------------------
inline void pilota_base_client::trigger_operation()
{
  p3->trigger();
}
//-------------------------------------------------------------------
#endif //pilota_base_HPP_INCLUDED