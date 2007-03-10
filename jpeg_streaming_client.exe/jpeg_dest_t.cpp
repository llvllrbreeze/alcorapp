#include "jpeg_dest_t.hpp"

jpeg_dest_t::jpeg_dest_t() : m_image(), m_image_disp(800, 600, "stream")
{
}

void jpeg_dest_t::process_data() {
	
	boost::crc_32_type::value_type enc_crc;
	int enc_data_size = m_data_size - sizeof(boost::crc_32_type::value_type);

	printf("data size: %i\n", enc_data_size);

	all::core::uint8_sarr enc_data(new all::core::uint8_t[enc_data_size]);
	
	all::core::jpeg_data_t image;

	all::core::uint8_ptr read_ptr = m_data.get();

	memcpy(&enc_crc, read_ptr, sizeof(enc_crc));
	//printf("CRC: %x\n", enc_crc);

	read_ptr+= sizeof(enc_crc);

	memcpy(enc_data.get(), read_ptr, enc_data_size);

	//printf("decompressing...\n");
	if (m_decoder.decode(image, enc_data, enc_data_size, enc_crc)) 
  {
    //printf("decompress ok: %d %d\n", image.width, image.height);
		m_image.assign(image.data.get(), image.width, image.height, 1, image.depth);
		m_image_disp.display(m_image);
	}
	else {
		printf("decompress failed\n");
	}
}