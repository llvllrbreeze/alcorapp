#include "grabber_t.hpp"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

int main() {

	sync_data_t sync_data(4);

	grabber_t first_grabber(&sync_data, 1);
	grabber_t second_grabber(&sync_data, 2);
	grabber_t third_grabber(&sync_data, 3);
	grabber_t fourth_grabber(&sync_data, 4);

	boost::thread first_thread(boost::bind(&grabber_t::grab_loop, &first_grabber));
	boost::thread second_thread(boost::bind(&grabber_t::grab_loop, &second_grabber));
	boost::thread third_thread(boost::bind(&grabber_t::grab_loop, &third_grabber));
	boost::thread fourth_thread(boost::bind(&grabber_t::grab_loop, &fourth_grabber));

	for (int i=0; i<10; i++) {
		//grab request
		sync_data.grab_barrier.wait();
	
		//wait that all grabbers retrieve image (internally they sync for grab and retrieve)
		sync_data.retrieve_barrier.wait();

    //get data
    first_grabber.do_op();
    second_grabber.do_op();
    third_grabber.do_op();
    fourth_grabber.do_op();

		//all grabbers return to wait on grab barrier so data access is safe (without mutex)
		printf("main: get data\n");
	}
	getchar();
}
