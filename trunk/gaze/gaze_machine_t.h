#ifndef gaze_machine_t_H_INCLUDED
#define gaze_machine_t_H_INCLUDE
//-------------------------------------------------------------------------++
#include "gaze_machine_inc.h"
//-------------------------------------------------------------------------++
namespace all { namespace gaze{
//-------------------------------------------------------------------------++
///
class  gaze_machine_t
{
public:
  ///ctor
  gaze_machine_t();

  ///
  ~gaze_machine_t();

  ///
  int nsamples() const;
  ///
  void  start_timing(){timer_.restart();};
  ///
  double elapsed(){return timer_.elapsed();}
  //specific device handlinf
  ///reset mti.
  void reset_mti();

  ///
  void run_machine();
  ///
  void cancel(){running_=false;};

  ///
  void log_enabled(bool enabled = true);

private:    
  ///
  void print_welcome();
  ///
  bool boot_machine_();  
  ///
  void sample_gaze_(); 
  ///
  void write_gaze_();
  ///
  void show_gaze_();
  ///
  boost::function<void (void)> process_gaze_data;

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

  ///sampling loop
  void gaze_loop();

  ///Binary Data Stream
  std::fstream gazelog_;

  ///eye buffer
  all::core::uint8_sarr   ieye;
  ///rgb scene buffer
  all::core::uint8_sarr   iscene;
  ///xyz depth
  all::core::single_sarr  idepth;
  ///Orientation
  all::math::rpy_angle_t  ihead;

  ///Size ...
  size_t  elapsed_sz   ;
  size_t  eye_sz       ;
  size_t  scene_sz     ;
  size_t  depth_sz     ;
  size_t  head_sz      ; 

  ///Samples num
  size_t nsamples_;
  ///elapsed time .. in seconds
  double elapsed_;
  ///loop control
  volatile bool running_;
  ///
  //volatile bool enabled_;
  //
  unsigned int msecspause;
  ///
  boost::shared_ptr<boost::thread> thread_ptr;
};
//-------------------------------------------------------------------------++
typedef boost::shared_ptr<gaze_machine_t> gaze_machine_ptr;
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
#endif //gaze_machine_t_H_INCLUDED