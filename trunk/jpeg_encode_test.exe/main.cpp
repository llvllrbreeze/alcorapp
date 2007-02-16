#include "alcor/core/jpeg_encoder_t.h"
#include "alcor/core/jpeg_decoder_t.h"
#include "alcor.extern/CImg/CImg.h"
#include "alcor/sense/bumblebee_ipc_recv_t.h"

#include <boost/timer.hpp>
using namespace cimg_library;
using namespace all;

int main ()
{
  //codec objects
  core::jpeg_encoder_t encoder;
  core::jpeg_decoder_t decoder;
  
  //structs
  core::jpeg_data_t jpegenc;
  core::jpeg_data_t jpegdec;

  boost::timer timer;
  double elapsed;

  core::uint8_sarr temp;

  sense::bumblebee_ipc_recv_t bee;
  
  if( bee.open("config/bumblebeeA.ini") )
  {
     //view-related stuff
    CImgDisplay view(bee.width(), bee.height(),"Bumblebee Right Image");   
    CImgDisplay viewdec(bee.width(), bee.height(),"Decoded");

    //retrieve frame from shared-memory
    core::uint8_sarr img = bee.get_color_buffer(core::right_img);

    //original image
    CImg<unsigned char> cimage; 
    //decoded image
    CImg<unsigned char> decimg; 
    //fill original
    cimage.assign(img.get(),bee.width(), bee.height(),1,3);
    //show it
    cimage.display(view);

    //CImg
    printf("Compression!\n");  
    //set parameters for compression
    encoder.reset(core::rgb_tag, core::planar_tag, bee.height(), bee.width());

    timer.restart();
      jpegenc = encoder.encode(img, 100);
    elapsed  = timer.elapsed();
    printf("Compressed size: %d\n", jpegenc.size);
    printf("Elapsed: %f\n\n", elapsed);

    //CORRUPT HEADER!!!!
    //decomment to test decoder error triggering
    //memset(jpegenc.data.get()+16, 123, 8);
    core::uint8_sarr rotto(new core::uint8_t[jpegenc.size-1024]);

    memcpy(rotto.get(),jpegenc.data.get(),jpegenc.size-1024);

    printf("Decompress\n");
    timer.restart();
    if( decoder.decode(jpegdec ,  rotto, jpegenc.size-1024) )
    {
    elapsed  = timer.elapsed();
    printf("Decoded succesfully! size: %d %d %d\n", 
        jpegdec.height, jpegdec.width, jpegdec.depth);
    printf("Elapsed: %f\n\n", elapsed);


    decimg.assign(jpegdec.data.get(),bee.width(), bee.height(),1,3);
    decimg.display(viewdec );
    }
    else
    {
      printf("DECODER ERROR!\n");
    }

  ////////////////////////////////////////////////
  ////////////////////////////////////////////////

    //encoder.reset(core::gray_tag, 240,320);

    //temp.reset(new core::uint8_t[240*320]);


    //timer.restart();
    //  jpegenc = encoder.encode(temp, 100);
    //elapsed  = timer.elapsed();
    //printf("Compressed size: %d\n", jpeg.size);
    //printf("Elapsed: %f\n\n", elapsed);

    //printf("Decompress\n");

    //timer.restart();  
    //bool dec_ok =   decoder.decode(jpegdec, jpeg.data, jpeg.size);
    //elapsed  = timer.elapsed();

    //if(dec_ok)
    //{
    //printf("Decoded succesfully! size: %d %d %d\n", 
    //  jpegdec.height, jpegdec.width, jpegdec.depth);
    //printf("Elapsed: %f\n\n", elapsed);
    //}
    //else
    //{
    //  printf("DECODER ERROR!\n");
    //}
  getchar();
  }

  else
  {
    printf("Cannot open camera!\n");
  }

}