#pragma once //trm_app_inc.h
//-----------------------------------------------------------------------++
#include "alcor/act/p3_gateway.h"
//#include "alcor/act/p3_odometry_adapter_t.h"
#include "alcor/act/directed_perception_ptu_t.h"
//#include "alcor/act/pantilt_control_loop_t.hpp"
#include "alcor/sense/bumblebee_driver_t.h"
//-----------------------------------------------------------------------++
///experimental
#include "alcor/core/pixel_traits.h"
#include "alcor/core/depth_image_utils.hpp"
//-----------------------------------------------------------------------++
#include "alcor.extern/CImg/CImg.h"
namespace cimglib=cimg_library;
//-----------------------------------------------------------------------++
#include "alcor/matlab/matlab_engine_t.h"
//-----------------------------------------------------------------------++
#include "alcor/math/pinhole_utils.hpp"
//-----------------------------------------------------------------------++
#include "task_listener.h"
//-----------------------------------------------------------------------++
#include "alcor/core/memory_stream_source_t.hpp"
#include <alcor/core/stream_server_t.hpp>
//-----------------------------------------------------------------------++