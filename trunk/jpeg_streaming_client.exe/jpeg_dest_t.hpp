#ifndef jpeg_dest_t_H_INCLUDED
#define jpeg_dest_t_H_INCLUDED

#include <alcor/core/jpeg_decoder_t.h>
#include <alcor/core/stream_dest_t.hpp>
#include <alcor.extern/CImg/Cimg.h>


class jpeg_dest_t: public all::core::stream_dest_t {
public:
	jpeg_dest_t();

	virtual void process_data();

private:
	all::core::jpeg_decoder_t m_decoder;
	
	cimg_library::CImg <unsigned char> m_image;
	cimg_library::CImgDisplay m_image_disp;
};

#endif