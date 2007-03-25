#include "gaze_machine_t.h"
#include "alcor/core/config_parser_t.hpp"
#include "alcor.extern/CImg/CImg.h"
using namespace cimg_library;
#define NOBEE_

//-------------------------------------------------------------------------++
namespace all { namespace gaze { 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
gaze_machine_t::gaze_machine_t():nsamples_(0),running_(true)
{
  process_gaze_data = boost::bind(&gaze_machine_t::show_gaze_, this);
  print_welcome();
}
/////////////////////////////////////////////////////////////////////////////
gaze_machine_t::~gaze_machine_t()
{

}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::log_enabled(bool enabled)
{
  if(enabled)
    process_gaze_data = boost::bind(&gaze_machine_t::write_gaze_, this);
  else
    process_gaze_data = boost::bind(&gaze_machine_t::show_gaze_, this);
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
    std::string filelog = config.get<std::string>("config.binfile", "gazelog.bin");
    std::string mticonf = config.get<std::string>("config.mti", "config/mti_config.ini");
    std::string eyeconf = config.get<std::string>("config.reye","config/eyecamera.ini");
    std::string beeconf = config.get<std::string>("config.bee", "config/bumblebeeB.ini");
    //    
    msecspause          = config.get<unsigned int>("config.msecspause", 50);

    ////
    std::cout << "<Eye Camera ...>"; 
    printf("Config: %s\n", eyeconf.c_str());
    eye_.reset(new sense::opencv_grabber_t);
    if( !eye_->open(eyeconf) ) return false;
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<MTi......>"; 
    mti_.reset(new sense::MTi_driver_t());
    if( !mti_->open(mticonf) ) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

#ifndef NOBEE_
    //
    std::cout << "<Bumblebee ...>";
    bee_.reset(new sense::bumblebee_driver_t);
    if( !bee_->open(beeconf) ) return false;
    std::cout << "<Done>" << std::endl<< std::endl;
#endif

    //
    gazelog_.open(filelog.c_str(),std::ios::out|std::ios::binary);
    
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

  //skip  ... this position will held the number of frames.
  gazelog_.seekp(sample_sz);

  gazelog_.write((char*)&elapsed_sz, sizeof(size_t)); 
  gazelog_.write((char*)&eye_sz    , sizeof(size_t));

#ifndef NOBEE_
  gazelog_.write((char*)&scene_sz  ,   sizeof(size_t));
  gazelog_.write((char*)&depth_sz  ,   sizeof(size_t));
#endif

  gazelog_.write((char*)&head_sz  ,   sizeof(size_t));
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
  mti_->reset(sense::tags::align_reset);
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
//-------------------------------------------------------------------------++
void gaze_machine_t::write_gaze_()
{
  //nsamples_++;

  gazelog_.write((char*)&elapsed_,      elapsed_sz); 
  gazelog_.write((char*)ieye.get()  ,   eye_sz);
#ifndef NOBEE_
  gazelog_.write((char*)iscene.get(),   scene_sz);
  gazelog_.write((char*)idepth.get(),   depth_sz);
#endif
  gazelog_.write((char*)&ihead,         head_sz);
}
//-------------------------------------------------------------------------++
void gaze_machine_t::show_gaze_()
{
  //nsamples_++;
}
//-------------------------------------------------------------------------++
int gaze_machine_t::nsamples() const
{
return nsamples_;
}
//-------------------------------------------------------------------------++
void gaze_machine_t::run_machine()
{
  thread_ptr.reset( new boost::thread(boost::bind(&gaze_machine_t::gaze_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine_t::gaze_loop()
{
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  {  
    CImgDisplay view (  eye_->width(),  eye_->height(), "Camera");
    CImg<core::uint8_t> imag;
    reset_mti();
    start_timing();
    printf("->machine booted ...starting loop\n");
    const unsigned char color  [3] = {215,  240,  60};
    const unsigned char blue   [3] = {0,  0,  255};
    while (running_)
    {
        nsamples_++;
        sample_gaze_();
        process_gaze_data();
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

        boost::thread::yield();
        all::core::BOOST_SLEEP(msecspause);
    }
    printf("Thread Canceled\n");
    elapsed_ = elapsed();
    //
    gazelog_.seekp(std::ios::beg);
    //
    gazelog_.write((char*)&nsamples_, sizeof(size_t)); 

    //
    gazelog_.close(); 
  }
  else
      printf("devices not started!\n"); 
}
//-------------------------------------------------------------------------++
/////////////////////////////////////////////////////////////////////////////
}}///all::sense::detail
