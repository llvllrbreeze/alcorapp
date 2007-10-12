#include "alcor/core/cameralog_reader_t.h"

int main (int argc, char* argv[])
{
  //
  bool b_savelog = false;

  //
  if(argc > 1)
      b_savelog = ( strcmp("save", argv[1]) == 0);
  

  //
  all::core::cameralog_reader_t cameralog(b_savelog);
  //
  printf("Starting  Loop.......\n");
  cameralog.begin_loop();

  getchar();

  return 0;
}
