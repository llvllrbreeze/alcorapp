#include <alcor/core/stream_server_t.hpp>
#include "alcor/sense/opencv_data_source_t.h"
//[server]
//address=localhost
//port=41170
using namespace all;

int main() {

  sense::opencv_data_source_t m_source;

  //m_source.set_quality(85);

	all::core::stream_server_t server(m_source,"config/stream_server.ini");

	server.run_async();

	server.start_streaming();

	getchar();

	server.stop_streaming();

	server.stop();

	//getchar();

} 