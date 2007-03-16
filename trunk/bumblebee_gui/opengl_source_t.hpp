#pragma once
//##########################################################################
#include <alcor/core/jpeg_encoder_t.h>
#include <alcor/core/stream_source_t.hpp>
//##########################################################################
#include <boost/thread/mutex.hpp>
//##########################################################################
namespace all { namespace core {
//##########################################################################
class opengl_source_t : public all::core::stream_source_t 
{
public:
  ///
	opengl_source_t(size_t h, size_t w);
	///
	void set_quality(int);
  ///
  void update_image_buffer(const all::core::uint8_sarr&);
  ///
  void process_command(net_packet_ptr_t) {};
  ///
	int get_data(all::core::uint8_ptr*);
  ///
  boost::mutex  mutex;

private:
  ///

  ///
	all::core::jpeg_encoder_t   m_encoder;
  ///
  all::core::uint8_sarr       m_image;
  ///
	int m_jpeg_quality;
};

//##########################################################################
//##########################################################################
//##########################################################################

inline opengl_source_t::opengl_source_t (size_t h, size_t w) 
{
	m_encoder.reset
    (all::core::rgb_tag, all::core::planar_tag, h, w);

  m_image.reset(new all::core::uint8_t[w*h*3]);
	m_jpeg_quality = 100;
}

///
inline void opengl_source_t::update_image_buffer(const all::core::uint8_sarr& image)
{
  m_image = image;
}

inline void opengl_source_t::set_quality(int quality) {
	m_jpeg_quality = quality;
}

inline int opengl_source_t::get_data(all::core::uint8_ptr* data) 
{
	all::core::jpeg_data_t jpeg_data;

  //{
  boost::mutex::scoped_lock lock (mutex);
	jpeg_data = m_encoder.encode(m_image, m_jpeg_quality);


	size_t buffer_size = jpeg_data.size + sizeof(jpeg_data.crc);

	all::core::uint8_ptr start_of_return_data = new all::core::uint8_t[buffer_size];

  all::core::uint8_ptr write_ptr = start_of_return_data;

	memcpy(write_ptr, &(jpeg_data.crc), sizeof(jpeg_data.crc));

	//
	write_ptr+=sizeof(jpeg_data.crc);

	memcpy(write_ptr, jpeg_data.data.get(), jpeg_data.size);
  //}
	*data = start_of_return_data;

	return buffer_size;
}

}}//all::core