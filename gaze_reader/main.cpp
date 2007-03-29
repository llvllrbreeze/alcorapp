#include "alcor.apps/gaze/gaze_reader_t.h"


int main()
{
  std::string binlogname = "gazelog.bin";
  all::gaze::gaze_reader_t gazelog;

  gazelog.load(binlogname);

  getchar();
  gazelog.play(false);
  getchar();

  //size_t  thesize;
  //double  elapsed;

  /////Binary Data Stream
  //std::fstream gazelog;
  //gazelog.open(binlogname.c_str(), std::ios::in|std::ios::binary);

  //gazelog.read((char*)&thesize, sizeof(size_t));
  //printf("nsamples: %d\n", thesize);

  //gazelog.read((char*)&elapsed, sizeof(elapsed));
  //printf("total time: %f\n", elapsed);

  //gazelog.read((char*)&thesize, sizeof(size_t));
  //printf("elapsed offset: %d\n", thesize);

  //gazelog.read((char*)&thesize, sizeof(size_t));
  //printf("eye offset: %d\n", thesize);

  //gazelog.read((char*)&thesize, sizeof(size_t));
  //printf("head offset: %d\n", thesize);


  return 0;
}