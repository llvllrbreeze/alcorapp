#include <alcor/core/stream_server_t.hpp>
#include "alcor/sense/opencv_data_source_t.h"
//[server]
//address=localhost
//port=41170
using namespace all;

int main() {

  boost::shared_ptr<sense::opencv_data_source_t> m_source_ptr(new sense::opencv_data_source_t);

  m_source_ptr->set_quality(90);

	all::core::stream_server_t server(m_source_ptr,"config/trm_stream_server.ini");

	server.run_async();

	server.start_streaming();

	getchar();

	server.stop_streaming();

	server.stop();

	//getchar();

} 