#ifndef gaze_machine_2_t_H_INCLUDED
#define gaze_machine_2_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include "gaze_machine2_inc.h"
#include "alcor/math/size3d_t.hpp"
//-------------------------------------------------------------------------++
namespace all { namespace gaze{
//-------------------------------------------------------------------------++
///
class  gaze_machine2_t
{
public:
  ///ctor
  gaze_machine2_t();

  ///
  ~gaze_machine2_t();

  ///
  int nsamples() const;
  ///
  void  start_timing(){timer_.restart();};
  ///
  double elapsed(){return timer_.elapsed();}

  //specific device handling
  ///reset mti.
  void reset_mti();

  ///
  void run_gaze_machine();

  ///
  void run_machine(calib_t const&);
  ///
  void run_machine(binlog_t const&);
  ///
  void run_machine(show_t const&);

  ///invoke this to stop the thread
  void cancel(){running_=false;};

  ///
  void enable_views(bool b_enable=true)
      {b_enabled_views_ = b_enable;}

  ///
  void set_logname(std::string&);
  ///
  void sample_gaze_(); 
  ///
  void save_calib(){bsavecalib_ = true;};

  ///left or right
  enum
  {
    left  = 0,
    right = 1
  } e_source;

private:    
  ///
  void print_welcome();
  ///
  void reset_devices_();
  ///
  bool boot_machine_();  

  /////////////////////////////////////////////////
  //routines
  ///
  void write_gaze_();
  ///unuseful
  void null_op_();

  ///calib loop
  void calib_loop();

  ///sampling loop
  void gaze_loop();

  ///sampling loop
  void show_loop();
  /////////////////////////////////////////////////
  ///
  boost::function<void (void)> f_process_gaze_data;

  ///
  boost::timer timer_;
    ///Eye Camera
  all::sense::opencv_grabber_ptr    eye_[2];
    ///Scene Camera
  all::sense::opencv_grabber_ptr    scene_[2];

  ///
  all::sense::MTi_driver_ptr        mti_;

  //all::sense::bumblebee_sptr        bee_; 

  ///
  bool is_opened_;

  ///allocate enough space .. 
  void allocate_();
  void get_dims_();
  void write_header_();

  ///generic gaze_loop
  boost::function<void (void)> gaze_loop_;

  ///Binary Data Stream
  std::fstream gazelog_;
  ///
  std::string logname_;

  //[GAZE DATA]
  ///elapsed time .. in seconds
  double elapsed_;

  /////eye buffer
  //all::core::uint8_sarr   eye_img_[2];
  /////rgb scene buffer
  //all::core::uint8_sarr   scene_img_[2];

    ///eye buffer
  IplImage*   ipl_eye_img_[2];
  ///rgb scene buffer
  IplImage*   ipl_scene_img_[2];

  ///Orientation
  all::math::rpy_angle_t  heading_;


  ///image sizes:
  all::math::size3d_t eyedims_[2];
  all::math::size3d_t scenedims_[2];

  ///n smaple data size
  size_t  sample_sz_ ;

  ///per samples sizes ...
  size_t  elapsed_sz_ ;
  size_t  eye_sz_[2]  ;
  size_t  scene_sz_[2];

  //size_t  depth_sz  ;
  size_t  heading_sz_ ; 

  ///Samples num
  size_t nsamples_;
  ///
  BOOST_STATIC_CONSTANT(int, calibsamples = 9);
  ///
  int calib_samples_cnt_;

  ///loop control
  volatile bool running_;

  //useful?
  ///
  volatile bool bsavecalib_;
  ///
  volatile bool b_enabled_views_;

  //what?
  ///what operation... obsolete?
  int m_mode_;
  ///
  unsigned int msecspause_;

  ///the thread!
  boost::shared_ptr<boost::thread> thread_ptr_;
};
//-------------------------------------------------------------------------++
typedef boost::shared_ptr<gaze_machine2_t> gaze_machine_sptr;
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
#endif //gaze_machine_2_t_H_INCLUDED