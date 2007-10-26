#ifndef gaze_jpeg_encoder_t_HPP_INCLUDED
#define gaze_jpeg_encoder_t_HPP_INCLUDED

#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/bind.hpp>
#include <alcor/core/jpeg_encoder_t.h>

namespace all {
	namespace core {

struct enc_sync_data_t {
	enc_sync_data_t (int count) : enc_barrier(count + 1), get_barrier(count + 1) {}

	boost::barrier enc_barrier;
	boost::barrier get_barrier;
};

class gaze_jpeg_encoder_t {

public:
	gaze_jpeg_encoder_t( all::core::uint8_sarr, all::core::jpeg_data_t*, bool, int, int); 

	void encode_loop_(bool, int, int);

	static void stop();

	static void start(all::core::enc_sync_data_t*);

	static void encode();


private:

	jpeg_encoder_ptr_t _encoder;
	static enc_sync_data_t* _sync_data;
	static volatile bool _running;

	all::core::jpeg_data_t* _jpeg_data;
	all::core::uint8_sarr _to_encode;

	boost::shared_ptr<boost::thread> _loop_thread;
};

}}

#endif