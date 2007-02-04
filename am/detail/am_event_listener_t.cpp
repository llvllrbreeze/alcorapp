#include "..\am_event_listener_t.h"
#include "alcor\core\config_parser_t.h"
//--------------------------------------------------------------------------+
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
//--------------------------------------------------------------------------+
#include "..\attention_events_t.h"
#include "alcor\core\core.h"
#include "alcor\core\i_connection_handler_t.h"
//###########################################################################
namespace all { namespace am { namespace detail {
//###########################################################################
///
  struct am_event_listener_imp  
    : public all::am::am_event_listener_t
    ,public all::core::i_service_handler
  {
  ///
  am_event_listener_imp();
  ///
  //void start();
  ///
  void recv_task_imp( ArNetPacket* pkt);
  ///
  events::tag get_event() const;
  ///
  attention_events_t evt;
  ///
	void register_to();
  ///
	void lost_connection();
  ///
  //boost::thread conn_thr;
  ///
  all::core::i_connection_handler_t connection_handler;
  ///
	ArFunctor1C<am_event_listener_imp, ArNetPacket* > 
    m_task_recv_cb;
  };
//###########################################################################
  //ctor
am_event_listener_imp::am_event_listener_imp(): 
      			connection_handler(this,("config/am_service.ini"))
      ,m_task_recv_cb(this, &am_event_listener_imp::recv_task_imp)
{

}
//###########################################################################
void am_event_listener_imp::recv_task_imp( ArNetPacket* pkt)
{
	evt.import(pkt);
	notify();
}
//###########################################################################
//void am_event_listener_imp::start() 
//{
//
//}
//###########################################################################
void am_event_listener_imp::register_to()
{
	connection_handler.m_client.addHandler("dispatch", &m_task_recv_cb);
	connection_handler.m_client.request("dispatch",500);
}
//###########################################################################
void am_event_listener_imp::lost_connection()
{
  evt.event_tag = events::RESET;
	cout << endl << "RESETTING::Server has disconnected .. " <<  endl;
  notify();
}
//###########################################################################
events::tag am_event_listener_imp::get_event() const
{
  return evt.event_tag;
}
//###########################################################################
}}}//namespaces
//###########################################################################
//-------------------------------------------------------------------------++
namespace all { namespace am {
//-------------------------------------------------------------------------++
///
  std::auto_ptr<am_event_listener_t> 
    create_listener()
{
  std::auto_ptr<am_event_listener_t > plistener 
      (
      reinterpret_cast<am_event_listener_t*>
      (new all::am::detail::am_event_listener_imp)
      );
    return plistener;
}
//-------------------------------------------------------------------------++
}}//namespaces
//###########################################################################
