#include "alcor.apps/gaze/gaze_reader_t.h"


int main(int argc, char* argv[])
{ 
  std::string binlogname;// = "gazelog.bin";

  if (argc>1)
  {
   binlogname  = argv[1];
  }
  else
  {
    binlogname = "gazelog.bin";
  }

  all::gaze::gaze_reader_t gazelog;

  printf("Opening: %s\n", binlogname.c_str());
  gazelog.load(binlogname);

  getchar();
  gazelog.play(true);

  return 0;
}