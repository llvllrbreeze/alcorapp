#include <alcor/core/stream_client_t.hpp>

#include "jpeg_dest_t.hpp"

int main() {
	//cimg_dest_t stream_dest;
	jpeg_dest_t stream_dest;
	all::core::stream_client_t client(stream_dest);
	client.run_async();
	getchar();
	client.start_receive();
	getchar();
	client.stop_receive();
	getchar();
}
