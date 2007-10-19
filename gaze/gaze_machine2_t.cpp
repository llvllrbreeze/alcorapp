#include "gaze_machine2_t.h"
//-------------------------------------------------------------------------++
#include "alcor/core/config_parser_t.hpp"
#include "alcor/matlab/matlab_mx_utils.hpp"
#include "alcor/core/image_utils.h"
#include "alcor/core/opencv_utils.hpp"
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
    config.load(core::tags::ini,"config/gaze_machine2.ini");

    //
    //logname_ = config.get<std::string>("config.binfile", "gazelog.bin");
    std::string mticonf     = config.get<std::string>("config.mti", "config/mti_config.ini");
    std::string cameraconf  = config.get<std::string>("config.gazecamera","config/gazecamera.ini");

    //m_mode_              = config.get<int>("config.mode",0);
    calib_samples_cnt_   = config.get<int>("config.calibcount", 9);

    //    
    msecspause_          = config.get<unsigned int>("config.msecspause", 50);

    ///
    std::cout << "msec loop pause: " << msecspause_ << std::endl;

    ////
    //std::cout << "<Eye Camera ...>"; 
    //printf("Config: %s\n", cameraconf.c_str());

    //eyes left and right
    printf("--> Select EYE left\n");
    if( !eye_[left]->open(cameraconf,  "eyeleft",true) ) return false; 
    printf("--> Select EYE right\n");
    if( !eye_[right]->open(cameraconf, "eyeright", true) ) return false; 

    //scene left and right
    printf("--> Select SCENE left\n");
    if( !scene_[left]->open(cameraconf,  "sceneleft", true) ) return false; 
    printf("--> Select SCENE right\n");
    if( !scene_[right]->open(cameraconf, "sceneright", true) ) return false; 

    //
    //if (!is_opened_) is_opened_ = true;

    //
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<MTi......>"; 

    if( !mti_->open(mticonf) ) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    //allocate_();
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
  ////scene
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
  printf("Getting data dimension!\n"); 
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

  gazelog_.write((char*)&scene_sz_[left]  ,   sizeof(scene_sz_[left]  ));
  gazelog_.write((char*)&scene_sz_[right] ,   sizeof(scene_sz_[right] ));

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
  printf("CALIB LOOP\n");
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  {  

    //reset_mti();
    start_timing();

    printf("->machine booted ...starting loop\n");
    const unsigned char color  [3] = {215,  240,  60};
    const unsigned char blue   [3] = {0,  0,  255};

    const int imagestosave = calib_samples_cnt_;

    int imagecnt = 0;

    //Display image windows

    //
    CImgDisplay viewleft_eye (  eye_[left]->width(),  eye_[left]->height(), "EYE::LEFT");
    CImg<core::uint8_t> imleft_eye;
    //
    CImgDisplay viewright_eye (  eye_[right]->width(),  eye_[right]->height(), "EYE::RIGHT");
    CImg<core::uint8_t> imright_eye;
    
    //
    CImgDisplay viewleft_scene (  scene_[left]->width(),  scene_[left]->height(), "SCENE::LEFT");
    CImg<core::uint8_t> imleft_scene;
    //
    CImgDisplay viewright_scene (  scene_[right]->width(),  scene_[right]->height(), "SCENE::RIGHT");
    CImg<core::uint8_t> imright_scene;

  //  //in the furious loop!
  //  ///////////////////////////////////////////
    while (running_ && imagecnt < imagestosave)
    {
  //    //nsamples_++;
      //just triggers the acquisition
      sample_gaze_();

      //EYE LEFT
      all::core::uint8_sarr lefteye = 
        all::core::ocv::iplimage_to_planar(eye_[left]->retrieve_ipl_image());
      //EYE RIGHT
      all::core::uint8_sarr righteye = 
        all::core::ocv::iplimage_to_planar(eye_[right]->retrieve_ipl_image());
      //SCENE LEFT
      all::core::uint8_sarr leftscene = 
        all::core::ocv::iplimage_to_planar(scene_[left]->retrieve_ipl_image());
      //SCENE RIGHT
      all::core::uint8_sarr rightscene = 
        all::core::ocv::iplimage_to_planar(scene_[right]->retrieve_ipl_image());

      //assign
      imleft_eye.assign( lefteye.get(),  eye_[left]->width(), eye_[left]->height(), 1,eye_[left]->channels());
      imright_eye.assign( righteye.get(),  eye_[right]->width(), eye_[right]->height(), 1,eye_[right]->channels());
      imleft_scene.assign( leftscene.get(),  scene_[left]->width(), scene_[left]->height(), 1,scene_[left]->channels());
      imright_scene.assign( rightscene.get(),  scene_[right]->width(), scene_[right]->height(), 1,scene_[right]->channels());

      //Display
      imleft_eye.display(viewleft_eye) ;
      imright_eye.display(viewright_eye) ;
      imleft_scene.display(viewleft_scene) ;
      imright_scene.display(viewright_scene) ;

      if (bsavecalib_)
      {
        //
        bsavecalib_ = false;
        imagecnt++;        
        printf("Saving %d calib file.\n", imagecnt);
        ////////////////////////////////////////////////////////////////////////
        //MATLAB --------------------------------------------------------------+
  //      //
        MATFile *pmat = 0;
  //      //MATLAB
        std::string namebase = "calib_";
  //      //
        namebase += boost::lexical_cast<std::string>(imagecnt);
        namebase += ".mat";
  //      //
        pmat = matOpen(namebase.c_str(), "w");

        //-----------------
        mxArray* mx_lefteye = 
          matlab::buffer2array<core::uint8_t>::create_from_planar(lefteye.get()
                                                          , matlab::row_major
                                                          , eyedims_[left].row_
                                                          , eyedims_[left].col_
                                                          , eyedims_[left].depth_);
        mxArray* mx_righteye = 
          matlab::buffer2array<core::uint8_t>::create_from_planar(righteye.get()
                                                          , matlab::row_major
                                                          , eyedims_[right].row_
                                                          , eyedims_[right].col_
                                                          , eyedims_[right].depth_);
        //-----------------
        //mxArray!
        mxArray* mx_leftscene = 
          matlab::buffer2array<core::uint8_t>::create_from_planar(leftscene.get()
                                                          , matlab::row_major
                                                          , scenedims_[left].row_
                                                          , scenedims_[left].col_);
        mxArray* mx_rightscene = 
          matlab::buffer2array<core::uint8_t>::create_from_planar(rightscene.get()
                                                          , matlab::row_major
                                                          , scenedims_[right].row_
                                                          , scenedims_[right].col_);


        //-----------------
        mxArray* mx_num = mxCreateDoubleScalar(imagecnt);
        //-----------------
        //MTI
        mxArray* mx_roll = 
          mxCreateScalarDouble(heading_.roll.deg());
        mxArray* mx_pitch = 
          mxCreateScalarDouble(heading_.pitch.deg());
        mxArray* mx_yaw = 
          mxCreateScalarDouble(heading_.yaw.deg());

        //
       const char *field_names[] = {  "count"  ,
                                      "sceneleft"  ,
                                      "sceneright"  ,
                                      "eyeleft"  ,
                                      "eyeright"  ,
                                      "roll"   ,
                                      "pitch"  ,
                                      "yaw" };
  //      //
        mwSize dims[2] = {1, 1};

  //      //
        mxArray* ostruct= mxCreateStructArray(2, dims, 8, field_names);

  //      //
        int count_field
          , scene_L_field
          , scene_R_field
          , eye_L_field
          , eye_R_field
          , roll_field
          , pitch_field
          , yaw_field;

  //      //
        count_field     = mxGetFieldNumber(ostruct,   "count" );
        scene_L_field   = mxGetFieldNumber(ostruct,   "sceneleft" );
        scene_R_field     = mxGetFieldNumber(ostruct, "sceneright" );
        eye_L_field     = mxGetFieldNumber(ostruct,   "eyeleft" );
        eye_R_field     = mxGetFieldNumber(ostruct,   "eyeright" );
        roll_field      = mxGetFieldNumber(ostruct,   "roll"  );
        pitch_field     = mxGetFieldNumber(ostruct,   "pitch" );
        yaw_field       = mxGetFieldNumber(ostruct,   "yaw"   );

        //count
        mxSetFieldByNumber( ostruct
                          , 0
                          , count_field
                          , mx_num);
        //scene
        mxSetFieldByNumber( ostruct
                          , 0
                          , scene_L_field
                          , mx_leftscene);
        //depth
        mxSetFieldByNumber( ostruct
                          , 0
                          , scene_R_field
                          , mx_rightscene);
        //imeye
        mxSetFieldByNumber( ostruct
                          , 0
                          , eye_L_field
                          , mx_lefteye);
        //imeye
        mxSetFieldByNumber( ostruct
                          , 0
                          , eye_R_field
                          , mx_righteye);
        //roll
        mxSetFieldByNumber( ostruct
                          , 0
                          , roll_field
                          , mx_roll);
        //pitch
        mxSetFieldByNumber( ostruct
                          , 0
                          , pitch_field
                          , mx_pitch);
        //yaw
        mxSetFieldByNumber( ostruct
                          , 0
                          , yaw_field
                          , mx_yaw);
  //      //-----------------
        //write to matfile
        matPutVariable(pmat, "calibrazione", ostruct);

  //      //  
        matClose(pmat);  

        //
        mxDestroyArray(mx_leftscene);
        mxDestroyArray(mx_rightscene);
        mxDestroyArray(mx_lefteye);
        mxDestroyArray(mx_righteye);
        mxDestroyArray(mx_num);
        mxDestroyArray(mx_roll);
        mxDestroyArray(mx_pitch);
        mxDestroyArray(mx_yaw);
      }


      core::BOOST_SLEEP(1);
      //
      boost::thread::yield();
    }

  }
  else
      printf("devices not started!\n"); 
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
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  { 
    //
    cvNamedWindow("EyeLEFT");
    cvNamedWindow("EyeRIGHT");
    cvNamedWindow("SceneLEFT");
    cvNamedWindow("SceneRIGHT");
    ////
    //IplImage* my_left_eye = 0;
    //IplImage* my_right_eye = 0;  
    //IplImage* my_left_scene = 0;  
    //IplImage* my_right_scene = 0;  

    start_timing();
    printf("->machine booted ...starting loop\n");

    while (running_)
    {
      ///
      nsamples_++;

      //
      sample_gaze_();

      ////
      cvShowImage("EyeLEFT",    eye_[left]->retrieve_ipl_image()    );
      cvShowImage("EyeRIGHT",   eye_[right]->retrieve_ipl_image()   );
      cvShowImage("SceneLEFT",  scene_[left]->retrieve_ipl_image()  );
      cvShowImage("SceneRIGHT", scene_[right]->retrieve_ipl_image() );
      
      //eye_[left]->retrieve_ipl_image()   ;
      //eye_[right]->retrieve_ipl_image()  ;
      //scene_[left]->retrieve_ipl_image() ;
      //scene_[right]->retrieve_ipl_image();

      //
      cvWaitKey(1);
      //boost::thread::yield();
      //all::core::BOOST_SLEEP(msecspause_);
  }

    printf("Thread Canceled\n");
    elapsed_ = elapsed();
    ////
    //cvReleaseImage(&my_left_eye);
    //cvReleaseImage(&my_right_eye);  
    //cvReleaseImage(&my_left_scene);  
    //cvReleaseImage(&my_right_scene);  
  }
  else
      printf("devices not started!\n"); 
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
  //gaze_loop_ = boost::bind(&gaze_machine2_t::gaze_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine2_t::gaze_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::run_machine(calib_t const&)
{
  //gaze_loop_ = boost::bind(&gaze_machine2_t::calib_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine2_t::calib_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine2_t::run_machine(show_t const&)
{
  //gaze_loop_ = boost::bind(&gaze_machine2_t::show_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine2_t::show_loop, this) ) );
}

//-------------------------------------------------------------------------++
/////////////////////////////////////////////////////////////////////////////
}}///all::sense::detail
