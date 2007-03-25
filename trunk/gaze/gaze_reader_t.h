#ifndef gaze_reader_t_H_INCLUDED
#define gaze_reader_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include <fstream>
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
///
class  gaze_reader_t
{
public:
  gaze_reader_t();
  void load(std::string& binlog);

  size_t nsamples()const {return nsamples_;}

private:
  size_t nsamples_;
  std::fstream gazelog_;
};
//-------------------------------------------------------------------------++
}}//all::gaze
//-------------------------------------------------------------------------++
#endif //gaze_reader_t_H_INCLUDED