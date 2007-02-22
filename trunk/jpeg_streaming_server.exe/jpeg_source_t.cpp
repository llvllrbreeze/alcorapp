#include "jpeg_source_t.hpp"

//jpeg_source_t::jpeg_source_t() {
//	m_bee.open("config/bumblebeeB.ini");
//
//	m_encoder.reset(all::core::rgb_tag, all::core::planar_tag, m_bee.ncols(), m_bee.nrows());
//
//	m_jpeg_quality = 100;
//}
//
//
//void jpeg_source_t::set_quality(int quality) {
//	m_jpeg_quality = quality;
//}
//
//int jpeg_source_t::get_data(all::core::uint8_ptr* data) {
//	printf("start get data\n");
//	printf("grabbing color image\n");
//	m_bee.grab();
//	all::core::uint8_sarr image = m_bee.get_color_buffer(all::core::left_img, true);
//	printf("encoding image\n");
//	all::core::jpeg_data_t jpeg_data = m_encoder.encode(image, m_jpeg_quality);
//	printf("copying image\n");
//	all::core::uint8_ptr return_data = new all::core::uint8_t[jpeg_data.size];
//	memcpy(return_data, jpeg_data.data.get(), jpeg_data.size);
//
//	*data = return_data;
//
//	return jpeg_data.size;	
//}

jpeg_source_t::jpeg_source_t() {
	m_bee.open("config/bumblebeeA.ini");

	m_encoder.reset(all::core::rgb_tag, all::core::planar_tag, m_bee.height(), m_bee.width());

	m_jpeg_quality = 100;
}


void jpeg_source_t::set_quality(int quality) {
	m_jpeg_quality = quality;
}

int jpeg_source_t::get_data(all::core::uint8_ptr* data) {
	//printf("start get data\n");
	//m_bee.lock();
	//printf("grabbing color image\n");
	all::core::jpeg_data_t jpeg_data;
	all::core::uint8_sarr image = m_bee.get_color_buffer(all::core::right_img, true);
	//printf("encoding image\n");
	
	jpeg_data = m_encoder.encode(image, m_jpeg_quality);
	//m_bee.unlock();
	//printf("copying image\n");
	printf("CRC: %x\n", jpeg_data.crc);
	printf("size: %i\n", jpeg_data.size);

	size_t buffer_size = jpeg_data.size + sizeof(jpeg_data.crc);

	all::core::uint8_ptr return_data = new all::core::uint8_t[buffer_size];

	all::core::uint8_ptr write_ptr = return_data;

	memcpy(write_ptr, &(jpeg_data.crc), sizeof(jpeg_data.crc));
	
	write_ptr+=sizeof(jpeg_data.crc);

	memcpy(write_ptr, jpeg_data.data.get(), jpeg_data.size);

	*data = return_data;

	return buffer_size;
}