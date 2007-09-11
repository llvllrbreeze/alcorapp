#ifndef simple_server_t_H_INCLUDED
#define simple_server_t_H_INCLUDED

#include "alcor/core/server_base_t.hpp"
#include <boost\shared_ptr.hpp>

//---------------------------------------------------------------------------
namespace all {

  class simple_server_t : public core::server_base_t
  {
  public:
    simple_server_t(const std::string& ini = "server.ini");

  private:
    ///callbacks
    //reset
    void reset(core::client_connection_ptr_t, core::net_packet_ptr_t);

    ///
    void shout(core::client_connection_ptr_t, core::net_packet_ptr_t);

    ///
    void helloworld(core::client_connection_ptr_t, core::net_packet_ptr_t)
      {printf("Hello World\n");};

    ///
    void info (core::client_connection_ptr_t, core::net_packet_ptr_t);
  };
//---------------------------------------------------------------------------
  //
  all::simple_server_t::simple_server_t(const std::string& unused)
  {
    //assegna porta
    set_port(99999);

    //ora definisci le funzioni che rispondono ad un determinato servizio
    //sono solo esempi!
    add_command_handler
      ("reset", boost::bind(&simple_server_t::reset, this,_1, _2));

    //
    add_command_handler
      ("helloworld",  boost::bind(&simple_server_t::helloworld, this, _1, _2));

    //
    add_command_handler
      ("shout",  boost::bind(&simple_server_t::shout, this, _1, _2));

    //
    add_command_handler
      ("inforequest",  boost::bind(&simple_server_t::info, this, _1, _2));

  }
//---------------------------------------------------------------------------
  void all::simple_server_t::reset(core::client_connection_ptr_t, core::net_packet_ptr_t packet)
  {
    printf("<<\n\nreset done>>\n\n");
  }

//---------------------------------------------------------------------------
  void all::simple_server_t::shout(core::client_connection_ptr_t, core::net_packet_ptr_t packet)
  {
    int recv = packet->buf_to_int();
    printf("EVENT-> received shout #%d\n", recv);
  }

//---------------------------------------------------------------------------
  void all::simple_server_t::info(core::client_connection_ptr_t client, core::net_packet_ptr_t packet)
  {
    //printf("get_pantilt\n");
    packet.reset(new core::net_packet_t);

    //
    packet->double_to_buf(3.12332);
    //
    packet->int_to_buf(42);
    

    //printf(" send_answer_packet\n");
    send_answer_packet("inforequest", client, packet);
  }
//---------------------------------------------------------------------------
}//namespace all
#endif //simple_server_t_H_INCLUDED