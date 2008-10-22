#include "simple_gaze_logger_t.h"

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>  // includes all needed Boost.Filesystem declarations
 

namespace uor{
	//
	simple_gaze_logger_t::simple_gaze_logger_t():
	 running_(true)
	,ipl_scene_img_(0)
	,camera_id_(0)
	,logname_("headingLog.dat")
	{

	}

	simple_gaze_logger_t::~simple_gaze_logger_t()
	{
		if (ipl_scene_img_)  cvReleaseImage(&ipl_scene_img_);
		if (video_)          cvReleaseVideoWriter(&video_);
	}

	//
	void simple_gaze_logger_t::mainloop()
	{
		thread_ptr_.reset( 
			new boost::thread(boost::bind(&simple_gaze_logger_t::loop_, this) ) 
			);
	}

	//
	void simple_gaze_logger_t::cancel()
	{
		running_ = false;
	}

	//
	void simple_gaze_logger_t::loop_()
	{
		if(bootstrap_()){ 
			//
			cvNamedWindow("SCENE");
			int nsamples = 0;
					//
			timer_.restart();
			//entering the loop
			while (running_)
			{				
				nsamples ++;

				sample_gaze_();
				log_gaze_();
				show_gaze_();


				boost::thread::yield();
			}
			printf("\nRATE: %.2f\n", nsamples/timer_.elapsed());
			//tidy your room now
		    clean_();
		}
			
	}

	//
	bool simple_gaze_logger_t::bootstrap_()
	{		
		namespace fs = boost::filesystem;
		fs::path outfolder = "./samples";
		
		//
		if (!fs::exists(outfolder) )
		{
			std::cout 
				<< "Creating Directory: "
				<< outfolder
				<< std::endl;

			fs::create_directory(outfolder);
		}

		//CAMERA
		VI_.reset(new videoInput());

		if (!VI_->setupDevice(camera_id_, 640, 480))
		{
			std::cout 
				<< "Cannot SetUp camera ID #" << camera_id_
				<< std::endl;

			return false;
		}
		//IMAGE
		ipl_scene_img_ = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);

		video_  = cvCreateVideoWriter 
				("scene.avi", -1, 15, cvSize( 640, 480));

		std::cout 
			<< "Opening MTi ......"
			<< std::endl;

		//MTi
		mti_.reset(new all::sense::MTi_driver_t());

		//
		if( !mti_->open("mti_conf.ini") ) return false;
		
		std::cout 
			<< "Opening Log File ......"
			<< std::endl;
		
		//output File
		gazelog_.open(logname_.c_str(),std::ios::out);

		return true;
	}

	//
	void simple_gaze_logger_t::clean_()
	{
		gazelog_.close();

		if (video_) cvReleaseVideoWriter(&video_);
		video_ = 0; 
	}

	//
	void simple_gaze_logger_t::sample_gaze_()
	{
		if (VI_->isFrameNew( camera_id_ ) ){
			//sample time
			elapsed_ = timer_.elapsed();
			//sample image
			VI_->getPixels(camera_id_, reinterpret_cast <unsigned char*> (ipl_scene_img_->imageData), false);
			
			if (mti_->req_data())
			{
				//-
				//heading_ = mti_->get_euler();
				mti_->get_acc(acc_);
				mti_->get_dcm(dcmgs_);
			}

		}
	}

	//
	void simple_gaze_logger_t::log_gaze_()
	{
		 gazelog_ 
			 << elapsed_ << " " 
			 //<< heading_.roll.deg()  << " " 
			 //<< heading_.pitch.deg()  << " " 
			 //<< heading_.yaw.deg()  << " "

			 << dcmgs_[0]   << " " 
			 << dcmgs_[1]   << " " 
			 << dcmgs_[2]   << " "
			 << dcmgs_[3]   << " "
			 << dcmgs_[4]   << " "
			 << dcmgs_[5]   << " "
			 << dcmgs_[6]   << " "
			 << dcmgs_[7]   << " "
			 << dcmgs_[8]   << " "

			 << acc_[0] << " "
			 << acc_[1] << " "
			 << acc_[2]  
			 << std::endl;

		 //std::cout 
			// << heading_.roll.deg()   << " "
			// << heading_.pitch.deg()  << " " 
			// << heading_.yaw.deg() 
			// << std::endl;

		 cvWriteFrame(video_, ipl_scene_img_);
 	}

	void simple_gaze_logger_t::show_gaze_()
	{
	 cvShowImage("SCENE", ipl_scene_img_ );
	 cvWaitKey(1);
	}

}//namespace