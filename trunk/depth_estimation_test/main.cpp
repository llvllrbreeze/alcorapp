#include "alcor/core/depth_image_utils.hpp"
#include "alcor/sense/bumblebee_driver_t.h"

#include "alcor.extern/CImg/CImg.h"

using namespace all;
using namespace cimg_library;


int main()
{
  sense::bumblebee_driver_t bee;

  if (bee.open("config/bumblebeeB.ini"))
  {
    CImgDisplay view    (bee.ncols(), bee.nrows(), "depth");
    CImgDisplay viewrgb (bee.ncols(), bee.nrows(), "depth");
    CImg<core::single_t>  dimg;
    CImg<core::uint8_t>   rgbimg;

    while(    view.key != cimg::keyESC 
          && !view.is_closed ) 

    {
      //
      bee.grab();
      //get
      core::single_sarr depth = 
            bee.get_depth_buffer();

      core::uint8_sarr rgb =
        bee.get_color_buffer(core::right_img);

      //fill dimg
	    dimg.assign(depth.get() + bee.nrows()*bee.ncols()*2,  
                  bee.ncols(), bee.nrows(),1,1); 

      rgbimg.assign(rgb.get(),  bee.ncols(), bee.nrows(),1,3);

   //   //show
      dimg.display(view);
      rgbimg.display(viewrgb);

    if( view.button&1  && view.mouse_y >= 0)
    {
      view.button&=~1;
      printf(" Left Button clicked ");
      printf("x: %d y: %d\n",view.mouse_x, view.mouse_y);

      //moooolto rischio!! :D
      core::depth_image_t depthim;
      depthim.assign(bee.nrows(), bee.ncols(), depth.get());

      //centro dell'intorno
      core::pixelcoord_t center;
      center.row = view.mouse_y;
      center.col = view.mouse_x;
      //raggio dell'intorno
      size_t  hsize =  7;

      core::mystat vstat  
        = core::estimate_depth(depthim, center, hsize);

    }

    cimg::wait(70);
    }

  }
  return 0;
}