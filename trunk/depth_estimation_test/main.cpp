#include "alcor/core/depth_image_utils.hpp"
#include "alcor/sense/bumblebee_driver_t.h"

#include "alcor.extern/CImg/CImg.h"

using namespace all;
using namespace cimg_library;


int main()
{
  sense::bumblebee_driver_t bee;

  if (bee.open("config/bumblebeeA.ini"))
  {
    CImgDisplay view(bee.ncols(), bee.nrows(), "depth");
    CImg<core::single_t> dimg;

    while(    view.key != cimg::keyESC 
          && !view.is_closed ) 

    {
      //
      bee.grab();
      //get
      core::single_sarr depth = 
            bee.get_depth_buffer();
      //fill dimg
	    dimg.assign(depth.get() + bee.nrows()*bee.ncols()*2,  
                  bee.ncols(), bee.nrows(),1,1); 
   //   //show
      dimg.display(view);

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
      size_t  hsize =  9;

      core::mystat vstat  
        = core::estimate_depth(depthim, center, hsize);

      //printf("**mean: %f\n", vmean);
    }

    cimg::wait(50);
    }

  }
  return 0;
}