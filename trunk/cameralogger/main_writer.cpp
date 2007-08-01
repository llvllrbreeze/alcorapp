#include "alcor/core/camera_logger_t.h"


int main ()
{
  //
  all::core::camera_logger_t 
      camera_logger;
  //
  camera_logger.begin_loop();
  getchar();
  camera_logger.end_loop();

  getchar();
  return 0 ;
}