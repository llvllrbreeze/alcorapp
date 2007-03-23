#define WIN32_LEAN_AND_MEAN
//---------------------------------------------------------------------------
#include "task_listener.h"
#include "alcor/core/config_parser_t.hpp"
//---------------------------------------------------------------------------
namespace all { namespace xpr {
//---------------------------------------------------------------------------
  task_listener::task_listener(const std::string& ini_file)
  {
    core::config_parser_t ini;
    if (ini.load(core::tags::ini, ini_file))
    {
      int port = ini.get<int>("server.port", 99990);
      set_port(port);
    }

    add_command_handler
      ("taskset", boost::bind(&task_listener::taskset, this, _1, _2));

  }
  //---------------------------------------------------------------------------
  ///
  void task_listener::taskset(core::client_connection_ptr_t, core::net_packet_ptr_t packet)
  {
      if(!notify_evt.empty())
      {
          int evt = packet->buf_to_int();
          notify_evt(evt);
      }
  }
  //---------------------------------------------------------------------------

    }}//all::xpr
//---------------------------------------------------------------------------