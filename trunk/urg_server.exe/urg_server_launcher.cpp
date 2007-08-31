#include <alcor/sense/urg_server_t.hpp>

int main() {

	all::sense::urg_server_t urg_server;

	urg_server.run_async();

	getchar();

	urg_server.stop();
}