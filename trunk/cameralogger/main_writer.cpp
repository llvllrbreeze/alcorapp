#include "alcor/core/camera_logger_t.h"


int main ()
{
  //
  all::core::camera_logger_t 
    camera_logger(e_iplimage);
  //
  camera_logger.begin_loop();
  getchar();
  camera_logger.quit_loop();

  getchar();
  return 0 ;
}