#include <fstream>


int main()
{
  std::string binlogname = "gazelog.bin";


  size_t  thesize;

  ///Binary Data Stream
  std::ifstream gazelog;
  gazelog.open(binlogname.c_str(), std::ios::binary);

  gazelog.read((char*)&thesize, sizeof(size_t));
  printf("nsamples: %d\n", thesize);

  gazelog.read((char*)&thesize, sizeof(size_t));
  printf("elapsed: %d\n", thesize);

  gazelog.read((char*)&thesize, sizeof(size_t));
  printf("eye: %d\n", thesize);

  gazelog.read((char*)&thesize, sizeof(size_t));
  printf("head: %d\n", thesize);


  return 0;
}