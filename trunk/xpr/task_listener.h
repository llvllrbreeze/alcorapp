#pragma once 

#ifndef trm_task_listener_H_INC
#define trm_task_listener_H_INC
//---------------------------------------------------------------------------
#include "alcor/core/server_base_t.hpp"
//---------------------------------------------------------------------------
#include "xpr_tags_inc.h"
//---------------------------------------------------------------------------
namespace all { namespace xpr {
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
    notify_callback_t       notify_evt;

    ///
    void taskset(core::client_connection_ptr_t, core::net_packet_ptr_t);

  };
//---------------------------------------------------------------------------
typedef boost::shared_ptr< task_listener > task_listener_sptr;
//---------------------------------------------------------------------------
}}
//---------------------------------------------------------------------------
#endif //trm_task_listener_H_INC
