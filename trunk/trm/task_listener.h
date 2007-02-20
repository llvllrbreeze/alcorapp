#ifndef trm_task_listener_H_INC
#define trm_task_listener_H_INC
//---------------------------------------------------------------------------
#include "alcor/core/server_base_t.hpp"
//---------------------------------------------------------------------------
namespace all { namespace trm {
//---------------------------------------------------------------------------
  class task_listener : public core::server_base_t
  {
  public:
    ///
    task_listener(const std::string& ini_file);

  public:
    ///
    typedef boost::function<void (int)> notify_callback_t;
    ///The actual notify callback
    notify_callback_t notify;
      ///
    void taskset(core::client_connection_ptr_t, core::net_packet_ptr_t);

  };
//---------------------------------------------------------------------------
}}
//---------------------------------------------------------------------------
#endif //trm_task_listener_H_INC