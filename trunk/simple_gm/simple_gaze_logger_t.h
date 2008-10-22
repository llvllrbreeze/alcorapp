#ifndef SIMPLE_GAZE_LOGGER_H_INCLUDED_
#define SIMPLE_GAZE_LOGGER_H_INCLUDED_
//-------------------------------------------------------------------------++
#include "alcor.extern/videoinput/include/videoinput.h"
#include <cv.h>
#include <highgui.h>
//-------------------------------------------------------------------------++
#include "alcor/sense/MTi_driver_t.h"
//-------------------------------------------------------------------------++
#include <boost/timer.hpp>
#include <boost/thread/thread.hpp>
//-------------------------------------------------------------------------++
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <memory>
//-------------------------------------------------------------------------++
#pragma comment (lib, "highgui.lib")
#pragma comment (lib, "cv.lib")
#pragma comment (lib, "cxcore.lib")
#pragma comment(lib, "Vfw32.Lib")
//NOTA! messa in alcor.lib
#pragma comment (lib,  "videoInput.lib")
#pragma comment (lib, "MTi_driver_t.lib")
//-------------------------------------------------------------------------++

namespace uor{

class simple_gaze_logger_t
{
	//[]
public:
	///
	simple_gaze_logger_t();
	///
	~simple_gaze_logger_t();
	///
	void mainloop();
	///
	void cancel();

private:
	///
	bool bootstrap_();
	///
	void clean_();
	///
	void loop_();
	///
	void sample_gaze_();
	///
	void log_gaze_();
	///
	void show_gaze_();

private:
	///
	std::auto_ptr<videoInput> VI_;
	///
	all::sense::MTi_driver_ptr  mti_;
	///
	int camera_id_;

	 ///rgb scene buffer
	IplImage*   ipl_scene_img_;
	///
	CvVideoWriter* video_;

	///Orientation
	all::math::rpy_angle_t  heading_;

	///Accelerations [ax ay az]
	float acc_[3];

	///orientation matrix
	float dcmgs_[9];

	// 
	double elapsed_;

	 ///Binary Data Stream
	std::fstream gazelog_;
	///
	std::string logname_;

	///
	boost::timer timer_;
	///loop control
	volatile bool running_;
	///the MAIN thread!
	boost::shared_ptr<boost::thread> thread_ptr_;
};


}//closes namespace uor

#endif //SIMPLE_GAZE_LOGGER_H_INCLUDED_