#include "task_dispatcher.h"
#include "alcor/core/iniWrapper.h"
//---------------------------------------------------------------------------
namespace all { namespace trm {
//-------------------------------------------------------+
task_dispatcher::task_dispatcher(const std::string& inifile)
{
  iniWrapper ini;
  if ( ini.Load(inifile.c_str()) ) 
  {
		all::core::ip_address_t server_address;
    server_address.hostname = ini.GetStringAsChar("server:hostname", "127.0.0.1");
    server_address.port = ini.GetInt("server:port", 99999);
    set_server_addr(server_address);
  }
  else
  {
  }

  add_command_handler
    ("updatetask", boost::bind(&task_dispatcher::updatetask, this, _1));

  set_connect_callback(boost::bind(&task_dispatcher::connected_cb, this));
}
//-------------------------------------------------------+
  ///
void task_dispatcher::send_event(int evt)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());
  packet->int_to_buf(evt);
  send_command("taskset", packet);
}
//-------------------------------------------------------+
///
void task_dispatcher::updatetask(core::net_packet_ptr_t)
{

}
//-------------------------------------------------------+
///
void task_dispatcher::connected_cb()
{

}
//-------------------------------------------------------+
///
void task_dispatcher::send_roi(int r, int c , int h , int w)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());
  packet->int_to_buf(r);
  packet->int_to_buf(c);
  packet->int_to_buf(h);
  packet->int_to_buf(w);

  send_command("setuproi", packet);

  }
//-------------------------------------------------------+
}}
//-------------------------------------------------------+

