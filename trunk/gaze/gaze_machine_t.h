#ifndef gaze_machine_t_H_INCLUDED
#define gaze_machine_t_H_INCLUDED
//-------------------------------------------------------------------------++
#pragma comment(lib, "gaze_machine_library.lib")
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
#include "alcor/sense/shared_mt9_data_t.h"
#include "alcor/core/image_info_t.h"
//-------------------------------------------------------------------------++
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
//-------------------------------------------------------------------------++
namespace all { namespace sense{
///
class gaze_machine_t;
}}
//-------------------------------------------------------------------------++
///
class all::sense::gaze_machine_t
{
public:
    ///
    virtual bool start_machine()=0;
    ///
    virtual void print_welcome()=0;

//private:
    ///MT9B
    virtual bool get_mt9(mt9_data_t&)=0;
    ///
    //virtual sp_mt9_data_t get_mt9()=0;

    ///EYE CAMERA
    //virtual bool get_eye_image(core::uint8_t::ptr)=0;
    ///
    //virtual all::core::uint8_t::ptr get_eye_image()=0;

    ///COLOR CAMERA (bumblebee)
    ///
public:
    virtual void sample_gaze()=0;

    virtual int  nsamples() const=0;
};
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
///
boost::shared_ptr<all::sense::gaze_machine_t> 
    create_gaze_machine();

typedef boost::shared_ptr<all::sense::gaze_machine_t> sp_gaze_t;
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
#endif //gaze_machine_t_H_INCLUDED