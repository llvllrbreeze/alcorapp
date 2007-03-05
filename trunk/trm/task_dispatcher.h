#ifndef trm_task_dispatcher_H_INC
#define trm_task_dispatcher_H_INC
//---------------------------------------------------------------------------
#include "trm_tags_inc.h"
//---------------------------------------------------------------------------
#include "alcor/core/client_base_t.hpp"
//---------------------------------------------------------------------------
namespace all { namespace trm {
        ///
        class task_dispatcher : public all::core::client_base_t
        {
        public:
            ///
            task_dispatcher(const std::string& ini);
            ///
            void send_event(int);

        private:
            ///
            void connected_cb();
            ///
            void updatetask(core::net_packet_ptr_t);
        };
//---------------------------------------------------------------------------
        typedef boost::shared_ptr<task_dispatcher> task_dispatcher_sptr;
//---------------------------------------------------------------------------
    }}
//---------------------------------------------------------------------------
#endif //trm_task_dispatcher_H_INC