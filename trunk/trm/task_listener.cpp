#include "task_listener.h"
#include "alcor/core/iniWrapper.h"
//---------------------------------------------------------------------------
namespace all { namespace trm {
        //---------------------------------------------------------------------------
        task_listener::task_listener(const std::string& ini_file)
        {
            iniWrapper ini;
            if ( ini.Load(ini_file.c_str()) )
            {
                int port = ini.GetInt("server:port", 99999);
                set_port(port);
            }

            add_command_handler
            ("taskset", boost::bind(&task_listener::taskset, this, _1, _2));
        }
        //---------------------------------------------------------------------------
        ///
        void task_listener::taskset(core::client_connection_ptr_t, core::net_packet_ptr_t packet)
        {
            if(!notify.empty())
            {
                int evt = packet->buf_to_int();
                notify(evt);
            }
        }
        //---------------------------------------------------------------------------

        //---------------------------------------------------------------------------

    }}
//---------------------------------------------------------------------------