#include "..\gaze_machine_t.h"
#include "..\gaze_machine_inc.h"
//-------------------------------------------------------------------------++
#include "alcor/core/iniWrapper.h"
//-------------------------------------------------------------------------++
#include <cstdio>
#include <boost/shared_array.hpp>
#include <boost/timer.hpp>
//-------------------------------------------------------------------------++
namespace all { namespace gaze { namespace detail {
//-------------------------------------------------------------------------++
class gaze_machine_impl: public gaze_machine_t
{
private:
  //ctor -- not copy constructible
  gaze_machine_impl(gaze_machine_impl const &);
  //not copyable
  gaze_machine_impl& operator=(gaze_machine_impl const &);

public:
  ///
  gaze_machine_impl();
  ///
  ~gaze_machine_impl();
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
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
inline gaze_machine_impl::gaze_machine_impl():nsamples_(0)
{
    //using namespace boost::posix_time;
    ////get the current time from the clock -- one second resolution
    //ptime now = second_clock::local_time();
    binstream_.reset(::fopen("gaze_log.bin","wb"));
}
/////////////////////////////////////////////////////////////////////////////
gaze_machine_impl::~gaze_machine_impl()
{
}
/////////////////////////////////////////////////////////////////////////////
bool gaze_machine_impl::start_machine()
{

std::cout << std::endl
<< " _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____ " << std::endl
<< "|  |  |     |   __|   __|  | | | |     |   --|     |-   -| | | |   __|" << std::endl
<< "|_____|__|__|_____|_____|  |_|_|_|__|__|_____|__|__|_____|_|___|_____|" << std::endl
    <<std::endl;

    std::cout << "<<STARTING MACHINE>>" << std::endl;
    //
    iniWrapper gazeini;
    gazeini.Load("config/gaze_machine.ini");

    char* mticonf = gazeini.GetStringAsChar("config:mti","config/mti_config.ini");
    char* eyeconf = gazeini.GetStringAsChar("config:reye","config/cmoscam.ini");
    char* beeconf = gazeini.GetStringAsChar("config:bee","config/bumblebeeB.ini");

    ////
    std::cout << "<Eye Camera ...>"; 
    eye_.reset(new sense::opencv_grabber_t);
    if( !eye_->open(std::string(eyeconf) )) return false;
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<MTi......>"; 
    mti_.reset(new sense::MTi_driver_t());
    if( !mti_->open(std::string(mticonf) ) ) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<Bumblebee ...>";
    bee_.reset(new sense::bumblebee_driver_t);
    if( !bee_->open(std::string(beeconf)) ) return false;
    std::cout << "<Done>" << std::endl<< std::endl;

    allocate_();
    
    write_header_();

    timer_.restart();
    return true;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_impl::allocate_()
{
  ieye.reset  ( new core::uint8_t  [eye_->size()]              );
  iscene.reset( new core::uint8_t  [bee_->color_buffer_size()] );
  idepth.reset( new core::single_t [bee_->depth_buffer_size()] );
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_impl::write_header_()
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
void gaze_machine_impl::print_welcome()
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
void gaze_machine_impl::sample_gaze()
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
int gaze_machine_impl::nsamples() const
{
return nsamples_;
}

//-------------------------------------------------------------------------++
/////////////////////////////////////////////////////////////////////////////
}}}///all::sense::detail
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
gaze_machine_ptr create_gaze_machine()
{
    boost::shared_ptr<all::gaze::gaze_machine_t> gaze_ptr 
        (reinterpret_cast<all::gaze::gaze_machine_t*>
        ( new all::gaze::detail::gaze_machine_impl) );
    return gaze_ptr;
}
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++