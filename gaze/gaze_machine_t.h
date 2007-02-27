#ifndef gaze_machine_t_H_INCLUDED
#define gaze_machine_t_H_INCLUDE
//-------------------------------------------------------------------------++
#include "gaze_machine_inc.h"
//-------------------------------------------------------------------------++
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
//-------------------------------------------------------------------------++
namespace all { namespace gaze{
//-------------------------------------------------------------------------++
///
class  gaze_machine_t
{
public:
  ///
  gaze_machine_t();
  ///
  ~gaze_machine_t();
  ///
  bool start_machine();
  ///
  void print_welcome();
  ///
  void sample_gaze();
  ///
  int nsamples() const;
  ///
  void  start_timing(){timer_.restart();};
  ///
  double elapsed(){return timer_.elapsed();}

private:
  ///
  boost::timer timer_;
    ///Eye Camera
  all::sense::opencv_grabber_ptr    eye_;
  ///
  all::sense::MTi_driver_ptr        mti_;
  ///Scene Camera
  all::sense::bumblebee_sptr        bee_; 

  ///allocate enough space .. 
  void allocate_();
  void write_header_();

  ///Binary Data Stream
  boost::shared_ptr<FILE> binstream_;

  ///eye buffer
  all::core::uint8_sarr   ieye;
  ///rgb scene buffer
  all::core::uint8_sarr   iscene;
  ///xyz depth
  all::core::single_sarr  idepth;
  ///Orientation
  all::math::rpy_angle_t  ihead;
  ///Size ...
  size_t  elapsed_sz  ;
  size_t eye_sz       ;
  size_t scene_sz     ;
  size_t depth_sz     ;
  size_t head_sz     ; 

  ///Samples tag
  int nsamples_;
};
//-------------------------------------------------------------------------++
typedef boost::shared_ptr<gaze_machine_t> gaze_machine_ptr;
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
#endif //gaze_machine_t_H_INCLUDED