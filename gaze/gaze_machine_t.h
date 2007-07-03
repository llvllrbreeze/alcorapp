#ifndef gaze_machine_t_H_INCLUDED
#define gaze_machine_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include "gaze_machine_inc.h"
#include "alcor/math/size3d_t.hpp"
//-------------------------------------------------------------------------++
namespace all { namespace gaze{
//-------------------------------------------------------------------------++
struct calib_t {};
struct binlog_t{};
struct show_t{};
//-------------------------------------------------------------------------++
static const calib_t  calib   = calib_t();
static const binlog_t binlog  = binlog_t();
static const show_t   show    = show_t();
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
  void run_gaze_machine();

  ///
  void run_machine(calib_t const&);
  ///
  void run_machine(binlog_t const&);
  ///
  void run_machine(show_t const&);

  ///
  void cancel(){running_=false;};

  ///
  void enable_views(bool b_enable=true)
      {b_enabled_views = b_enable;}

  ///
  void set_logname(std::string&);
  ///
  void sample_gaze_(); 
  ///
  void save_calib(){bsavecalib = true;};


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
  ///
  void null_op_();

  ///calib loop
  void calib_loop();

  ///sampling loop
  void gaze_loop();

  ///sampling loop
  void show_loop();
  /////////////////////////////////////////////////
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

  ///
  bool is_opened_;

  ///allocate enough space .. 
  void allocate_();
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
  ///eye buffer
  all::core::uint8_sarr   ieye;
  ///rgb scene buffer
  all::core::uint8_sarr   iscene;
  ///xyz depth
  all::core::single_sarr  idepth;
  ///Orientation
  all::math::rpy_angle_t  ihead;


  ///image sizes:
  all::math::size3d_t eyedims_;
  all::math::size3d_t scenedims_;
  
  ///per samples sizes ...
  size_t  elapsed_sz ;
  size_t  eye_sz     ;
  size_t  scene_sz   ;
  size_t  depth_sz   ;
  size_t  head_sz    ; 

  ///Samples num
  size_t nsamples_;
  ///
  BOOST_STATIC_CONSTANT(int, calibsamples = 9);
  ///
  int calib_samples_cnt;

  ///loop control
  volatile bool running_;
  ///
  volatile bool bsavecalib;
  ///
  volatile bool b_enabled_views;
  ///
  int m_mode;
  ///
  unsigned int msecspause;
  ///
  boost::shared_ptr<boost::thread> thread_ptr;
};
//-------------------------------------------------------------------------++
typedef boost::shared_ptr<gaze_machine_t> gaze_machine_sptr;
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
#endif //gaze_machine_t_H_INCLUDED