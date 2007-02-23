#include "alcor/sense/opencv_grabber_t.h"
#include "alcor.extern/CImg/CImg.h"

using namespace all;
using namespace cimg_library;

int main ()
{
  all::sense::opencv_grabber_t cam;
  cam.open(core::open_camera, 0);

  //getchar();

  CImgDisplay view ( cam.width(), cam.height(), "Camera");
  CImg<core::uint8_t> imag;
  //cvNamedWindow( "Ocv");
  
  core::uint8_sarr imag_sptr
    ( new core::uint8_t[cam.width()*cam.height()*cam.channels()] );

  //int code = 0;

  while(    view.key != cimg::keyESC 
        && !view.is_closed )
    {

    if (cam.get_color_buffer(imag_sptr) )
    {
      imag.assign( imag_sptr.get(),  cam.width(), cam.height(), 1,3);
      imag.display(view); 
    }

        cimg::wait(40);
  }
  //if( code == 27 || code == 'q' || code == 'Q' )
  //    break;
  //}


  return 0;
}