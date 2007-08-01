#include "alcor/sense/opencv_grabber_t.h"
#include "alcor.extern/CImg/CImg.h"


#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <iostream>

using namespace all;
using namespace cimg_library;

using namespace boost::gregorian;
using namespace boost::posix_time;

int main ()
{
  date today = day_clock::local_day();
  ptime now = second_clock::local_time();

  std::cout << "Today is: " << today  <<std::endl;
  std::cout << "Time: "     << now.time_of_day().hours()  
    << ":"
                            << now.time_of_day().minutes()
                            << ":"
                            << now.time_of_day().seconds()
                            <<std::endl;

  all::sense::opencv_grabber_t cam;
  cam.open("config/opencvcam.ini");

  CImgDisplay view ( cam.width(), cam.height(), "Camera");
  CImg<core::uint8_t> imag;
  
  core::uint8_sarr imag_sptr
    ( new core::uint8_t[cam.width()*cam.height()*cam.channels()] );

  //int code = 0;

  while(    view.key != cimg::keyESC 
        && !view.is_closed )
    {

    if (cam.get_color_buffer(imag_sptr) )
    {
      imag.assign( imag_sptr.get(),  cam.width(), cam.height(), 1,cam.channels());
      imag.display(view) ; 
    }

        cimg::wait(40);
  }
  //if( code == 27 || code == 'q' || code == 'Q' )
  //    break;
  //}


  return 0;
}