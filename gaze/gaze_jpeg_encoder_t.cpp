#include "gaze_jpeg_encoder_t.hpp"

namespace all {
	namespace core {


volatile bool gaze_jpeg_encoder_t::_running = false;
all::core::enc_sync_data_t* gaze_jpeg_encoder_t::_sync_data = NULL;

gaze_jpeg_encoder_t::gaze_jpeg_encoder_t(all::core::uint8_sarr to_encode, all::core::jpeg_data_t* jpeg_data, bool gray, int width, int height) {
	_jpeg_data = jpeg_data;
	_to_encode = to_encode;
	//_sync_data = sync_data;

	_loop_thread.reset(new boost::thread(boost::bind(&gaze_jpeg_encoder_t::encode_loop_, this, gray, width, height)));
	//encode_loop_(gray, width, height);
}

void gaze_jpeg_encoder_t::encode_loop_(bool gray, int w, int h) {

	_encoder.reset(new all::core::jpeg_encoder_t());

	if (gray)
		_encoder->reset(all::core::gray_tag, w, h);
	else
		_encoder->reset(all::core::rgb_tag, all::core::interleaved_tag, w, h);

	while(gaze_jpeg_encoder_t::_running) {

		gaze_jpeg_encoder_t::_sync_data->enc_barrier.wait();

		*_jpeg_data = _encoder->encode(_to_encode, 75);

		gaze_jpeg_encoder_t::_sync_data->get_barrier.wait();
	}

}

void gaze_jpeg_encoder_t::start(all::core::enc_sync_data_t* sync_data) {
	_running = true;
	_sync_data = sync_data;
}

void gaze_jpeg_encoder_t::stop() {
	_running = false;
	_sync_data->enc_barrier.wait();
	_sync_data->get_barrier.wait();
}

void gaze_jpeg_encoder_t::encode() {
	_sync_data->enc_barrier.wait();
	_sync_data->get_barrier.wait();
}

}}