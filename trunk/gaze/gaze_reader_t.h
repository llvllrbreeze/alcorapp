#ifndef gaze_reader_t_H_INCLUDED
#define gaze_reader_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include <fstream>
#include "alcor/core/core.h"
#include "alcor/math/rpy_angle_t.h"
#include "alcor/math/size3d_t.hpp"
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
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
  ///
  void next();
  ///
  void play();
  ///
  size_t nsamples()const {return nsamples_;}

private:
  ///allocate enough space 
  void allocate_();

  ///read a sample from current pos
  void sample_();

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