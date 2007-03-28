#ifndef xrp_app_inc_H_INCLUDED
#define xrp_app_inc_H_INCLUDED 
//-----------------------------------------------------------------------++
#include "alcor/act/p3_gateway.h"
#include "alcor/act/p3_server_t.h"
#include "alcor/act/directed_perception_ptu_t.h"
#include "alcor/sense/bumblebee_driver_t.h"
////-----------------------------------------------------------------------++
/////experimental
//#include "alcor/core/pixel_traits.h"
//#include "alcor/core/depth_image_utils.hpp"
//-----------------------------------------------------------------------++
//#include "alcor.extern/CImg/CImg.h"
//namespace cimglib=cimg_library;
//-----------------------------------------------------------------------++
#include "alcor/splam/splam_client.h"
//-----------------------------------------------------------------------++
#include "alcor/matlab/matlab_engine_t.h"
//-----------------------------------------------------------------------++
#include "alcor/math/pinhole_utils.hpp"
//-----------------------------------------------------------------------++
#include "task_listener.h"
//-----------------------------------------------------------------------++
#pragma comment (lib, "MTi_driver_t.lib")
#pragma comment (lib, "bumblebee_driver_t.lib")
#pragma comment (lib, "splam_client.lib")
#pragma comment (lib, "splam_data.lib")
#pragma comment (lib, "coder.lib")
#pragma comment (lib, "p3_client_server.lib")
//-----------------------------------------------------------------------++
#endif
