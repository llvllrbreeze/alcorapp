#pragma once

#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>

struct sync_data_t {
	sync_data_t(int count) : grab_barrier(count+1), query_barrier(count), retrieve_barrier(count+1) {}
	boost::barrier grab_barrier;
	boost::barrier query_barrier;
	boost::barrier retrieve_barrier;
};

class grabber_t {

public:
	grabber_t(sync_data_t*, int);

	void grab_loop();

private:
	sync_data_t* _sync_data;
	volatile bool _running;
	int _n;
};


