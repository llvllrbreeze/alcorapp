#ifndef task_dispatcher_T_HPP_INCLUDED
#define task_dispatcher_T_HPP_INCLUDED
//---------------------------------------------------------------------------
#include "xpr_tags_inc.h"
//---------------------------------------------------------------------------
#include "alcor/core/client_base_t.hpp"
//---------------------------------------------------------------------------
#include "alcor/core/config_parser_t.hpp"
//---------------------------------------------------------------------------
namespace all { namespace xpr {
//---------------------------------------------------------------------------

  class task_dispatcher_t : public all::core::client_base_t
  {
  public:
      ///
      task_dispatcher_t(const std::string& ini);
      ///
      void send_event(int);
      /////
      //void send_roi(int r, int c , int h , int w, int scala);

  private:
      ///
      void connected_cb();
      ///
      void updatetask(core::net_packet_ptr_t);
  };

//-------------------------------------------------------+
inline task_dispatcher_t::task_dispatcher_t(const std::string& inifile)
{
  all::core::config_parser_t ini;
  if ( ini.load(core::tags::ini, inifile) ) 
  {
		all::core::ip_address_t server_address;
    server_address.hostname = ini.get<std::string>("server.hostname", "127.0.0.1");
    server_address.port = ini.get<int>("server.port", 55055);
    set_server_addr(server_address);
  }
  else
  {
  }

  add_command_handler
    ("updatetask", boost::bind(&task_dispatcher_t::updatetask, this, _1));

  set_connect_callback(boost::bind(&task_dispatcher_t::connected_cb, this));
}
//-------------------------------------------------------+
  ///
inline void task_dispatcher_t::send_event(int evt)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());
  packet->int_to_buf(evt);
  send_command("taskset", packet);
}
//-------------------------------------------------------+
///
inline void task_dispatcher_t::updatetask(core::net_packet_ptr_t)
{

}
//-------------------------------------------------------+
///
inline void task_dispatcher_t::connected_cb()
{

}
////-------------------------------------------------------+
/////
//inline void task_dispatcher_t::send_roi(int r, int c , int h , int w, int scala)
//{
//  core::net_packet_ptr_t packet(new core::net_packet_t());
//
//  packet->int_to_buf(r);
//  packet->int_to_buf(c);
//  packet->int_to_buf(h);
//  packet->int_to_buf(w);
//  packet->int_to_buf(scala);
//
//  send_command("setuproi", packet);
//
//  }
//-------------------------------------------------------+

//-------------------------------------------------------+
//---------------------------------------------------------------------------
}}
//---------------------------------------------------------------------------
#endif //task_dispatcher_T_HPP_INCLUDED