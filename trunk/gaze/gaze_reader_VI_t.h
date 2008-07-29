#ifndef gaze_reader_VI_t_H_INCLUDED
#define gaze_reader_VI_t_H_INCLUDED
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
#include <fstream>
#include "alcor/core/core.h"
#include "alcor/math/rpy_angle_t.h"
#include "alcor/math/size3d_t.hpp"
#include "alcor/core/jpeg_decoder_t.h"
//#include "alcor/sense/stereo_process_t.hpp"
//-------------------------------------------------------------------------++
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
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
  all::core::uint8_sarr   eye_[2];
  ///rgb scene buffer
  all::core::uint8_sarr   scene_[2];
  ///xyz depth
  all::core::single_sarr  depth_;
  ///Orientation
  all::math::rpy_angle_t  rpy_;
  };
//-------------------------------------------------------------------------++
///
class  gaze_reader_VI_t
{
public:
  ///
  gaze_reader_VI_t();
  ///
  ~gaze_reader_VI_t();
  ///
  void load(std::string& binlog);

  ///to beginning
  void reset();  
  
  ///read a sample from current pos
  boost::function <bool (void)> sample;
  
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

  ///left or right
  enum
  {
    left  = 0,
    right = 1,
  } e_source;

private:
  ///allocate enough space 
  void allocate_();

  bool sample_bin_();
  bool sample_avi_();
  ///
  //void play_loop_();

  //[header content]
  ///
  size_t nsamples_;
  ///
  double total_elapsed_;

  //avi or bin log
  int log_type_;
  ///
  math::size3d_t eyedims_[2];
  ///
  math::size3d_t scenedims_[2];
  
  //[byte offsets]
  ///
  size_t elapsed_offset_;
  ///
  size_t eye_offset_[2];
  ///
  size_t scene_offset_[2];
  ///
  size_t depth_offset_;
  
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
  all::core::uint8_sarr   ieye[2];
  ///rgb scene buffer
  all::core::uint8_sarr   iscene[2];
  ///xyz depth
  all::core::single_sarr  idepth;
  //Orientation
  all::math::rpy_angle_t  ihead;

  ///
  size_t current_sample_;

  ///jpeg decoder
  all::core::jpeg_decoder_t jpeg_dec_;

  all::core::jpeg_data_t enc_eye_data_[2];
  all::core::jpeg_data_t enc_scene_data_[2];

  all::core::jpeg_data_t dec_eye_data_[2];
  all::core::jpeg_data_t dec_scene_data_[2];

  //avi reader
  CvCapture* eye_avi_[2];
  CvCapture* scene_avi_[2];

  IplImage* eye_img_[2];
  IplImage* eye_bw_img_[2];
  IplImage* scene_img_[2];

  //stereo process
  //all::sense::stereo_process_t stereo_process;

  //CvCalibFilter _eye_filter[2];

  

};
//-------------------------------------------------------------------------++
}}//all::gaze
//-------------------------------------------------------------------------++
#endif //gaze_reader_VI_t_H_INCLUDED