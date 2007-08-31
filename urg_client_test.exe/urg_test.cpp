#include <alcor/sense/urg_client_t.hpp>

void scan_ready_cb(all::sense::urg_scan_data_ptr scan_data) {
	printf("scan ready\n");
}

int main() {
	all::sense::urg_client_t urg_client;

	urg_client.run_async();

	getchar();

	urg_client.set_laser_on();

	getchar();
	
	urg_client.set_scan_ready_cb(&scan_ready_cb);
	urg_client.start_continous_scan(1, 768, 1, 0);

	getchar();

	urg_client.stop_continous_scan();

	getchar();

	urg_client.stop();

	getchar();


}