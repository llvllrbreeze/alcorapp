#ifndef trm_task_listener_H_INC
#define trm_task_listener_H_INC
//---------------------------------------------------------------------------
#include "alcor/core/server_base_t.hpp"
//---------------------------------------------------------------------------
#include "trm_tags_inc.h"
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
    ///
    typedef boost::function<void (int, int, int, int, int)> notify_setup_t;
    ///
    typedef boost::function<void (void)> on_disconnect_cb_t;

    ///The actual notify callback
    notify_callback_t   notify_evt;
    ///
    notify_setup_t      notify_roi;
    ///
    on_disconnect_cb_t  on_disconnect;

    ///
    void taskset(core::client_connection_ptr_t, core::net_packet_ptr_t);
    ///
    void setup_roi(core::client_connection_ptr_t, core::net_packet_ptr_t);

  private:
    ///
    void on_disconnect_internal(int nclients);

  };
//---------------------------------------------------------------------------
}}
//---------------------------------------------------------------------------
#endif //trm_task_listener_H_INC