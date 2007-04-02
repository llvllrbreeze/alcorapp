#include "gaze_machine_t.h"
#include "alcor/core/config_parser_t.hpp"
#include "alcor/matlab/matlab_mx_utils.hpp"
#include "alcor.extern/CImg/CImg.h"
using namespace cimg_library;
//#define NOBEE_
//-------------------------------------------------------------------------++
#include <boost/lexical_cast.hpp>
#include "mat.h"
//-------------------------------------------------------------------------++
namespace all { namespace gaze { 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
gaze_machine_t::gaze_machine_t():
                        nsamples_(0)
                      , calib_samples_cnt(0)
                      , running_(true)
                      , logname_("gazelog.bin")
                      , is_opened_(false)
{
  process_gaze_data = boost::bind(&gaze_machine_t::null_op_, this);

  reset_devices_();

  print_welcome();
}
/////////////////////////////////////////////////////////////////////////////
gaze_machine_t::~gaze_machine_t()
{

}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::set_logname(std::string& logname)
{
  logname_ = logname;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::reset_devices_()
{
  eye_.reset(new sense::opencv_grabber_t);
  mti_.reset(new sense::MTi_driver_t());
  bee_.reset(new sense::bumblebee_driver_t);
}
/////////////////////////////////////////////////////////////////////////////
bool gaze_machine_t::boot_machine_()
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
    std::string beeconf = config.get<std::string>("config.bee", "config/bumblebeeB.ini");
    //    
    msecspause          = config.get<unsigned int>("config.msecspause", 50);

    ////
    std::cout << "<Eye Camera ...>"; 
    printf("Config: %s\n", eyeconf.c_str());
    //eye_.reset(new sense::opencv_grabber_t);

    if( !eye_->open(eyeconf, !is_opened_) ) return false;    
    if (!is_opened_) is_opened_ = true;

    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<MTi......>"; 
    //mti_.reset(new sense::MTi_driver_t());
    if( !mti_->open(mticonf) ) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

#ifndef NOBEE_
    //
    std::cout << "<Bumblebee ...>";
    //bee_.reset(new sense::bumblebee_driver_t);
    if( !bee_->open(beeconf) ) return false;
    std::cout << "<Done>" << std::endl<< std::endl;
#endif

    //
    gazelog_.open(logname_.c_str(),std::ios::out|std::ios::binary);
    
    allocate_();
    
    write_header_();

    timer_.restart();
    return true;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::allocate_()
{
  ieye.reset  ( new core::uint8_t  [eye_->size()]              );
  //iscene.reset( new core::uint8_t  [bee_->color_buffer_size()] );
  //idepth.reset( new core::single_t [bee_->depth_buffer_size()] );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::print_welcome()
{
std::cout << "Library Ver: " <<  core::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  core::AUTHOR_INFORMATION << std::endl;
std::cout << "GAZE MACHINE Ver: " << gaze::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  gaze::AUTHOR_INFORMATION << std::endl;

//std::cout << std::endl
//<< "          ,-~-. _.---._ ,-~-. " << std::endl
//<< "         / .- ,'       `. -.\\ " << std::endl
//<< "        \\ ` /`        '\\ ' / " << std::endl
//<< "          `-/   'a___a`  \\-'" << std::endl
//<< "            |   ,'(_)`.   | " << std::endl
//<< "           \\  ( ._|_. )  / " << std::endl
//<< "            \\  `.___,'  /" << std::endl
//<< "            .-`._     _,'-." << std::endl
//<< "          ,'  ,' `---' `.  `." << std::endl
//<< "         /   /     :    \\  \\ " << std::endl
//<< "       ,'   /      :     \\   `." << std::endl
//<< "     ,'     |      :      |     `." << std::endl
//<< "    |     ,'|      :      |`.     |" << std::endl
//<< "    `.__,' .-\\     :    /-. `.__,'" << std::endl
//<< "          /   `.   :   ,'  \\ " << std::endl
//<< "   .""-.,'      `._:_,'      `.,-"". " << std::endl
//<< "  / ,-. `         ) (         ' ,-.\\ " << std::endl
//<< " ( (   `.       ,'   `.       ,'   ) )" << std::endl
//<< " \\\\   \\   _,'       `._   /    / / " << std::endl
//<< "   `.`._,'  /            \\  `._,',' " << std::endl
//<< "     `.__.-'               `-.__,' " << std::endl << std::endl;

}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::reset_mti()
{
  mti_->reset(sense::tags::global_reset);
  mti_->reset(sense::tags::heading_reset);
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::sample_gaze_()
{  

#ifndef NOBEE_
  //grab
  bee_->grab();
#endif
  //eye image
  eye_->get_color_buffer(ieye);
  //rpy
  ihead = mti_->get_euler();
  //time
  elapsed_ = timer_.elapsed();  
#ifndef NOBEE_
  //images
  iscene = bee_->get_color_buffer(core::right_img);  
  idepth = bee_->get_depth_buffer();
#endif

}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::write_header_()
{
  printf("Writing Header!\n");

  size_t sample_sz = sizeof(nsamples_);
  
  elapsed_sz = sizeof(double);
  eye_sz     = eye_->size();

#ifndef NOBEE_
  scene_sz   = bee_->color_buffer_size();
  depth_sz   = bee_->depth_buffer_size();
#endif

  head_sz    = sizeof(math::rpy_angle_t);

  //skip  ... this position will hold the number of frames.
  gazelog_.seekp(sample_sz);
  //skip .... this will hold the total time elapsed.
  gazelog_.seekp(sample_sz+elapsed_sz);

  //eye dimensions
  eyedims_.row_ = eye_->height();
  eyedims_.col_ = eye_->width();
  eyedims_.depth_= eye_->channels();
  gazelog_.write((char*)&eyedims_, sizeof(eyedims_));

  ////scene dimensions
  scenedims_.row_ = bee_->nrows();
  scenedims_.col_ = bee_->ncols();
  scenedims_.depth_ = 3; //always
  gazelog_.write((char*)&scenedims_, sizeof(scenedims_) );  

  gazelog_.write((char*)&elapsed_sz, sizeof(size_t)); 
  gazelog_.write((char*)&eye_sz    , sizeof(size_t));

#ifndef NOBEE_
  gazelog_.write((char*)&scene_sz  ,   sizeof(size_t));
  gazelog_.write((char*)&depth_sz  ,   sizeof(size_t));
#endif

  gazelog_.write((char*)&head_sz  ,   sizeof(size_t));
}
//-------------------------------------------------------------------------++
void gaze_machine_t::write_gaze_()
{
  gazelog_.write((char*)&elapsed_,      elapsed_sz); 
  gazelog_.write((char*)ieye.get()  ,   eye_sz);

#ifndef NOBEE_
  gazelog_.write((char*)iscene.get(),   scene_sz);
  gazelog_.write((char*)idepth.get(),   depth_sz);
#endif

  gazelog_.write((char*)&ihead,         head_sz);
}
//-------------------------------------------------------------------------++
void gaze_machine_t::calib_()
{
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  {  
    //
    CImgDisplay view (  eye_->width(),  eye_->height(), "Camera");
    CImg<core::uint8_t> imag;
    //
    CImgDisplay viewscene (  bee_->ncols(), bee_->nrows(), "Scene");
    CImg<core::uint8_t> imagscene;

    reset_mti();
    start_timing();
    printf("->machine booted ...starting loop\n");
    const unsigned char color  [3] = {215,  240,  60};
    const unsigned char blue   [3] = {0,  0,  255};

    const int imagestosave = 9;
    int imagecnt = 0;
    //in the furious loop!
    ///////////////////////////////////////////
    while (running_ && imagecnt < imagestosave)
    {
      //nsamples_++;
      sample_gaze_();
      //write_gaze_();

      //EYE
      imag.assign( ieye.get(),  eye_->width(), eye_->height(), 1,eye_->channels());
      //
      //imag.draw_text(10,20,  blue, 0, 16, 1, "Elapsed: %.2f", elapsed_);
      //imag.draw_text(10,40,  blue, 0, 16, 1, "Roll: %.2f", ihead.roll);
      //imag.draw_text(10,60,  blue, 0, 16, 1, "Pitch: %.2f", ihead.pitch);
      //imag.draw_text(10,80,  blue, 0, 16, 1, "Yaw: %.2f", ihead.yaw);
      //imag.draw_text(10,120, blue, 0, 16, 1, "#: %d", nsamples_);

      //imag.draw_rectangle(1, 1, 200, 200,color, 0.2);
      //imag.draw_line(1,1, 200,1, color);
      //imag.draw_line(1,1, 0,200, color);
      //imag.draw_line(200,1, 200,200, color);
      //imag.draw_line(1,200, 200,200, color);
      imag.display(view) ;

      //SCENE
      imagscene.assign( 
            iscene.get()
        ,   bee_->ncols()
        ,   bee_->nrows()
        ,   1
        ,   3);

      ///
      imagscene.display(viewscene);

      ///
      if (viewscene.key == cimg::keySPACE) 
      {
        imagecnt++;
        ////////////////////////////////////////////////////////////////////////
        //MATLAB --------------------------------------------------------------+
        //
        MATFile *pmat = 0;
        //MATLAB
        std::string namebase = "calib_";
        //
        namebase += boost::lexical_cast<std::string>(imagecnt);
        namebase += ".mat";
        //
        pmat = matOpen(namebase.c_str(), "w");

        //-----------------
        mxArray* mx_rgb = 
          matlab::buffer2array<core::uint8_t>::create_from_planar(iscene.get()
                                                          , matlab::row_major
                                                          , scenedims_.row_
                                                          , scenedims_.col_);
        //-----------------
        mxArray* mx_eye = 
          matlab::buffer2array<core::uint8_t>::create_from_planar(ieye.get()
                                                          , matlab::row_major
                                                          , eyedims_.row_
                                                          , eyedims_.col_
                                                          , eyedims_.depth_);

        //-----------------
        mxArray* mx_depth  = 
          all::matlab::buffer2array<  core::single_t  >::create_from_planar(
            idepth.get()
          , matlab::row_major
          , scenedims_.row_
          , scenedims_.col_);
        //-----------------
        mxArray* mx_num = mxCreateDoubleScalar(imagecnt);
        //-----------------

        //add to file
        matPutVariable(pmat, "calib_rgb", mx_rgb);
        matPutVariable(pmat, "calib_xyz", mx_depth);
        matPutVariable(pmat, "calib_eye", mx_eye);
        matPutVariable(pmat, "calib_num", mx_num);
        
        //  
        matClose(pmat);  

        //
        mxDestroyArray(mx_rgb);
        mxDestroyArray(mx_depth);
        mxDestroyArray(mx_eye);
        mxDestroyArray(mx_num);
      }

      //
      cimg::wait(100);
      //
      boost::thread::yield();
    }

    printf("Thread Canceled\n");
  }
  else
      printf("devices not started!\n"); 
}
//-------------------------------------------------------------------------++
void gaze_machine_t::null_op_()
{
}
//-------------------------------------------------------------------------++
int gaze_machine_t::nsamples() const
{
return nsamples_;
}
//-------------------------------------------------------------------------++
void gaze_machine_t::run_machine(binlog_t const&)
{
  gaze_loop_ = boost::bind(&gaze_machine_t::gaze_loop,      this);
  thread_ptr.reset( new boost::thread(boost::bind(&gaze_machine_t::gaze_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine_t::run_machine(calib_t const&)
{
  gaze_loop_ = boost::bind(&gaze_machine_t::calib_,      this);
  thread_ptr.reset( new boost::thread(boost::bind(&gaze_machine_t::gaze_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine_t::gaze_loop()
{
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  { 
    //
    CImgDisplay view (  eye_->width(),  eye_->height(), "Camera");
    CImg<core::uint8_t> imag;
    //
    CImgDisplay viewscene (  bee_->ncols(), bee_->nrows(), "Scene");
    CImg<core::uint8_t> imagscene;

    ////
    //CImgDisplay threedview (  bee_->ncols(),  bee_->nrows(), "3D");
    //CImg<core::single_t> threedscene;

    reset_mti();
    start_timing();
    printf("->machine booted ...starting loop\n");
    const unsigned char color  [3] = {215,  240,  60};
    const unsigned char blue   [3] = {0,  0,  255};
    while (running_)
    {
      ///
      nsamples_++;
      sample_gaze_();
      write_gaze_();
      //////
      imag.assign( ieye.get(),  eye_->width(), eye_->height(), 1,eye_->channels());
      //
      imag.draw_text(10,20,  blue, 0, 16, 1, "Elapsed: %.2f", elapsed_);
      imag.draw_text(10,40,  blue, 0, 16, 1, "Roll: %.2f", ihead.roll);
      imag.draw_text(10,60,  blue, 0, 16, 1, "Pitch: %.2f", ihead.pitch);
      imag.draw_text(10,80,  blue, 0, 16, 1, "Yaw: %.2f", ihead.yaw);
      imag.draw_text(10,120, blue, 0, 16, 1, "#: %d", nsamples_);

      imag.draw_rectangle(1, 1, 200, 200,color, 0.2);
      imag.draw_line(1,1, 200,1, color);
      imag.draw_line(1,1, 0,200, color);
      imag.draw_line(200,1, 200,200, color);
      imag.draw_line(1,200, 200,200, color);
      imag.display(view) ;

      //DRAW
      imagscene.assign( 
            iscene.get()
        ,   bee_->ncols()
        ,   bee_->nrows()
        ,   1
        ,   3);

      ///
      imagscene.display(viewscene);

      /////
      // threedscene.assign(
      //   idepth.get()+ (bee_->ncols()*bee_->nrows()*2)
      //   ,  bee_->ncols()
      //   ,  bee_->nrows()
      //   ,  1);

      //threedscene.display(threedview);

      boost::thread::yield();
      all::core::BOOST_SLEEP(msecspause);
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
    //
    gazelog_.close(); 
  }
  else
      printf("devices not started!\n"); 
}
//-------------------------------------------------------------------------++
/////////////////////////////////////////////////////////////////////////////
}}///all::sense::detail
