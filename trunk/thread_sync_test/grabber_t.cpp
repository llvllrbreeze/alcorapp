#include "grabber_t.hpp"
#include "alcor/core/core.h"

grabber_t::grabber_t(sync_data_t* sync_data, int n) {
	_sync_data = sync_data;
	_running = true;
	_n = n;
}

void grabber_t::do_op()
{
  printf("getting data from %d\n", _n);
  all::core::BOOST_SLEEP(10*_n);
}


void grabber_t::grab_loop() {
	
	printf("%i: init grabbing\n", _n);

	while (_running) {

		//wait that all grabbers are ready to grab and that main asks to grab
		_sync_data->grab_barrier.wait();

		printf("%i: query frame\n", _n);

    //
    all::core::BOOST_SLEEP(10*_n);
		//wait for all grabbers query
		//_sync_data->query_barrier.wait();

		printf("%i: retrieve frame\n", _n);

		//wait on this so main knows when all data is ready
		_sync_data->retrieve_barrier.wait();

	}

}