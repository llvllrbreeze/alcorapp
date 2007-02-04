#include "..\am_dispatcher_t.h"
#include "alcor\core\config_parser_t.h"
//--------------------------------------------------------------------------+
#include "..\attention_events_t.h"
#include "alcor\core\core.h"
//###########################################################################
#include <Aria.h>
#include <ArNetworking.h>
//###########################################################################
namespace all { namespace am { namespace detail {
//###########################################################################
  struct  am_dispatcher_imp : public am::am_dispatcher_t
  {
    ///
    am_dispatcher_imp();
    ///
	  void run_server();
    ///
	  void stop_server();
    ///
    void dispatch_event(events::tag);
    ///
    all::core::ip_address_t m_service_address;
    ///
    ArServerBase m_server;
    ///
    void dispatch_imp(ArServerClient*, ArNetPacket*){};
    ///
	  ArFunctor2C<am_dispatcher_imp, ArServerClient*, ArNetPacket*> 
          m_dispatch_cb;
  };
//###########################################################################
  //IMPL
//###########################################################################
  am_dispatcher_imp::am_dispatcher_imp():
			m_dispatch_cb(this, &am_dispatcher_imp::dispatch_imp)
  {
    printf ("am dispatcher: Loading configuration file\n");
    core::config_parser_t config;
    config.load(core::ini, "config/am_service.ini");

    m_service_address.hostname = "localhost";
    m_service_address.port = config.as_int("server.port",4456);

    Aria::init();

  	if (!m_server.open(m_service_address.port))
		{
			printf("Could not open server port: %d\n", m_service_address.port );
			exit(1);
		}
	  //[Assign Callbacks]
	  m_server.addData("dispatch", "Task Dispatcher", 
						  &m_dispatch_cb, "none", "none");
  }
//###########################################################################
//
void am_dispatcher_imp::run_server()
{
	m_server.runAsync();
}    
///
void am_dispatcher_imp::stop_server()
{
  m_server.stopRunning();
}
//###########################################################################
///
void am_dispatcher_imp::dispatch_event(events::tag thetag)
{
	printf("Broadcasting Task Message\n");
	//Build the packet and send it back to the client
  attention_events_t evt(thetag);
	ArNetPacket pkt;
  evt.pack(&pkt);
	m_server.broadcastPacketTcp(&pkt,"dispatch");
}
//###########################################################################
}}}//all::am::detail
//###########################################################################

//-------------------------------------------------------------------------++
namespace all { namespace am {
//-------------------------------------------------------------------------++
///
boost::shared_ptr<am_dispatcher_t> 
    create_dispatcher()
{
    boost::shared_ptr<am_dispatcher_t> pdispatcher 
      (reinterpret_cast<all::am::am_dispatcher_t*>
      (new all::am::detail::am_dispatcher_imp));
    return pdispatcher;
}
//-------------------------------------------------------------------------++
}}