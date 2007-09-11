#include "gaze_machine2_t.h"
//-------------------------------------------------------------------------++
#include "alcor/core/config_parser_t.hpp"
#include "alcor/matlab/matlab_mx_utils.hpp"
//-------------------------------------------------------------------------++
//display
#include "alcor.extern/CImg/CImg.h"
using namespace cimg_library;
//#define NOBEE_
//-------------------------------------------------------------------------++
#include <boost/lexical_cast.hpp>
//matlab
#include "mat.h"
//-------------------------------------------------------------------------++
namespace all { namespace gaze { 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
gaze_machine2_t::gaze_machine2_t():
                        nsamples_(0)
                      , calib_samples_cnt_(0)
                      , running_(true)
                      , logname_("gazelog.bin")
                      , is_opened_(false)
                      , bsavecalib_(false)
                      , b_enabled_views_(false)
{
  f_process_gaze_data = boost::bind(&gaze_machine2_t::null_op_, this);

  reset_devices_();

  print_welcome();
}
/////////////////////////////////////////////////////////////////////////////
gaze_machine2_t::~gaze_machine2_t()
{

}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::set_logname(std::string& logname)
{
  logname_ = logname;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::reset_devices_()
{
  //eye cameras
  eye_[gaze_machine2_t::left].reset(new sense::opencv_grabber_t);
  eye_[gaze_machine2_t::right].reset(new sense::opencv_grabber_t);
  
  //
  scene_[gaze_machine2_t::left].reset(new sense::opencv_grabber_t);
  scene_[gaze_machine2_t::right].reset(new sense::opencv_grabber_t);

  mti_.reset(new sense::MTi_driver_t());
  //bee_.reset(new sense::bumblebee_driver_t);
}
/////////////////////////////////////////////////////////////////////////////
bool gaze_machine2_t::boot_machine_()
{

//std::cout << std::endl
//<< " _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____ " << std::endl
//<< "|  |  |     |   __|   __|  | | | |     |   --|     |-   -| | | |   __|" << std::endl
//<< "|_____|__|__|_____|_____|  |_|_|_|__|__|_____|__|__|_____|_|___|_____|" << std::endl
//    <<std::endl;

    std::cout << "<<STARTING MACHINE>>" << std::endl;


    all::core::config_parser_t config;
    config.load(core::tags::ini,"config/gaze_machine.ini");

    //
    //logname_ = config.get<std::string>("config.binfile", "gazelog.bin");
    std::string mticonf = config.get<std::string>("config.mti", "config/mti_config.ini");
    std::string eyeconf = config.get<std::string>("config.reye","config/eyecamera.ini");
    //std::string beeconf = config.get<std::string>("config.bee", "config/bumblebeeB.ini");
    m_mode_              = config.get<int>("config.mode",0);
    calib_samples_cnt_   = config.get<int>("config.calibcount", 9);

    //    
    msecspause_          = config.get<unsigned int>("config.msecspause", 50);

    ///
    std::cout << "msec loop pause: " << msecspause_ << std::endl;

    ///
    switch(m_mode_)
    {
      case 0:
            std::cout << "MODe: show" << std::endl;
            break;
      case 1: 
            std::cout << "MODe: calib" << std::endl;
            break;
      case 2:
            std::cout << "MODe: acquire" << std::endl;
            break;
    }


    ////
    std::cout << "<Eye Camera ...>"; 
    printf("Config: %s\n", eyeconf.c_str());
    //eye_.reset(new sense::opencv_grabber_t);

    //eyes left and right
    if( !eye_[left]->open(eyeconf,  !is_opened_) ) return false; 
    if( !eye_[right]->open(eyeconf, !is_opened_) ) return false; 

    //scene left and right
    if( !scene_[left]->open(eyeconf,  !is_opened_) ) return false; 
    if( !scene_[right]->open(eyeconf, !is_opened_) ) return false; 

    //
    if (!is_opened_) is_opened_ = true;

    //
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<MTi......>"; 
    //mti_.reset(new sense::MTi_driver_t());
    if( !mti_->open(mticonf) ) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

//#ifndef NOBEE_
//    //
//    std::cout << "<Bumblebee ...>";
//    //bee_.reset(new sense::bumblebee_driver_t);
//    if( !bee_->open(beeconf) ) return false;
//    std::cout << "<Done>" << std::endl<< std::endl;
//#endif
    //
    allocate_();
    //
    get_dims_();

    timer_.restart();
    return true;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::allocate_()
{
  //eye_img_[left].reset(new core::uint8_t  [eye_[left]->size()]);
  //eye_img_[right].reset(new core::uint8_t  [eye_[right]->size()]);

  //iscene.reset( new core::uint8_t  [bee_->color_buffer_size()] );
  //idepth.reset( new core::single_t [bee_->depth_buffer_size()] );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::print_welcome()
{
std::cout << "Library Ver: " <<  core::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  core::AUTHOR_INFORMATION << std::endl;
std::cout << "GAZE MACHINE Ver: " << gaze::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  gaze::AUTHOR_INFORMATION << std::endl;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::reset_mti()
{
  mti_->reset(sense::tags::global_reset);
  mti_->reset(sense::tags::heading_reset);
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::sample_gaze_()
{  
  //eye
  eye_[left]->grab_ipl_image();
  eye_[right]->grab_ipl_image();
  //scene
  scene_[left]->grab_ipl_image();
  scene_[right]->grab_ipl_image();
  //rpy
  heading_ = mti_->get_euler();
  //time
  elapsed_ = timer_.elapsed();  
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::get_dims_()
{
  printf("Getting image dimension!\n"); 
  sample_sz_ = sizeof(nsamples_);
  
  elapsed_sz_ = sizeof(double);
  eye_sz_[left]  = eye_[left]->size();
  eye_sz_[right] = eye_[right]->size();

  //
  scene_sz_[left] = scene_[left]->size();
  scene_sz_[right] = scene_[right]->size();

  heading_sz_    = sizeof(math::rpy_angle_t);

  //eye dimensions
  eyedims_[left].row_   = eye_[left]->height();
  eyedims_[left].col_   = eye_[left]->width();
  eyedims_[left].depth_ = eye_[left]->channels();

  //
  eyedims_[right].row_   = eye_[right]->height();
  eyedims_[right].col_   = eye_[right]->width();
  eyedims_[right].depth_ = eye_[right]->channels();

  ///scene dimensions
  scenedims_[left].row_   = scene_[left]->height();
  scenedims_[left].col_   = scene_[left]->width();
  scenedims_[left].depth_ = scene_[left]->channels();
  //
  scenedims_[right].row_   = scene_[right]->height();
  scenedims_[right].col_   = scene_[right]->width();
  scenedims_[right].depth_ = scene_[right]->channels();
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine2_t::write_header_()
{
  printf("Writing Header!\n");
  //skip  ... this position will hold the number of frames.
  gazelog_.seekp(sample_sz_);

  //skip .... this will hold the total time elapsed.
  gazelog_.seekp(sample_sz_ + elapsed_sz_);


  gazelog_.write((char*)&eyedims_[left], sizeof(eyedims_[left]));
  gazelog_.write((char*)&eyedims_[right], sizeof(eyedims_[right]));

  gazelog_.write((char*)&scenedims_[left], sizeof(scenedims_[left]) );  
  gazelog_.write((char*)&scenedims_[right], sizeof(scenedims_[right]) );  

  gazelog_.write((char*)&elapsed_sz_, sizeof(size_t)); 

  gazelog_.write((char*)&eye_sz_[left]  , sizeof(eye_sz_[left]));
  gazelog_.write((char*)&eye_sz_[right] , sizeof(eye_sz_[right]));

//#ifndef NOBEE_
  gazelog_.write((char*)&scene_sz_[left]  ,   sizeof(scene_sz_[left]  ));
  gazelog_.write((char*)&scene_sz_[right] ,   sizeof(scene_sz_[right] ));

  //gazelog_.write((char*)&depth_sz  ,   sizeof(size_t));
//#endif

  gazelog_.write((char*)&heading_sz_  ,   sizeof(heading_sz_));
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::write_gaze_()
{
  ipl_eye_img_[left]    = eye_[left]->retrieve_ipl_image();
  ipl_eye_img_[right]   = eye_[right]->retrieve_ipl_image();

  ipl_scene_img_[left]  = scene_[left]->retrieve_ipl_image();
  ipl_scene_img_[right] = scene_[right]->retrieve_ipl_image();

  //write time
  gazelog_.write((char*)&elapsed_,      elapsed_sz_); 
  //write eyes
  gazelog_.write((char*)&ipl_eye_img_[left]->imageData,    eye_sz_[left]);
  gazelog_.write((char*)&ipl_eye_img_[right]->imageData,   eye_sz_[right]);

  //write scene
  gazelog_.write((char*)&ipl_scene_img_[left]->imageData,   scene_sz_[left]);
  gazelog_.write((char*)&ipl_scene_img_[right]->imageData,  scene_sz_[right]);

  //write mti
  gazelog_.write((char*)&heading_,   heading_sz_);
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::calib_loop()
{
  //printf("->in the thread loop!\n");
  //printf("->boot_machine_ .. \n");
  //if(boot_machine_())
  //{  

  //  //
  //  CImgDisplay view (  eye_->width(),  eye_->height(), "Camera");
  //  CImg<core::uint8_t> imag;
  //  //
  //  CImgDisplay viewscene (  bee_->ncols(), bee_->nrows(), "Scene");
  //  CImg<core::uint8_t> imagscene;

  //  reset_mti();
  //  start_timing();
  //  printf("->machine booted ...starting loop\n");
  //  const unsigned char color  [3] = {215,  240,  60};
  //  const unsigned char blue   [3] = {0,  0,  255};

  //  const int imagestosave = calib_samples_cnt;
  //  int imagecnt = 0;
  //  //in the furious loop!
  //  ///////////////////////////////////////////
  //  while (running_ && imagecnt < imagestosave)
  //  {
  //    //nsamples_++;
  //    sample_gaze_();


  //    //EYE
  //    imag.assign( ieye.get(),  eye_->width(), eye_->height(), 1,eye_->channels());

  //    imag.display(view) ;

  //    //SCENE
  //    imagscene.assign( 
  //          iscene.get()
  //      ,   bee_->ncols()
  //      ,   bee_->nrows()
  //      ,   1
  //      ,   3);

  //    ///
  //    imagscene.display(viewscene);
  //    ///
  //    //if (viewscene.key == cimg::keySPACE) 
  //    if (bsavecalib)
  //    {
  //      //
  //      bsavecalib = false;

  //      //viewscene.key&=0;
  //      imagecnt++;        
  //      printf("Saving %d calib file.\n", imagecnt);
  //      ////////////////////////////////////////////////////////////////////////
  //      //MATLAB --------------------------------------------------------------+
  //      //
  //      MATFile *pmat = 0;
  //      //MATLAB
  //      std::string namebase = "calib_";
  //      //
  //      namebase += boost::lexical_cast<std::string>(imagecnt);
  //      namebase += ".mat";
  //      //
  //      pmat = matOpen(namebase.c_str(), "w");

  //      //-----------------
  //      mxArray* mx_rgb = 
  //        matlab::buffer2array<core::uint8_t>::create_from_planar(iscene.get()
  //                                                        , matlab::row_major
  //                                                        , scenedims_.row_
  //                                                        , scenedims_.col_);
  //      //-----------------
  //      mxArray* mx_eye = 
  //        matlab::buffer2array<core::uint8_t>::create_from_planar(ieye.get()
  //                                                        , matlab::row_major
  //                                                        , eyedims_.row_
  //                                                        , eyedims_.col_
  //                                                        , eyedims_.depth_);

  //      //-----------------
  //      mxArray* mx_depth  = 
  //        all::matlab::buffer2array<  core::single_t  >::create_from_planar(
  //          idepth.get()
  //        , matlab::row_major
  //        , scenedims_.row_
  //        , scenedims_.col_);

  //      //-----------------
  //      mxArray* mx_num = mxCreateDoubleScalar(imagecnt);
  //      //-----------------
  //      //MTI
  //      mxArray* mx_roll = 
  //        mxCreateScalarDouble(ihead.roll.deg());
  //      mxArray* mx_pitch = 
  //        mxCreateScalarDouble(ihead.pitch.deg());
  //      mxArray* mx_yaw = 
  //        mxCreateScalarDouble(ihead.yaw.deg());

  //      //
  //     const char *field_names[] = {  "count"  ,
  //                                    "scene"  ,
  //                                    "depth"  ,
  //                                    "imeye"  ,
  //                                    "roll"   ,
  //                                    "pitch"  ,
  //                                    "yaw" };
  //      //
  //      mwSize dims[2] = {1, 1};

  //      //
  //      mxArray* ostruct= mxCreateStructArray(2, dims, 7, field_names);

  //      //
  //      int count_field
  //        , scene_field
  //        , depth_field
  //        , imeye_field
  //        , roll_field
  //        , pitch_field
  //        , yaw_field;

  //      //
  //      count_field   = mxGetFieldNumber(ostruct,   "count" );
  //      scene_field   = mxGetFieldNumber(ostruct,   "scene" );
  //      depth_field   = mxGetFieldNumber(ostruct,   "depth" );
  //      imeye_field   = mxGetFieldNumber(ostruct,   "imeye" );
  //      roll_field    = mxGetFieldNumber(ostruct,   "roll"  );
  //      pitch_field   = mxGetFieldNumber(ostruct,   "pitch" );
  //      yaw_field     = mxGetFieldNumber(ostruct,   "yaw"   );

  //      //count
  //      mxSetFieldByNumber( ostruct
  //                        , 0
  //                        , count_field
  //                        , mx_num);
  //      //scene
  //      mxSetFieldByNumber( ostruct
  //                        , 0
  //                        , scene_field
  //                        , mx_rgb);
  //      //depth
  //      mxSetFieldByNumber( ostruct
  //                        , 0
  //                        , depth_field
  //                        , mx_depth);
  //      //imeye
  //      mxSetFieldByNumber( ostruct
  //                        , 0
  //                        , imeye_field
  //                        , mx_eye);
  //      //roll
  //      mxSetFieldByNumber( ostruct
  //                        , 0
  //                        , roll_field
  //                        , mx_roll);
  //      //pitch
  //      mxSetFieldByNumber( ostruct
  //                        , 0
  //                        , pitch_field
  //                        , mx_pitch);
  //      //yaw
  //      mxSetFieldByNumber( ostruct
  //                        , 0
  //                        , yaw_field
  //                        , mx_yaw);
  //      //-----------------
  //      //add to file
  //      //matPutVariable(pmat, "calib_rgb", mx_rgb);
  //      //matPutVariable(pmat, "calib_xyz", mx_depth);
  //      //matPutVariable(pmat, "calib_eye", mx_eye);
  //      //matPutVariable(pmat, "calib_num", mx_num);

  //      //write to matfile
  //      matPutVariable(pmat, "calibrazione", ostruct);

  //      //  
  //      matClose(pmat);  

  //      //
  //      mxDestroyArray(mx_rgb);
  //      mxDestroyArray(mx_depth);
  //      mxDestroyArray(mx_eye);
  //      mxDestroyArray(mx_num);
  //      mxDestroyArray(mx_roll);
  //      mxDestroyArray(mx_pitch);
  //      mxDestroyArray(mx_yaw);
  //    }

  //    //
  //    //viewscene.wait(100);

  //    core::BOOST_SLEEP(100);
  //    //
  //    boost::thread::yield();
  //  }

  //  printf("Thread Canceled\n");
  //}
  //else
  //    printf("devices not started!\n"); 
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::gaze_loop()
{
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  { 
    ///
    gazelog_.open(logname_.c_str(),std::ios::out|std::ios::binary);
    ///
    write_header_();

    //reset_mti();
    printf("->machine booted ...starting loop\n");
    //reset timer
    start_timing();
    //entering the loop
    while (running_)
    {
      ///
      nsamples_++;
      sample_gaze_();
      write_gaze_();
      boost::thread::yield();
      all::core::BOOST_SLEEP(msecspause_);
    }

    printf("Thread Canceled\n");
    elapsed_ = elapsed();
    //go beginning
    gazelog_.seekp(std::ios::beg);
    //save info
    //numsamples
    gazelog_.write((char*)&nsamples_, sizeof(nsamples_)); 
    //time spent
    gazelog_.write((char*)&elapsed_, sizeof(elapsed_)); 
    //and then close
    gazelog_.close(); 
  }
  else
      printf("devices not started!\n"); 
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::show_loop()
{
  //printf("->in the thread loop!\n");
  //printf("->boot_machine_ .. \n");
  //if(boot_machine_())
  //{ 
  //  //
  //  CImgDisplay view (  eye_->width(),  eye_->height(), "Camera");
  //  CImg<core::uint8_t> imag;
  //  //
  //  CImgDisplay viewscene (  bee_->ncols(), bee_->nrows(), "Scene");
  //  CImg<core::uint8_t> imagscene;

  //  ////
  //  //CImgDisplay threedview (  bee_->ncols(),  bee_->nrows(), "3D");
  //  //CImg<core::single_t> threedscene;

  //  reset_mti();
  //  start_timing();
  //  printf("->machine booted ...starting loop\n");
  //  const unsigned char color  [3] = {215,  240,  60};
  //  const unsigned char blue   [3] = {0,  0,  255};
  //  while (running_)
  //  {
  //    ///
  //    nsamples_++;

  //    sample_gaze_();

  //    //////
  //    imag.assign( ieye.get(),  eye_->width(), eye_->height(), 1,eye_->channels());
  //    //
  //    imag.draw_text(10,20,  blue, 0, 16, 1, "Elapsed: %.2f", elapsed_);
  //    imag.draw_text(10,40,  blue, 0, 16, 1, "Roll: %.2f", ihead.roll);
  //    imag.draw_text(10,60,  blue, 0, 16, 1, "Pitch: %.2f", ihead.pitch);
  //    imag.draw_text(10,80,  blue, 0, 16, 1, "Yaw: %.2f", ihead.yaw);
  //    imag.draw_text(10,120, blue, 0, 16, 1, "#: %d", nsamples_);

  //    imag.draw_rectangle(1, 1, 200, 200,color, 0.2);
  //    imag.draw_line(1,1, 200,1, color);
  //    imag.draw_line(1,1, 0,200, color);
  //    imag.draw_line(200,1, 200,200, color);
  //    imag.draw_line(1,200, 200,200, color);
  //    imag.display(view) ;

  //    //DRAW
  //    imagscene.assign( 
  //          iscene.get()
  //      ,   bee_->ncols()
  //      ,   bee_->nrows()
  //      ,   1
  //      ,   3);

  //    ///
  //    imagscene.display(viewscene);
  //    /////
  //    // threedscene.assign(
  //    //   idepth.get()+ (bee_->ncols()*bee_->nrows()*2)
  //    //   ,  bee_->ncols()
  //    //   ,  bee_->nrows()
  //    //   ,  1);

  //    //threedscene.display(threedview);

  //    boost::thread::yield();
  //    all::core::BOOST_SLEEP(msecspause);
  //}
  //  printf("Thread Canceled\n");
  //  elapsed_ = elapsed();
 
  //}
  //else
  //    printf("devices not started!\n"); 
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::null_op_()
{
}
//-------------------------------------------------------------------------++
int gaze_machine2_t::nsamples() const
{
return nsamples_;
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::run_gaze_machine()
{

}
//-------------------------------------------------------------------------++
void gaze_machine2_t::run_machine(binlog_t const&)
{
  gaze_loop_ = boost::bind(&gaze_machine2_t::gaze_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine2_t::gaze_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::run_machine(calib_t const&)
{
  gaze_loop_ = boost::bind(&gaze_machine2_t::calib_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine2_t::calib_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::run_machine(show_t const&)
{
  gaze_loop_ = boost::bind(&gaze_machine2_t::show_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine2_t::show_loop, this) ) );
}

//-------------------------------------------------------------------------++
/////////////////////////////////////////////////////////////////////////////
}}///all::sense::detail
