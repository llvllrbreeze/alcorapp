#include "gaze_machine_t.h"
#include "alcor/core/config_parser_t.hpp"
//-------------------------------------------------------------------------++
namespace all { namespace gaze { 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
gaze_machine_t::gaze_machine_t():nsamples_(0)
{
    //using namespace boost::posix_time;
    ////get the current time from the clock -- one second resolution
    //ptime now = second_clock::local_time();
    binstream_.reset(::fopen("gaze_log.bin","wb"));
}
/////////////////////////////////////////////////////////////////////////////
gaze_machine_t::~gaze_machine_t()
{
}
/////////////////////////////////////////////////////////////////////////////
bool gaze_machine_t::start_machine()
{

std::cout << std::endl
<< " _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____ " << std::endl
<< "|  |  |     |   __|   __|  | | | |     |   --|     |-   -| | | |   __|" << std::endl
<< "|_____|__|__|_____|_____|  |_|_|_|__|__|_____|__|__|_____|_|___|_____|" << std::endl
    <<std::endl;

    std::cout << "<<STARTING MACHINE>>" << std::endl;
    //
    //iniWrapper gazeini;
    //gazeini.Load("config/gaze_machine.ini");

    //char* mticonf = gazeini.GetStringAsChar("config:mti","config/mti_config.ini");
    //char* eyeconf = gazeini.GetStringAsChar("config:reye","config/cmoscam.ini");
    //char* beeconf = gazeini.GetStringAsChar("config:bee","config/bumblebeeB.ini");

    all::core::config_parser_t config;
    config.load(core::tags::ini,"config/gaze_machine.ini");

    std::string mticonf   = config.get<std::string>("config.mti","config/mti_config.ini");
    std::string eyeconf   = config.get<std::string>("config.reye","config/cmoscam.ini");
    std::string beeconf   = config.get<std::string>("config.bee","config/bumblebeeB.ini");

    ////
    std::cout << "<Eye Camera ...>"; 
    eye_.reset(new sense::opencv_grabber_t);
    if( !eye_->open(eyeconf) ) return false;
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<MTi......>"; 
    mti_.reset(new sense::MTi_driver_t());
    if( !mti_->open("ddd") ) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<Bumblebee ...>";
    bee_.reset(new sense::bumblebee_driver_t);
    if( !bee_->open(beeconf) ) return false;
    std::cout << "<Done>" << std::endl<< std::endl;

    allocate_();
    
    write_header_();

    timer_.restart();
    return true;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::allocate_()
{
  ieye.reset  ( new core::uint8_t  [eye_->size()]              );
  iscene.reset( new core::uint8_t  [bee_->color_buffer_size()] );
  idepth.reset( new core::single_t [bee_->depth_buffer_size()] );
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::write_header_()
{
  elapsed_sz = sizeof(double);
  eye_sz     = eye_->size();
  scene_sz   = bee_->color_buffer_size();
  depth_sz   = bee_->depth_buffer_size();
  head_sz    = sizeof(math::rpy_angle_t);

  ::fwrite(&elapsed_sz,sizeof(elapsed_sz),1,binstream_.get());
  ::fwrite(&eye_sz,sizeof(eye_sz),1,binstream_.get());
  ::fwrite(&scene_sz,sizeof(scene_sz),1,binstream_.get()); 
  ::fwrite(&depth_sz,sizeof(depth_sz),1,binstream_.get());
  ::fwrite(&head_sz,sizeof(head_sz),1,binstream_.get());
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::print_welcome()
{
std::cout << "Library Ver: " <<  core::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  core::AUTHOR_INFORMATION << std::endl;
std::cout << "GAZE MACHINE Ver: " << gaze::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  gaze::AUTHOR_INFORMATION << std::endl;

std::cout << std::endl
<< "          ,-~-. _.---._ ,-~-. " << std::endl
<< "         / .- ,'       `. -.\\ " << std::endl
<< "        \\ ` /`        '\\ ' / " << std::endl
<< "          `-/   'a___a`  \\-'" << std::endl
<< "            |   ,'(_)`.   | " << std::endl
<< "           \\  ( ._|_. )  / " << std::endl
<< "            \\  `.___,'  /" << std::endl
<< "            .-`._     _,'-." << std::endl
<< "          ,'  ,' `---' `.  `." << std::endl
<< "         /   /     :    \\  \\ " << std::endl
<< "       ,'   /      :     \\   `." << std::endl
<< "     ,'     |      :      |     `." << std::endl
<< "    |     ,'|      :      |`.     |" << std::endl
<< "    `.__,' .-\\     :    /-. `.__,'" << std::endl
<< "          /   `.   :   ,'  \\ " << std::endl
<< "   .""-.,'      `._:_,'      `.,-"". " << std::endl
<< "  / ,-. `         ) (         ' ,-.\\ " << std::endl
<< " ( (   `.       ,'   `.       ,'   ) )" << std::endl
<< " \\\\   \\   _,'       `._   /    / / " << std::endl
<< "   `.`._,'  /            \\  `._,',' " << std::endl
<< "     `.__.-'               `-.__,' " << std::endl << std::endl;

}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_t::sample_gaze()
{  
  ++nsamples_;

  bee_->grab();
  eye_->get_color_buffer(ieye);
  ihead = mti_->get_euler();
  double elapsed = timer_.elapsed();  

  //this ones takes longer  
  //but the images have been already grabbed
  bee_->get_color_buffer(core::right_img, iscene);  
  bee_->get_depth_buffer(idepth);
  
  ::fwrite(&elapsed     , elapsed_sz, 1, binstream_.get());
  ::fwrite(ieye.get()   , eye_sz    , 1, binstream_.get());
  ::fwrite(iscene.get() , scene_sz  , 1, binstream_.get());
  ::fwrite(idepth.get() , depth_sz  , 1, binstream_.get());
  ::fwrite(&ihead       , head_sz   , 1, binstream_.get());
}
//-------------------------------------------------------------------------++
int gaze_machine_t::nsamples() const
{
return nsamples_;
}
//-------------------------------------------------------------------------++
/////////////////////////////////////////////////////////////////////////////
}}///all::sense::detail
