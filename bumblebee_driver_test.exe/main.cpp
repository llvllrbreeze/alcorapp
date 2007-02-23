#include "alcor/sense/bumblebee_ipc_recv_t.h"
#include "alcor.extern/CImg/CImg.h"
using namespace all;
using namespace cimg_library;

int main ()
{
  sense::bumblebee_ipc_recv_t beeA;
   if (beeA.open("config/bumblebeeB.ini"))
   {
    //printf("Tout bien!\n");

    printf("Height: %d Width: %d\n",beeA.height(),beeA.width());
    printf("Channels %d\n", beeA.channels() );
    printf("Focal %f\n",beeA.focal());
   }

  CImgDisplay displ(beeA.width(), beeA.height(), "Bumblebee Left");
  CImgDisplay dispr(beeA.width(), beeA.height(), "Bumblebee Right");
  CImg<core::uint8_t> left;
  CImg<core::uint8_t> right;

  while(    dispr.key != cimg::keyESC 
            && displ.key != cimg::keyESC
            && !dispr.is_closed 
            && !displ.is_closed) 
  {

    //beeA.lock();

    core::uint8_sarr imgl =
      beeA.get_color_buffer(core::left_img);
    core::uint8_sarr imgr =
      beeA.get_color_buffer(core::right_img);

    if(!imgr || !imgl) 
      printf("What?\n");
    else
    {    
    left.assign(imgl.get(),   beeA.width(), beeA.height(),1,3);
    right.assign(imgr.get(),  beeA.width(), beeA.height(),1,3);  
    left.display(displ);   
    right.display(dispr);
    }

    //beeA.unlock();
    
    if( dispr.button&1  && dispr.mouse_y >= 0)
    {
      dispr.button&=~1;
      printf(" Left Button clicked ");
      printf("x: %d y: %d\n",dispr.mouse_x, dispr.mouse_y);
    }
    if( dispr.button&2  && dispr.mouse_y >= 0)
    {
      dispr.button&=~2;
      printf(" Right Button clicked");      
      printf("x: %d y: %d\n",dispr.mouse_x, dispr.mouse_y);
    }
    cimg::wait(50);
  }
  
}