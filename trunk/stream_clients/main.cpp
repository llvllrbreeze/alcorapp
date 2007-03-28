#include <alcor/core/stream_client_t.hpp>

#include "jpeg_dest_t.hpp"

int main() {
	jpeg_dest_ptr first_stream_dest(new jpeg_dest_t(320, 240));
	all::core::stream_client_ptr first_client(new all::core::stream_client_t(first_stream_dest, "config/first_client.ini"));

	first_client->run_async();

	jpeg_dest_ptr second_stream_dest(new jpeg_dest_t(640, 480));
	all::core::stream_client_ptr second_client(new all::core::stream_client_t(second_stream_dest, "config/second_client.ini"));

	second_client->run_async();
	
	getchar();
	
	first_client->stop();
	second_client->stop();
}