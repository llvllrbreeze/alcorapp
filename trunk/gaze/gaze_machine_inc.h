#ifndef gaze_machine_inc_H_INC
#define gaze_machine_inc_H_INC

#define WIN32_LEAN_AND_MEAN

//-------------------------------------------------------------------------++
#include "alcor/sense/opencv_grabber_t.h"
#include "alcor/sense/bumblebee_driver_t.h"
#include "alcor/sense/MTi_driver_t.h"
//-------------------------------------------------------------------------++
#include <boost/shared_array.hpp>
#include <boost/timer.hpp>
//-------------------------------------------------------------------------++
#include <iostream>
#include <vector>
#include <cstdio>
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
const char VERSION_INFORMATION[] = "Gaze Machine 0.5.0";
const char AUTHOR_INFORMATION[]  = "Andrea Carbone";
}}

#endif //gaze_machine_inc_H_INC