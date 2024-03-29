
#include "gaze_machine_VI_t.h"
//-------------------------------------------------------------------------++
#include "alcor/core/config_parser_t.hpp"
#include "alcor/matlab/matlab_mx_utils.hpp"
#include "alcor/core/image_utils.h"
#include "alcor/core/opencv_utils.hpp"
//#include "alcor/sense/stereo_process_t.hpp"
//-------------------------------------------------------------------------++
//display
#include "alcor.extern/CImg/CImg.h"
using namespace cimg_library;
#define TIMERDBG
//-------------------------------------------------------------------------++
#include <boost/lexical_cast.hpp>
//matlab
#include "mat.h"
//-------------------------------------------------------------------------++
namespace all { namespace gaze { 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
gaze_machine_VI_t::gaze_machine_VI_t():
                        
						nsamples_(0)
                      , calib_samples_cnt_(0)
                      , running_(true)
                      , logname_("gazelog.bin")
                      , bsavecalib_(false)
{
  create_devices_();

  print_welcome();
}
/////////////////////////////////////////////////////////////////////////////
gaze_machine_VI_t::~gaze_machine_VI_t()
{
	delete VI;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_VI_t::set_logname(std::string& logname)
{
  logname_ = logname;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_VI_t::create_devices_()
{

	int numDevices = videoInput::listDevices();
  
  
  //
  mti_.reset(new sense::MTi_driver_t());
}
/////////////////////////////////////////////////////////////////////////////
bool gaze_machine_VI_t::boot_machine_()
{

//std::cout << std::endl
//<< " _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____ " << std::endl
//<< "|  |  |     |   __|   __|  | | | |     |   --|     |-   -| | | |   __|" << std::endl
//<< "|_____|__|__|_____|_____|  |_|_|_|__|__|_____|__|__|_____|_|___|_____|" << std::endl
//    <<std::endl;

    std::cout << "<<STARTING MACHINE>>" << std::endl;


    all::core::config_parser_t config;
    config.load(core::ini,"config/gaze_machine.ini");

    //
    //logname_ = config.get<std::string>("config.binfile", "gazelog.bin");
    std::string mticonf     = config.get<std::string>("config.mti", "config/mti_config.ini");
    std::string cameraconf  = config.get<std::string>("config.gazecamera","config/gazecamera.ini");
    //
    calib_samples_cnt_   = config.get<int>("config.calibcount", 9);
    //    
    msecspause_          = config.get<unsigned int>("config.msecspause", 1);
    ///
    std::cout << "msec loop pause: " << msecspause_ << std::endl;

	VI = new videoInput();

	all::core::config_parser_t camera_config;
	camera_config.load(core::ini, cameraconf);
	eye_[left] = camera_config.get<int>("eyeleft.device", 0);
	eye_[right] = camera_config.get<int>("eyeright.device", 1);
	scene_[left] = camera_config.get<int>("sceneleft.device", 2);
	scene_[right] = camera_config.get<int>("sceneright.device", 3);

	int width = camera_config.get<int>("eyeleft.width", 640);
	int height = camera_config.get<int>("eyeleft.height",480);

	if (!VI->setupDevice(eye_[left], width, height))
		return false;
//	VI->showSettingsWindow(eye_[left]);
	if (!VI->setupDevice(eye_[right], width, height))
		return false;
//	VI->showSettingsWindow(eye_[right]);

	width = camera_config.get<int>("sceneleft.width", 640);
	height = camera_config.get<int>("sceneleft.height",480);
    if (!VI->setupDevice(scene_[left], width, height))
		return false;
	//VI->showSettingsWindow(scene_[left]);
	if (!VI->setupDevice(scene_[right], width, height))
		return false;
	//VI->showSettingsWindow(scene_[right]);

	//VI->setFormat(eye_[left], VI_NTSC_M_J);
	//VI->setFormat(eye_[right], VI_NTSC_M_J);
	//VI->setFormat(scene_[left], VI_NTSC_M_J);
	//VI->setFormat(scene_[right], VI_NTSC_M_J);

    //
    std::cout << "<MTi......>"; 

    if( !mti_->open(mticonf) ) return false;    
    std::cout << "<Done>"<< std::endl << std::endl;

    //
    get_dims_();

	ipl_eye_img_[left] = cvCreateImage(cvSize(eyedims_[left].col_, eyedims_[left].row_), IPL_DEPTH_8U, 3);
    ipl_eye_img_[right] = cvCreateImage(cvSize(eyedims_[right].col_, eyedims_[right].row_), IPL_DEPTH_8U, 3);
    ipl_scene_img_[left] = cvCreateImage(cvSize(scenedims_[left].col_, scenedims_[left].row_), IPL_DEPTH_8U, 3);
    ipl_scene_img_[right] = cvCreateImage(cvSize(scenedims_[right].col_, scenedims_[right].row_), IPL_DEPTH_8U, 3);

	//for bw conversion of eye images
	ipl_eye_bw_img_[left] = cvCreateImage(cvSize(eyedims_[left].col_, eyedims_[left].row_), IPL_DEPTH_8U, 1);
	ipl_eye_bw_img_[right] = cvCreateImage(cvSize(eyedims_[right].col_, eyedims_[right].row_), IPL_DEPTH_8U, 1);
	eye_bw_sz_[left] = ipl_eye_bw_img_[left]->imageSize;
	eye_bw_sz_[right] = ipl_eye_bw_img_[right]->imageSize;

	ipl_scene_rgb_img_[left] = cvCreateImage(cvSize(scenedims_[left].col_, scenedims_[left].row_), IPL_DEPTH_8U, 3);
	ipl_scene_rgb_img_[right] = cvCreateImage(cvSize(scenedims_[right].col_, scenedims_[right].row_), IPL_DEPTH_8U, 3);

	eye_sz_[left]  = ipl_eye_img_[left]->imageSize;
	eye_sz_[right] = ipl_eye_img_[right]->imageSize;

	//
	scene_sz_[left] = ipl_scene_img_[left]->imageSize;
	scene_sz_[right] = ipl_scene_img_[right]->imageSize;
	
	depth_sz_ = scenedims_[left].col_ * scenedims_[right].row_ * sizeof(all::core::single_t);

    timer_.restart();

    return true;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_VI_t::print_welcome()
{
std::cout << "Library Ver: " <<  core::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  core::AUTHOR_INFORMATION << std::endl;
std::cout << "GAZE MACHINE Ver: " << gaze::VERSION_INFORMATION << std::endl;
std::cout << "Author: " <<  gaze::AUTHOR_INFORMATION << std::endl;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_VI_t::reset_mti()
{
  //mti_->reset(sense::tags::global_reset);
  //mti_->reset(sense::tags::heading_reset);
}
/////////////////////////////////////////////////////////////////////////////
bool gaze_machine_VI_t::sample_gaze_()
{  


  //actually get images.
  if (VI->isFrameNew(eye_[left]) && VI->isFrameNew(eye_[right]) && VI->isFrameNew(scene_[left]) && VI->isFrameNew(scene_[right])) {
	elapsed_ = timer_.elapsed();

	VI->getPixels(eye_[left], reinterpret_cast <unsigned char*> (ipl_eye_img_[left]->imageData), false);
	VI->getPixels(eye_[right], reinterpret_cast <unsigned char*> (ipl_eye_img_[right]->imageData), false);
	VI->getPixels(scene_[left], reinterpret_cast <unsigned char*> (ipl_scene_img_[left]->imageData), false);
	VI->getPixels(scene_[right], reinterpret_cast <unsigned char*> (ipl_scene_img_[right]->imageData), false);
	heading_ = mti_->get_euler();
	return true;
  }
  return false;

}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_VI_t::get_dims_()
{
  printf("Getting data dimension!\n"); 
  sample_sz_ = sizeof(nsamples_);
  
  elapsed_sz_ = sizeof(double);

  heading_sz_    = sizeof(math::rpy_angle_t);

  //eye dimensions
  eyedims_[left].row_   = VI->getHeight(eye_[left]);
  eyedims_[left].col_   = VI->getWidth(eye_[left]);
  eyedims_[left].depth_ = 1;

  //
  eyedims_[right].row_   = VI->getHeight(eye_[right]);
  eyedims_[right].col_   = VI->getWidth(eye_[right]);
  eyedims_[right].depth_ = 1;

  ///scene dimensions
  scenedims_[left].row_   = VI->getHeight(scene_[left]);
  scenedims_[left].col_   = VI->getWidth(scene_[left]);
  scenedims_[left].depth_ = 3;
  //
  scenedims_[right].row_   = VI->getHeight(scene_[right]);
  scenedims_[right].col_   = VI->getWidth(scene_[right]);
  scenedims_[right].depth_ = 3;
}
/////////////////////////////////////////////////////////////////////////////
void gaze_machine_VI_t::write_header_bin_()
{
  printf("Writing Header!\n");
  //skip  ... this position will hold the number of frames.
  gazelog_.seekp(sample_sz_);

  //skip .... this will hold the total time elapsed.
  gazelog_.seekp(sample_sz_ + elapsed_sz_);

  int log_type_ = 0;
  gazelog_.write((char*)&log_type_, sizeof(log_type_));

  gazelog_.write((char*)&eyedims_[left], sizeof(eyedims_[left]));
  gazelog_.write((char*)&eyedims_[right], sizeof(eyedims_[right]));

  gazelog_.write((char*)&scenedims_[left], sizeof(scenedims_[left]) );  
  gazelog_.write((char*)&scenedims_[right], sizeof(scenedims_[right]) );  

  gazelog_.write((char*)&elapsed_sz_, sizeof(size_t));

  gazelog_.write((char*)&eye_bw_sz_[left]  , sizeof(eye_bw_sz_[left]));
  gazelog_.write((char*)&eye_bw_sz_[right] , sizeof(eye_bw_sz_[right]));

  gazelog_.write((char*)&scene_sz_[left]  ,   sizeof(scene_sz_[left]  ));
  gazelog_.write((char*)&scene_sz_[right] ,   sizeof(scene_sz_[right] ));

  gazelog_.write((char*)&heading_sz_  ,   sizeof(heading_sz_));
}

void gaze_machine_VI_t::write_header_avi_() {
  
  printf("Writing Header!\n");
  //skip  ... this position will hold the number of frames.
  gazelog_.seekp(sample_sz_);

  //skip .... this will hold the total time elapsed.
  gazelog_.seekp(sample_sz_ + elapsed_sz_);

  int log_type_ = 1;
  gazelog_.write((char*)&log_type_, sizeof(log_type_));

  gazelog_.write((char*)&eyedims_[left], sizeof(eyedims_[left]));
  gazelog_.write((char*)&eyedims_[right], sizeof(eyedims_[right]));

  gazelog_.write((char*)&scenedims_[left], sizeof(scenedims_[left]) );  
  gazelog_.write((char*)&scenedims_[right], sizeof(scenedims_[right]) );  

  gazelog_.write((char*)&elapsed_sz_, sizeof(size_t));

  gazelog_.write((char*)&eye_bw_sz_[left]  , sizeof(eye_bw_sz_[left]));
  gazelog_.write((char*)&eye_bw_sz_[right] , sizeof(eye_bw_sz_[right]));

  gazelog_.write((char*)&scene_sz_[left]  ,   sizeof(scene_sz_[left]  ));
  gazelog_.write((char*)&scene_sz_[right] ,   sizeof(scene_sz_[right] ));

  gazelog_.write((char*)&heading_sz_  ,   sizeof(heading_sz_));

}
//-------------------------------------------------------------------------++
void gaze_machine_VI_t::write_gaze_bin_()
{
  //write time
  gazelog_.write((char*)&elapsed_,  elapsed_sz_); 

  //BW conversion
  cvCvtColor(ipl_eye_img_[left], ipl_eye_bw_img_[left], CV_BGR2GRAY);
  cvCvtColor(ipl_eye_img_[right], ipl_eye_bw_img_[right], CV_BGR2GRAY);

  //cvCvtColor(ipl_scene_img_[left], ipl_scene_rgb_img_[left], CV_BGR2RGB);
  //cvCvtColor(ipl_scene_img_[right], ipl_scene_rgb_img_[right], CV_BGR2RGB);


  //jpeg write
  memcpy(sarr_eye_img_[left].get(), ipl_eye_bw_img_[left]->imageData, eye_bw_sz_[left]);
  memcpy(sarr_eye_img_[right].get(), ipl_eye_bw_img_[right]->imageData, eye_bw_sz_[right]);
  memcpy(sarr_scene_img_[left].get(), ipl_scene_img_[left]->imageData, scene_sz_[left]);
  memcpy(sarr_scene_img_[right].get(), ipl_scene_img_[right]->imageData, scene_sz_[right]);


  gaze_jpeg_encoder_t::encode();


  gazelog_.write((char*)&jpeg_eye_data_[left].size, sizeof(std::size_t));
  gazelog_.write(reinterpret_cast<char*> (jpeg_eye_data_[left].data.get()), jpeg_eye_data_[left].size);
  gazelog_.write((char*)&jpeg_eye_data_[right].size, sizeof(std::size_t));
  gazelog_.write(reinterpret_cast<char*> (jpeg_eye_data_[right].data.get()), jpeg_eye_data_[right].size);
  gazelog_.write((char*)&jpeg_scene_data_[left].size, sizeof(std::size_t));
  gazelog_.write(reinterpret_cast<char*> (jpeg_scene_data_[left].data.get()), jpeg_scene_data_[left].size);
  gazelog_.write((char*)&jpeg_scene_data_[right].size, sizeof(std::size_t));
  gazelog_.write(reinterpret_cast<char*> (jpeg_scene_data_[right].data.get()), jpeg_scene_data_[right].size);

  //write mti
  gazelog_.write((char*)&heading_,   heading_sz_);

}


void gaze_machine_VI_t::write_gaze_avi_() {
  //write time
  gazelog_.write((char*)&elapsed_,  elapsed_sz_); 
  

  //BW conversion
  cvCvtColor(ipl_eye_img_[left], ipl_eye_bw_img_[left], CV_BGR2GRAY);
  cvCvtColor(ipl_eye_img_[right], ipl_eye_bw_img_[right], CV_BGR2GRAY);

  //avi write
  cvWriteFrame(eye_avi_[left], ipl_eye_bw_img_[left]);
  cvWriteFrame(eye_avi_[right], ipl_eye_bw_img_[right]);
  cvWriteFrame(scene_avi_[left], ipl_scene_img_[left]);
  cvWriteFrame(scene_avi_[right], ipl_scene_img_[right]);

  //write mti
  gazelog_.write((char*)&heading_,   heading_sz_);
}

//-------------------------------------------------------------------------++
void gaze_machine_VI_t::calib_loop()
{

	if (boot_machine_()) {
		
		start_timing();
		const int imagestosave = calib_samples_cnt_;
		int imagecnt = 0;

		cvNamedWindow("EyeLEFT");
		cvNamedWindow("EyeRIGHT");
		cvNamedWindow("SceneLEFT");
		cvNamedWindow("SceneRIGHT");
		cvNamedWindow("Disparity");

		all::core::uint8_sarr ieye[2];
		all::core::uint8_sarr iscene[2];
		all::core::single_sarr idepth;
		all::core::single_sarr idisp;

		ieye[left].reset(new all::core::uint8_t[eye_bw_sz_[left]]);
		ieye[right].reset(new all::core::uint8_t[eye_bw_sz_[right]]);
		iscene[left].reset(new all::core::uint8_t[scene_sz_[left]]);
		iscene[right].reset(new all::core::uint8_t[scene_sz_[right]]);
		idisp.reset(new all::core::single_t[scenedims_[left].col_ * scenedims_[left].row_]);

		//all::sense::stereo_process_t stereo_process;
		//stereo_process.init("config/gaze_stereo.ini");

		while (running_ && imagecnt < imagestosave) {
			    
				sample_gaze_();


				ipl_eye_img_[left]->origin = IPL_ORIGIN_BL;
				ipl_eye_img_[right]->origin = IPL_ORIGIN_BL;
				ipl_scene_img_[left]->origin = IPL_ORIGIN_BL;
				ipl_scene_img_[right]->origin = IPL_ORIGIN_BL;

				//idepth = stereo_process.do_stereo(ipl_scene_img_[left], ipl_scene_img_[right]);


				cvShowImage("EyeLEFT",    ipl_eye_img_[left]);
				cvShowImage("EyeRIGHT",   ipl_eye_img_[right]);
				cvShowImage("SceneLEFT",  ipl_scene_img_[left]);
				cvShowImage("SceneRIGHT", ipl_scene_img_[right]);
				//cvShowImage("Disparity", stereo_process.get_disparity());


				if (bsavecalib_) {

					cvConvertImage(ipl_eye_img_[left], ipl_eye_img_[left], CV_CVTIMG_FLIP);
					cvConvertImage(ipl_eye_img_[right], ipl_eye_img_[right], CV_CVTIMG_FLIP);
					cvConvertImage(ipl_scene_img_[left], ipl_scene_img_[left], CV_CVTIMG_FLIP);
					cvConvertImage(ipl_scene_img_[right], ipl_scene_img_[right], CV_CVTIMG_FLIP);

					

					bsavecalib_ = false;
					imagecnt++;
					printf("Saving %d calib file.\n", imagecnt);

					//BW conversion
					cvCvtColor(ipl_eye_img_[left], ipl_eye_bw_img_[left], CV_BGR2GRAY);
					cvCvtColor(ipl_eye_img_[right], ipl_eye_bw_img_[right], CV_BGR2GRAY);

					////RGB conversion
					//cvCvtColor(scene_left, ipl_scene_rgb_img_[left], CV_BGR2RGB);
					//cvCvtColor(scene_right, ipl_scene_rgb_img_[right], CV_BGR2RGB);

					memcpy(ieye[left].get(), ipl_eye_bw_img_[left]->imageData, eye_bw_sz_[left]);
					memcpy(ieye[right].get(), ipl_eye_bw_img_[right]->imageData, eye_bw_sz_[right]);
					memcpy(iscene[left].get(), ipl_scene_img_[left]->imageData, scene_sz_[left]);
					memcpy(iscene[right].get(), ipl_scene_img_[right]->imageData, scene_sz_[right]);
					//memcpy(idisp.get(), stereo_process.get_disparity()->imageData, depth_sz_);

					////////////////////////////////////////////////////////////////////////
					//MATLAB --------------------------------------------------------------+
			  //      //
					MATFile *pmat = 0;
			  //      //MATLAB
					std::string namebase = "calib_";
			  //      //
					namebase += boost::lexical_cast<std::string>(imagecnt);
					namebase += ".mat";
			  //      //
					pmat = matOpen(namebase.c_str(), "w");

					//-----------------
					mxArray* mx_lefteye = 
					  matlab::buffer2array<core::uint8_t>::create_from_planar(ieye[left].get()
																	  , matlab::row_major
																	  , eyedims_[left].row_
																	  , eyedims_[left].col_
																	  , 1);
					mxArray* mx_righteye = 
					  matlab::buffer2array<core::uint8_t>::create_from_planar(ieye[right].get()
																	  , matlab::row_major
																	  , eyedims_[right].row_
																	  , eyedims_[right].col_
																	  , 1);
					//-----------------
					//mxArray!
					mxArray* mx_leftscene = 
					  matlab::buffer2array<core::uint8_t>::create_from_interleaved(iscene[left].get()
																	  , matlab::row_major
																	  , scenedims_[left].row_
																	  , scenedims_[left].col_);
					mxArray* mx_rightscene = 
					  matlab::buffer2array<core::uint8_t>::create_from_interleaved(iscene[right].get()
																	  , matlab::row_major
																	  , scenedims_[right].row_
																	  , scenedims_[right].col_);

					mxArray* mx_depth = 
						     all::matlab::buffer2array<core::single_t>::create_from_planar(idepth.get()
							 , matlab::row_major
							 , scenedims_[left].row_
							 , scenedims_[left].col_);

					mxArray* mx_disp = 
						     all::matlab::buffer2array<core::single_t>::create_from_planar(idisp.get()
							 , matlab::row_major
							 , scenedims_[left].row_
							 , scenedims_[left].col_,
							 1);

					//-----------------
					mxArray* mx_num = mxCreateDoubleScalar(imagecnt);
					//-----------------
					//MTI
					mxArray* mx_roll = 
					  mxCreateScalarDouble(heading_.roll.deg());
					mxArray* mx_pitch = 
					  mxCreateScalarDouble(heading_.pitch.deg());
					mxArray* mx_yaw = 
					  mxCreateScalarDouble(heading_.yaw.deg());

					//
				   const char *field_names[] = {  "count"  ,
												  "scene_left"  ,
												  "scene_right"  ,
												  "depth" ,
												  "disp" ,
												  "eye_left"  ,
												  "eye_right"  ,
												  "roll"   ,
												  "pitch"  ,
												  "yaw" };
			  //      //
					mwSize dims[2] = {1, 1};

			  //      //
					mxArray* ostruct= mxCreateStructArray(2, dims, 10, field_names);

			  //      //
					int count_field
					  , scene_L_field
					  , scene_R_field
					  , depth_field
					  , disp_field
					  , eye_L_field
					  , eye_R_field
					  , roll_field
					  , pitch_field
					  , yaw_field;

			  //      //
					count_field     = mxGetFieldNumber(ostruct,   "count" );
					scene_L_field   = mxGetFieldNumber(ostruct,   "scene_left" );
					scene_R_field     = mxGetFieldNumber(ostruct, "scene_right" );
					depth_field		= mxGetFieldNumber(ostruct,	  "depth");
					disp_field      = mxGetFieldNumber(ostruct,   "disp");
					eye_L_field     = mxGetFieldNumber(ostruct,   "eye_left" );
					eye_R_field     = mxGetFieldNumber(ostruct,   "eye_right" );
					roll_field      = mxGetFieldNumber(ostruct,   "roll"  );
					pitch_field     = mxGetFieldNumber(ostruct,   "pitch" );
					yaw_field       = mxGetFieldNumber(ostruct,   "yaw"   );

					//count
					mxSetFieldByNumber( ostruct
									  , 0
									  , count_field
									  , mx_num);
					//scene
					mxSetFieldByNumber( ostruct
									  , 0
									  , scene_L_field
									  , mx_leftscene);

					mxSetFieldByNumber( ostruct
									  , 0
									  , scene_R_field
									  , mx_rightscene);

					mxSetFieldByNumber( ostruct
									  , 0
									  , depth_field
									  , mx_depth);

					mxSetFieldByNumber( ostruct
									  , 0
									  , disp_field
									  , mx_disp);

					//imeye
					mxSetFieldByNumber( ostruct
									  , 0
									  , eye_L_field
									  , mx_lefteye);
					//imeye
					mxSetFieldByNumber( ostruct
									  , 0
									  , eye_R_field
									  , mx_righteye);
					//roll
					mxSetFieldByNumber( ostruct
									  , 0
									  , roll_field
									  , mx_roll);
					//pitch
					mxSetFieldByNumber( ostruct
									  , 0
									  , pitch_field
									  , mx_pitch);
					//yaw
					mxSetFieldByNumber( ostruct
									  , 0
									  , yaw_field
									  , mx_yaw);
			  //      //-----------------
					//write to matfile
					matPutVariable(pmat, "calibration", ostruct);

			  //      //  
					matClose(pmat);  

					//
					mxDestroyArray(mx_leftscene);
					mxDestroyArray(mx_rightscene);
					mxDestroyArray(mx_depth);
					mxDestroyArray(mx_disp);
					mxDestroyArray(mx_lefteye);
					mxDestroyArray(mx_righteye);
					mxDestroyArray(mx_num);
					mxDestroyArray(mx_roll);
					mxDestroyArray(mx_pitch);
					mxDestroyArray(mx_yaw);

			}
			

			cvWaitKey(1);
		}

	}

}
//-------------------------------------------------------------------------++
void gaze_machine_VI_t::log_loop()
{
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  { 
    //jpeg init
	sarr_eye_img_[left].reset(new all::core::uint8_t[eyedims_[left].row_ * eyedims_[left].col_]);
	sarr_eye_img_[right].reset(new all::core::uint8_t[eyedims_[right].row_ * eyedims_[right].col_]);
	sarr_scene_img_[left].reset(new all::core::uint8_t[scenedims_[left].row_ * scenedims_[left].col_ * 3]);
	sarr_scene_img_[right].reset(new all::core::uint8_t[scenedims_[right].row_ * scenedims_[right].col_ * 3]);

	//jpeg barrier test
	enc_sync_data = new all::core::enc_sync_data_t(4);

	gaze_jpeg_encoder_t::start(enc_sync_data);

	eye_enc_[left] = new all::core::gaze_jpeg_encoder_t(sarr_eye_img_[left], &jpeg_eye_data_[left], true, eyedims_[left].col_, eyedims_[left].row_);
	eye_enc_[right] = new all::core::gaze_jpeg_encoder_t(sarr_eye_img_[right], &jpeg_eye_data_[right], true, eyedims_[right].col_, eyedims_[right].row_);
	scene_enc_[left] = new all::core::gaze_jpeg_encoder_t(sarr_scene_img_[left], &jpeg_scene_data_[left], false, scenedims_[left].col_, scenedims_[left].row_);
	scene_enc_[right] = new all::core::gaze_jpeg_encoder_t(sarr_scene_img_[right], &jpeg_scene_data_[right], false, scenedims_[right].col_, scenedims_[right].row_);

	///
	gazelog_.open(logname_.c_str(),std::ios::out|std::ios::binary);
    ///
    write_header_bin_();


    printf("->machine booted ...starting loop\n");
    //reset timer
    start_timing();
    //entering the loop
    while (running_)
    {
      ///
		if (sample_gaze_()) {
			nsamples_++;
			write_gaze_bin_();
		}
      boost::thread::yield();
      //all::core::BOOST_SLEEP(msecspause_);
    }

    printf("Thread Canceled\n");
    elapsed_ = elapsed();
    //go beginning
    gazelog_.seekp(std::ios::beg);
    //save info
    //numsamples
    gazelog_.write((char*)&nsamples_, sizeof(nsamples_)); 
    //time spent
    gazelog_.write((char*)&elapsed_, sizeof(elapsed_)); 
    //and then close
    gazelog_.close(); 

	VI->stopDevice(eye_[left]);
	VI->stopDevice(eye_[right]);
	VI->stopDevice(scene_[left]);
	VI->stopDevice(scene_[right]);

    cvReleaseImage(&ipl_eye_img_[left]);
    cvReleaseImage(&ipl_eye_img_[right]);  
    cvReleaseImage(&ipl_scene_img_[left]);  
    cvReleaseImage(&ipl_scene_img_[right]); 
	cvReleaseImage(&ipl_eye_bw_img_[left]);
	cvReleaseImage(&ipl_eye_bw_img_[right]);

	gaze_jpeg_encoder_t::stop();

  }
  else
      printf("devices not started!\n"); 
}

void gaze_machine_VI_t::avi_loop() {
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  if(boot_machine_())
  { 

    //avi init
	eye_avi_[left] = cvCreateVideoWriter ("eye_left.avi", -1, 15, cvSize(eyedims_[left].col_, eyedims_[left].row_));
	eye_avi_[right] = cvCreateVideoWriter ("eye_right.avi", -1, 15, cvSize(eyedims_[right].col_, eyedims_[right].row_));
	scene_avi_[left] = cvCreateVideoWriter ("scene_left.avi", -1, 15, cvSize(scenedims_[left].col_, scenedims_[left].row_));
	scene_avi_[right] = cvCreateVideoWriter ("scene_right.avi", -1, 15, cvSize(scenedims_[right].col_, scenedims_[right].row_));

    ///
    gazelog_.open(logname_.c_str(),std::ios::out|std::ios::binary);
    ///
    write_header_avi_();

    //reset_mti();
    printf("->machine booted ...starting loop\n");
    //reset timer
    start_timing();
    //entering the loop
    while (running_)
    {
      ///
		if (sample_gaze_()) {
			nsamples_++;
			write_gaze_avi_();
		}
      boost::thread::yield();
      //all::core::BOOST_SLEEP(msecspause_);
    }

    printf("Thread Canceled\n");
    elapsed_ = elapsed();
    //go beginning
    gazelog_.seekp(std::ios::beg);
    //save info
    //numsamples
    gazelog_.write((char*)&nsamples_, sizeof(nsamples_)); 
    //time spent
    gazelog_.write((char*)&elapsed_, sizeof(elapsed_)); 
    //and then close
    gazelog_.close(); 

	cvReleaseVideoWriter(&eye_avi_[left] );
	cvReleaseVideoWriter(&eye_avi_[right] );
	cvReleaseVideoWriter(&scene_avi_[left] );
	cvReleaseVideoWriter(&scene_avi_[right] );

	VI->stopDevice(eye_[left]);
	VI->stopDevice(eye_[right]);
	VI->stopDevice(scene_[left]);
	VI->stopDevice(scene_[right]);

    cvReleaseImage(&ipl_eye_img_[left]);
    cvReleaseImage(&ipl_eye_img_[right]);  
    cvReleaseImage(&ipl_scene_img_[left]);  
    cvReleaseImage(&ipl_scene_img_[right]); 
	cvReleaseImage(&ipl_eye_bw_img_[left]);
	cvReleaseImage(&ipl_eye_bw_img_[right]);
  }
  else
      printf("devices not started!\n"); 
}

//-------------------------------------------------------------------------++
void gaze_machine_VI_t::show_loop()
{
  printf("->in the thread loop!\n");
  printf("->boot_machine_ .. \n");
  

  if(boot_machine_())
  { 
    cvNamedWindow("EyeLEFT");
    cvNamedWindow("EyeRIGHT");
    cvNamedWindow("SceneLEFT");
    cvNamedWindow("SceneRIGHT");
    ////
    printf("->machine booted ...starting loop\n"); 
  


   start_timing();

    while (running_)
    {


      //
	  if (sample_gaze_()) {
		  nsamples_++;

		ipl_eye_img_[left]->origin = IPL_ORIGIN_BL;
	    ipl_eye_img_[right]->origin = IPL_ORIGIN_BL;
		ipl_scene_img_[left]->origin = IPL_ORIGIN_BL;
	    ipl_scene_img_[right]->origin = IPL_ORIGIN_BL;

		cvShowImage("EyeLEFT",    ipl_eye_img_[left]     );
		cvShowImage("EyeRIGHT",   ipl_eye_img_[right]    );
		cvShowImage("SceneLEFT",  ipl_scene_img_[left]   );
		cvShowImage("SceneRIGHT", ipl_scene_img_[right]  );

	  }

      //
	  cvWaitKey(1);
      //all::core::BOOST_SLEEP(msecspause_);
  }

    printf("Thread Canceled\n");
    elapsed_ = elapsed();

	
	VI->stopDevice(eye_[left]);
	VI->stopDevice(eye_[right]);
	VI->stopDevice(scene_[left]);
	VI->stopDevice(scene_[right]);

    cvReleaseImage(&ipl_eye_img_[left]);
    cvReleaseImage(&ipl_eye_img_[right]);  
    cvReleaseImage(&ipl_scene_img_[left]);  
    cvReleaseImage(&ipl_scene_img_[right]);  
  }
  else
      printf("devices not started!\n"); 
}
//-------------------------------------------------------------------------++
void gaze_machine_VI_t::null_op_()
{
}
//-------------------------------------------------------------------------++
int gaze_machine_VI_t::nsamples() const
{
return nsamples_;
}
//-------------------------------------------------------------------------++
void gaze_machine_VI_t::run_machine(binlog_t const&)
{
  //gaze_loop_ = boost::bind(&gaze_machine_VI_t::gaze_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine_VI_t::log_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine_VI_t::run_machine(calib_t const&)
{
  //gaze_loop_ = boost::bind(&gaze_machine_VI_t::calib_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine_VI_t::calib_loop, this) ) );
}
//-------------------------------------------------------------------------++
void gaze_machine_VI_t::run_machine(show_t const&)
{
  //gaze_loop_ = boost::bind(&gaze_machine_VI_t::show_loop,      this);
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine_VI_t::show_loop, this) ) );
}

void gaze_machine_VI_t::run_machine(avilog_t const&)
{
  thread_ptr_.reset( new boost::thread(boost::bind(&gaze_machine_VI_t::avi_loop, this) ) );
}
//-------------------------------------------------------------------------++
/////////////////////////////////////////////////////////////////////////////
}}///all::sense::detail
