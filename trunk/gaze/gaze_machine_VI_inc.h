#ifndef gaze_machine_VI_inc_H_INC
#define gaze_machine_VI_inc_H_INC

#define WIN32_LEAN_AND_MEAN

//-------------------------------------------------------------------------++
#include "alcor.extern/videoinput/videoinput.h"
#include <cv.h>
#include <highgui.h>
//-------------------------------------------------------------------------++
#include "alcor/sense/MTi_driver_t.h"
#include "alcor/core/jpeg_encoder_t.h"
//-------------------------------------------------------------------------++
#include <boost/timer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
//-------------------------------------------------------------------------++
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
//-------------------------------------------------------------------------++
struct calib_t {};
struct binlog_t{};
struct show_t{};
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
static const calib_t  calib   = calib_t();
static const binlog_t binlog  = binlog_t();
static const show_t   show    = show_t();
}}
//-------------------------------------------------------------------------++
#pragma comment (lib, "highgui.lib")
#pragma comment (lib, "cv.lib")
#pragma comment (lib, "cxcore.lib")
//NOTA! messa in alcor.lib
#pragma comment(lib, "videoInput.lib")

#pragma comment (lib, "MTi_driver_t.lib")
#pragma comment (lib, "libmat.lib")
#pragma comment (lib, "libmx.lib")
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
const char VERSION_INFORMATION[] = "Gaze Machine VI";
const char AUTHOR_INFORMATION[]  = "Andrea Carbone & Stefano Marra";
}}

#endif //gaze_machine_VI_inc_H_INC