#ifndef jpeg_source_t_H_INCLUDED
#define jpeg_source_t_H_INCLUDED

//#include <alcor/core/jpeg_encoder_t.h>
//#include <alcor/core/stream_source_t.hpp>
//#include <alcor/sense/bumblebee_driver_t.h>
//
//
//class jpeg_source_t : public all::core::stream_source_t {
//public:	
//	jpeg_source_t();
//	
//	void set_quality(int);
//
//	virtual int get_data(all::core::uint8_ptr*);
//
//private:
//	all::core::jpeg_encoder_t m_encoder;
//	all::sense::bumblebee_driver_t m_bee;
//
//	int m_jpeg_quality;
//};

#include <alcor/core/jpeg_encoder_t.h>
#include <alcor/core/stream_source_t.hpp>
#include <alcor/sense/bumblebee_ipc_recv_t.h>


class jpeg_source_t : public all::core::stream_source_t {
public:	
	jpeg_source_t();
	
	void set_quality(int);

	virtual int get_data(all::core::uint8_ptr*);

private:
	all::core::jpeg_encoder_t m_encoder;
	all::sense::bumblebee_ipc_recv_t m_bee;

	int m_jpeg_quality;
};

#endif