#ifndef simple_client_H_INCLUDED
#define simple_client_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/core/client_base_t.hpp"
//---------------------------------------------------------------------------
namespace all {
/// 
class simple_client_t : public core::client_base_t
{
public:
  simple_client_t();

  //server method
  ///
  void reset();
  ///
  void shout(int);
  ///
  void helloworld();

private:
  ///
  void update_info(core::net_packet_ptr_t);
  ///
  void on_connect();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
all::simple_client_t::simple_client_t()
{
  all::core::ip_address_t server_address;

  server_address.hostname =  "127.0.0.1";
  server_address.port = 99999;
  //
  set_server_addr(server_address);

  add_command_handler
    ("inforequest", boost::bind(&all::simple_client_t::update_info, this, _1));

  set_connect_callback(boost::bind(&all::simple_client_t::on_connect, this));
}
//---------------------------------------------------------------------------
void all::simple_client_t::reset()
{
  send_command("reset");
}
//---------------------------------------------------------------------------
void all::simple_client_t::shout(int arg)
{
	core::net_packet_ptr_t packet(new core::net_packet_t());
  
  packet->int_to_buf(arg);

  send_command("shout", packet);
}
//---------------------------------------------------------------------------
void all::simple_client_t::update_info(core::net_packet_ptr_t packet)
{
  double  info1 = packet->buf_to_double();
  int     info2 = packet->buf_to_int();
  printf()
}
//---------------------------------------------------------------------------
void all::simple_client_t::on_connect() 
{
  send_request("inforequest", 100);
}
//---------------------------------------------------------------------------
}//namespace all
//---------------------------------------------------------------------------
#endif //simple_client_H_INCLUDED