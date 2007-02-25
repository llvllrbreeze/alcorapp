#ifndef gaze_machine_t_H_INCLUDED
#define gaze_machine_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
//-------------------------------------------------------------------------++
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
//-------------------------------------------------------------------------++
namespace all { namespace gaze{
///
struct gaze_machine_t;
typedef boost::shared_ptr<gaze_machine_t> gaze_machine_ptr;
}}
//-------------------------------------------------------------------------++
///
struct all::gaze::gaze_machine_t
{
public:
    ///
    virtual bool start_machine()=0;
    ///
    virtual void print_welcome()=0;
    ///
    virtual void sample_gaze()=0;
    ///
    virtual int  nsamples() const=0;
    ///
    virtual void  start_timing()= 0;
    ///
    virtual double elapsed()= 0;
};
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
///
gaze_machine_ptr  create_gaze_machine();

//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
#endif //gaze_machine_t_H_INCLUDED