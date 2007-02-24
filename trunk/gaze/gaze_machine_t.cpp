#include "gaze_machine_t.h"
#include "gaze_machine_inc.h"
//-------------------------------------------------------------------------++
#include <cstdio>
#include <boost/shared_array.hpp>
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

private:
    ///Eye Camera
  all::sense::opencv_grabber_ptr    eye_;
  ///
  all::sense::MTi_driver_ptr        mti_;
  ///Scene Camera
  all::sense::bumblebee_sptr        bee_; 

  ///allocate enough space .. 
  void allocate_();

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
    ////
    std::cout << "<Eye Camera ...>"; 
    eye_.reset(new sense::opencv_grabber_t);
    if(!eye_->open(core::open_camera)) return false;
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<MTi......>"; 
    mti_.reset(new sense::MTi_driver_t);
    if(!mti_->open("config/mti_config.ini")) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    std::cout << "<Bumblebee ...>";
    bee_.reset(new sense::bumblebee_driver_t);
    if( !bee_->open("config/bumblebeeB.ini") ) return false;
    std::cout << "<Done>" << std::endl<< std::endl;

    allocate_();

    return true;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_impl::allocate_()
{
  ieye.reset  ( new core::uint8_t  [eye_->size()]                  );
  iscene.reset( new core::uint8_t  [bee_->nrows()*bee_->ncols()*3] );
  idepth.reset( new core::single_t [bee_->nrows()*bee_->ncols()*3] );
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

    //core::uint8_sarr eye_ptr =  
    //        eye_.get_internal_buffer();

    //eye_.get_image_buffer2(eye_ptr);

    //sense::mt9_data_t temp_mt9;
    //get_mt9(temp_mt9);
            
    //boost::shared_array<core::uint8_t> scene_ptr
    //    = bee_.get_internal_color(sense::right_cam);
    
    //boost::shared_array<core::single_t> depth_ptr
    //    = bee_.get_internal_depth();

    //::fwrite(eye_ptr,    eye_.get_size(),    1,       binstream_.get());
    //::fwrite(scene_ptr.get(),  bee_.memsize(),     1, binstream_.get());
    //::fwrite(depth_ptr.get(), (bee_.memsize()*core::traits<core::single_t>::size), 1, binstream_.get());
    //::fwrite(&temp_mt9,  sizeof(temp_mt9),   1, binstream_.get());
}
//-------------------------------------------------------------------------++
int gaze_machine_impl::nsamples() const
{
return nsamples_;
}

/////////////////////////////////////////////////////////////////////////////
}}}///all::sense::detail
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
inline boost::shared_ptr<all::gaze::gaze_machine_t> 
    create_gaze_machine()
{
    boost::shared_ptr<all::gaze::gaze_machine_t> gaze_ptr 
        (reinterpret_cast<all::gaze::gaze_machine_t*>
        ( new all::gaze::detail::gaze_machine_impl) );
    return gaze_ptr;
}
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++