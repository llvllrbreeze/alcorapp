#include <alcor/core/stream_server_t.hpp>
#include "jpeg_source_t.hpp"

int main() {

	jpeg_source_t m_source;

	all::core::stream_server_t server(m_source);
	
	server.run_async();
	getchar();
	server.stop();
	getchar();

}