#ifndef gaze_machine_inc_H_INC
#define gaze_machine_inc_H_INC

#define WIN32_LEAN_AND_MEAN

//-------------------------------------------------------------------------++
#include "alcor/sense/opencv_grabber_t.h"
#include "alcor/sense/bumblebee_driver_t.h"
#include "alcor/sense/MTi_driver_t.h"
//-------------------------------------------------------------------------++
#include <boost/timer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
//#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------------++
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
//-------------------------------------------------------------------------++
#pragma comment (lib, "MTi_driver_t.lib")
#pragma comment (lib, "bumblebee_driver_t.lib")
#pragma comment (lib, "opencv_grabber_t.lib")
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
const char VERSION_INFORMATION[] = "Gaze Machine 0.5.5";
const char AUTHOR_INFORMATION[]  = "Andrea Carbone";
}}

#endif //gaze_machine_inc_H_INC