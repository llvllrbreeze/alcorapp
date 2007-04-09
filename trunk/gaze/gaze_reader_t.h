#ifndef gaze_reader_t_H_INCLUDED
#define gaze_reader_t_H_INCLUDED
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
#include <fstream>
#include "alcor/core/core.h"
#include "alcor/math/rpy_angle_t.h"
#include "alcor/math/size3d_t.hpp"
//-------------------------------------------------------------------------++
#include <boost/lexical_cast.hpp>
//-------------------------------------------------------------------------++
#include "mat.h"
#pragma comment (lib, "libmat.lib")
#pragma comment (lib, "libmx.lib")
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
  struct gaze_sample_t
  {
  //[GAZE DATA]
  ///elapsed time .. in seconds
  double elapsed_;
  ///eye buffer
  all::core::uint8_sarr   eye_;
  ///rgb scene buffer
  all::core::uint8_sarr   scene_;
  ///xyz depth
  all::core::single_sarr  depth_;
  ///Orientation
  all::math::rpy_angle_t  rpy_;
  };
//-------------------------------------------------------------------------++
///
class  gaze_reader_t
{
public:
  ///
  gaze_reader_t();
  ///
  ~gaze_reader_t();
  ///
  void load(std::string& binlog);

  ///to beginning
  void reset();  
  
  ///read a sample from current pos
  bool sample();
  ///advance to next sample
  bool next();
  ///jump at num sample
  bool peek(size_t num);
  ///
  size_t current_sample() const {return current_sample_;}

  ///
  void play(bool bshow = true, bool bsavemat = false);
  ///
  void matlab_dump();
  ///
  size_t nsamples()const {return nsamples_;}
  ///
  double total_elapsed_time() const {return total_elapsed_;}
  ///
  bool eof();

private:
  ///allocate enough space 
  void allocate_();

  ///
  //void play_loop_();

  //[header content]
  ///
  size_t nsamples_;
  ///
  double total_elapsed_;
  ///
  math::size3d_t eyedims_;
  ///
  math::size3d_t scenedims_;
  
  //[byte offsets]
  ///
  size_t elapsed_offset_;
  ///
  size_t eye_offset_;
  ///
  size_t scene_offset_;
  ///
  size_t depth_offset_;
  ///
  size_t rpy_offset_;

  ///depending on previous
  size_t header_offset_;
  size_t per_sample_offset_;

  ///
  std::fstream gazelog_;

  //[GAZE DATA]
  ///elapsed time .. in seconds
  double elapsed_;
  ///eye buffer
  all::core::uint8_sarr   ieye;
  ///rgb scene buffer
  all::core::uint8_sarr   iscene;
  ///xyz depth
  all::core::single_sarr  idepth;
  ///Orientation
  all::math::rpy_angle_t  ihead;

  ///
  size_t current_sample_;
};
//-------------------------------------------------------------------------++
}}//all::gaze
//-------------------------------------------------------------------------++
#endif //gaze_reader_t_H_INCLUDED