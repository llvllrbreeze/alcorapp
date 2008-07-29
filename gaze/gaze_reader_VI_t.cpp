#include "gaze_reader_VI_t.h"
#include "alcor/matlab/matlab_mx_utils.hpp"
#include "alcor/core/image_utils.h"
//-------------------------------------------------------------------------++
//#include "alcor.extern/CImg/CImg.h"
//using namespace cimg_library;

//-------------------------------------------------------------------------++
#define WRITESTRUCTS_
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
  gaze_reader_VI_t::gaze_reader_VI_t():
          current_sample_(0)
        , nsamples_(0)
{
}
//-------------------------------------------------------------------------++
gaze_reader_VI_t::~gaze_reader_VI_t()
{
  if (gazelog_.is_open())
    gazelog_.close();
  if (log_type_ == 1) {
	  cvReleaseCapture(&eye_avi_[left]);
	  cvReleaseCapture(&eye_avi_[right]);
	  cvReleaseCapture(&scene_avi_[left]);
	  cvReleaseCapture(&scene_avi_[right]);
  }

}
//-------------------------------------------------------------------------++
void gaze_reader_VI_t::load(std::string& binlog)
{
  //
  gazelog_.open(binlog.c_str(), std::ios::in|std::ios::binary);

  printf("\nSession data:\n");
  gazelog_.read((char*)&nsamples_, sizeof(nsamples_));
  printf("-> nsamples: %d\n", nsamples_);

  gazelog_.read((char*)&total_elapsed_, sizeof(total_elapsed_));
  printf("-> total time: %f\n", total_elapsed_);

  gazelog_.read((char*)&log_type_, sizeof(log_type_));
  if (log_type_ == 0)
	  printf("-> binary log\n");
  else
	  printf("-> avi log\n");

  gazelog_.read((char*)&eyedims_[left], sizeof(eyedims_[left]));
  printf("-> left eye dims: %d:%d:%d\n", eyedims_[left].row_, eyedims_[left].col_, eyedims_[left].depth_);
  gazelog_.read((char*)&eyedims_[right], sizeof(eyedims_[right]));
  printf("-> right eye dims: %d:%d:%d\n", eyedims_[right].row_, eyedims_[right].col_, eyedims_[right].depth_);

  gazelog_.read((char*)&scenedims_[left], sizeof(scenedims_[left]));
  printf("-> left scene dims: %d:%d:%d\n", scenedims_[left].row_, scenedims_[left].col_, scenedims_[left].depth_);
  gazelog_.read((char*)&scenedims_[right], sizeof(scenedims_[right]));
  printf("-> right scene dims: %d:%d:%d\n", scenedims_[right].row_, scenedims_[right].col_, scenedims_[right].depth_);

  printf("\nOffsets:\n");

  gazelog_.read((char*)&elapsed_offset_, sizeof(elapsed_offset_));
  printf("elapsed offset: %d\n", elapsed_offset_);

  gazelog_.read((char*)&eye_offset_[left], sizeof(eye_offset_[left]));
  printf("left eye offset: %d\n", eye_offset_[left]);
  gazelog_.read((char*)&eye_offset_[right], sizeof(eye_offset_[right]));
  printf("right eye offset: %d\n", eye_offset_[right]);

  gazelog_.read((char*)&scene_offset_[left], sizeof(scene_offset_[left]));
  printf("left scence offset: %d\n", scene_offset_[left]);
  gazelog_.read((char*)&scene_offset_[right], sizeof(scene_offset_[right]));
  printf("right scence offset: %d\n", scene_offset_[right]);

  gazelog_.read((char*)&rpy_offset_, sizeof(rpy_offset_));
  printf("rpy  offset: %d\n", rpy_offset_);

  //
  //header_offset_ =  sizeof(nsamples_) 
  //                + sizeof(total_elapsed_)
  //                + sizeof(eyedims_)
  //                + sizeof(scenedims_)
  //                + sizeof(elapsed_offset_)
  //                + sizeof(eye_offset_)
  //                + sizeof(scene_offset_)
  //                + sizeof(depth_offset_)
  //                + sizeof(rpy_offset_)
  //                ;

  //size_t per_sample_offset_ = 
  //                elapsed_offset_
  //              + eye_offset_[left]
  //              + scene_offset_[left]
  //              + depth_offset_
  //              + rpy_offset_;

  //printf("header offset: %d\n", header_offset_);
  //printf("per sample offset: %d\n", per_sample_offset_);

  if (log_type_ == 0) 
	  sample = boost::bind(&gaze_reader_VI_t::sample_bin_, this);
  else if (log_type_ == 1)
	  sample = boost::bind(&gaze_reader_VI_t::sample_avi_, this);
  else
	  sample = NULL;

  allocate_();
}
//-------------------------------------------------------------------------++
void gaze_reader_VI_t::allocate_()
{
  //ieye[left].reset  ( new core::uint8_t  [ eye_offset_ ]  );
  //ieye[right].reset  ( new core::uint8_t  [ eye_offset_ ]  );
  //iscene.reset( new core::uint8_t  [ scene_offset_] );
  //idepth.reset( new core::single_t [ depth_offset_] );

	if (log_type_ == 0) {
		jpeg_dec_.set_output_ordering(all::core::interleaved_tag);
		enc_eye_data_[left].data.reset(new all::core::uint8_t [eye_offset_[left]]);
		enc_eye_data_[right].data.reset(new all::core::uint8_t [eye_offset_[right]]);
		enc_scene_data_[left].data.reset(new all::core::uint8_t [scene_offset_[left]]);
		enc_scene_data_[right].data.reset(new all::core::uint8_t [scene_offset_[right]]);
	}
	else if (log_type_ == 1) {
		eye_avi_[left] = cvCreateFileCapture("eye_left.avi");
		eye_avi_[right] = cvCreateFileCapture("eye_right.avi");
		scene_avi_[left] = cvCreateFileCapture("scene_left.avi");
		scene_avi_[right] = cvCreateFileCapture("scene_right.avi");

	}

   //for matlab array creation
   ieye[left].reset(new all::core::uint8_t[eye_offset_[left]]);
   ieye[right].reset(new all::core::uint8_t[eye_offset_[right]]);
   iscene[left].reset(new all::core::uint8_t[scene_offset_[left]]);
   iscene[right].reset(new all::core::uint8_t[scene_offset_[right]]);
   idepth.reset(new all::core::single_t[scene_offset_[left]]);

   
   eye_img_[left] = cvCreateImage(cvSize(eyedims_[left].col_, eyedims_[left].row_),IPL_DEPTH_8U, 3);
   eye_img_[right] = cvCreateImage(cvSize(eyedims_[right].col_, eyedims_[right].row_),IPL_DEPTH_8U, 3);
   
   eye_bw_img_[left] = cvCreateImage(cvSize(eyedims_[left].col_, eyedims_[left].row_),IPL_DEPTH_8U, 1);
   eye_bw_img_[right] = cvCreateImage(cvSize(eyedims_[right].col_, eyedims_[right].row_),IPL_DEPTH_8U, 1);
   scene_img_[left] = cvCreateImage(cvSize(scenedims_[left].col_, scenedims_[left].row_),IPL_DEPTH_8U, 3); 
   scene_img_[right] = cvCreateImage(cvSize(scenedims_[right].col_, scenedims_[right].row_),IPL_DEPTH_8U, 3);

   eye_img_[left]->origin = IPL_ORIGIN_BL;
   eye_img_[right]->origin = IPL_ORIGIN_BL;
   eye_bw_img_[left]->origin = IPL_ORIGIN_BL;
   eye_bw_img_[right]->origin = IPL_ORIGIN_BL;
   scene_img_[left]->origin = IPL_ORIGIN_BL;
   scene_img_[right]->origin = IPL_ORIGIN_BL;

   //stereo_process.init("config/gaze_stereo.ini");

   //_eye_filter[left].LoadCameraParams("cmos2.ini");
   //_eye_filter[right].LoadCameraParams("cmos2.ini");

   current_sample_ = 0;
}
//-------------------------------------------------------------------------++
bool gaze_reader_VI_t::sample_bin_()
{ 
  if(!eof())
  {
    current_sample_++;
    //elapsed time .. in seconds
    gazelog_.read((char*)&elapsed_,  elapsed_offset_);

	gazelog_.read((char*)&enc_eye_data_[left].size, sizeof(std::size_t));
	gazelog_.read((char*)enc_eye_data_[left].data.get(), enc_eye_data_[left].size);
	gazelog_.read((char*)&enc_eye_data_[right].size, sizeof(enc_eye_data_[right].size));
	gazelog_.read((char*)enc_eye_data_[right].data.get(), enc_eye_data_[right].size);
	gazelog_.read((char*)&enc_scene_data_[left].size, sizeof(enc_scene_data_[left].size));
	gazelog_.read((char*)enc_scene_data_[left].data.get(), enc_scene_data_[left].size);
	gazelog_.read((char*)&enc_scene_data_[right].size, sizeof(enc_scene_data_[right].size));
	gazelog_.read((char*)enc_scene_data_[right].data.get(), enc_scene_data_[right].size);

	jpeg_dec_.decode(dec_eye_data_[left], enc_eye_data_[left].data, enc_eye_data_[left].size);
	jpeg_dec_.decode(dec_eye_data_[right], enc_eye_data_[right].data, enc_eye_data_[right].size);
	jpeg_dec_.decode(dec_scene_data_[left], enc_scene_data_[left].data, enc_scene_data_[left].size);
	jpeg_dec_.decode(dec_scene_data_[right], enc_scene_data_[right].data, enc_scene_data_[right].size);

	//ieye[left] = (dec_eye_data_[left].data);
	//ieye[right] = (dec_eye_data_[right].data);
	//iscene[left] = (dec_scene_data_[left].data);
	//iscene[right] = (dec_scene_data_[right].data);

	memcpy(eye_bw_img_[left]->imageData, dec_eye_data_[left].data.get(), eye_offset_[left]);
	memcpy(eye_bw_img_[right]->imageData, dec_eye_data_[right].data.get(), eye_offset_[right]);
	memcpy(scene_img_[left]->imageData, dec_scene_data_[left].data.get(), scene_offset_[left]);
	memcpy(scene_img_[right]->imageData, dec_scene_data_[right].data.get(), scene_offset_[right]);

	//cvCvtColor(eye_img_[left], eye_bw_img_[left], CV_BGR2GRAY);
	//cvCvtColor(eye_img_[right], eye_bw_img_[right], CV_BGR2GRAY);

    ///Orientation
    gazelog_.read((char*)&ihead,  rpy_offset_);
  }
  else
    return false;

  return true;
}

bool gaze_reader_VI_t::sample_avi_() {
  if(!eof())
  {
    current_sample_++;
    //elapsed time .. in seconds
    gazelog_.read((char*)&elapsed_,  elapsed_offset_);

	eye_img_[left] = cvQueryFrame(eye_avi_[left]);
	eye_img_[right] = cvQueryFrame(eye_avi_[right]);
	scene_img_[left] = cvQueryFrame(scene_avi_[left]);
	scene_img_[right] = cvQueryFrame(scene_avi_[right]);

	cvCvtColor(eye_img_[left], eye_bw_img_[left], CV_BGR2GRAY);
	cvCvtColor(eye_img_[right], eye_bw_img_[right], CV_BGR2GRAY);
	//memcpy(ieye[left].get(), eye_bw_img_[left]->imageData, eye_offset_[left]);
	//memcpy(ieye[right].get(), eye_bw_img_[right]->imageData, eye_offset_[right]);
	//memcpy(iscene[left].get(), scene_img_[left]->imageData, scene_offset_[left]);
	//memcpy(iscene[right].get(), scene_img_[right]->imageData, scene_offset_[right]);

    ///Orientation
    gazelog_.read((char*)&ihead,  rpy_offset_);
  }
  else
    return false;

  return true;
}

//-------------------------------------------------------------------------++
void gaze_reader_VI_t::reset()
{
  gazelog_.seekg(header_offset_);
  current_sample_ = 0;
}
//-------------------------------------------------------------------------++
///
bool gaze_reader_VI_t::next()
{
  return sample();
}
//-------------------------------------------------------------------------++
  ///jump at num sample
bool gaze_reader_VI_t::peek(size_t num)
{
  if(num < nsamples_)
    {
    gazelog_.seekg(header_offset_ + ((num-1) * per_sample_offset_));
    current_sample_ = num-1;
    return sample();
    }
  else 
    return false;
}
//-------------------------------------------------------------------------++
///
void gaze_reader_VI_t::play(bool bshow, bool bsavemat)
{
  //reset();

  double last_elapsed = 0;
  unsigned int timeroffset;

   cvNamedWindow("EyeLEFT");
   cvNamedWindow("EyeRIGHT");
   cvNamedWindow("SceneLEFT");
   cvNamedWindow("SceneRIGHT");
   //cvNamedWindow("Depth");

 /*  IplImage* eye_left = cvCreateImage(cvSize(eyedims_[left].col_, eyedims_[left].row_),IPL_DEPTH_8U, 1);
   IplImage* eye_right = cvCreateImage(cvSize(eyedims_[right].col_, eyedims_[right].row_),IPL_DEPTH_8U, 1);
   IplImage* scene_left = cvCreateImage(cvSize(scenedims_[left].col_, scenedims_[left].row_),IPL_DEPTH_8U, 3); 
   IplImage* scene_right = cvCreateImage(cvSize(scenedims_[right].col_, scenedims_[right].row_),IPL_DEPTH_8U, 3);*/


  ///
  while(sample())
  {
    //TIME
    //timeroffset = static_cast<unsigned int> ( (elapsed_- last_elapsed) * 1000.0);
    //core::BOOST_SLEEP( timeroffset );

	  //idepth = stereo_process.do_stereo(scene_img_[left], scene_img_[right]);

	  //IplImage* stereo_pair[] = {scene_img_[left], scene_img_[right]};
	  //stereo_process.undistort(stereo_pair, stereo_pair);

	  //_eye_filter[left].Undistort(&eye_bw_img_[left], &eye_bw_img_[left]);
	  //_eye_filter[right].Undistort(&eye_bw_img_[right], &eye_bw_img_[right]);

	  //memcpy(eye_left->imageData, ieye[left].get(), eye_offset_[left]);
	  //memcpy(eye_right->imageData, ieye[right].get(), eye_offset_[right]);
	  //memcpy(scene_left->imageData, iscene[left].get(), scene_offset_[left]);
	  //memcpy(scene_right->imageData, iscene[right].get(), scene_offset_[right]);

	  //cvConvertImage(scene_left, scene_left, CV_CVTIMG_FLIP);
	  //cvConvertImage(scene_right, scene_right, CV_CVTIMG_FLIP);
	  //eye_left->origin = IPL_ORIGIN_BL;
	  //eye_right->origin = IPL_ORIGIN_BL;
	  //cvConvertImage(eye_left, eye_img_[left], CV_CVTIMG_FLIP);
	  //cvConvertImage(eye_right, eye_img_[right], CV_CVTIMG_FLIP);


    if(bshow)
      {
		  //memcpy(eye_left->imageData, ieye[left].get(), eye_offset_[left]);
		  //memcpy(eye_right->imageData, ieye[right].get(), eye_offset_[right]);
		  //memcpy(scene_left->imageData, iscene[left].get(), scene_offset_[left]);
		  //memcpy(scene_right->imageData, iscene[right].get(), scene_offset_[right]);

		  //cvCvtColor(scene_left, scene_left, CV_RGB2BGR);
		  //cvCvtColor(scene_right, scene_right, CV_RGB2BGR);

		  cvShowImage("EyeLEFT", eye_bw_img_[left]);
		  cvShowImage("EyeRIGHT", eye_bw_img_[right]);
		  cvShowImage("SceneLEFT", scene_img_[left]);
		  cvShowImage("SceneRIGHT", scene_img_[right]);
		  //cvShowImage("Depth", stereo_process.get_disparity());

		  cvWaitKey(1);
    }

    ////////////////////////////////////////////////////////////////////////
	if (bsavemat) { 
	
		//cvFlip(eye_bw_img_[left], eye_bw_img_[left], 0);
		//cvFlip(eye_bw_img_[right], eye_bw_img_[right], 0);
		//cvFlip(scene_img_[left], scene_img_[left], 0);
		//cvFlip(scene_img_[right], scene_img_[right], 0);

		if(log_type_ == 0) {
			cvFlip(eye_bw_img_[left], eye_bw_img_[left], 0);
			cvFlip(eye_bw_img_[right], eye_bw_img_[right], 0);
			cvConvertImage(scene_img_[left], scene_img_[left], CV_CVTIMG_FLIP);
			cvConvertImage(scene_img_[right], scene_img_[right], CV_CVTIMG_FLIP);
		}
		

		memcpy(ieye[left].get(), eye_bw_img_[left]->imageData, eye_offset_[left]);
		memcpy(ieye[right].get(), eye_bw_img_[right]->imageData, eye_offset_[right]);
		memcpy(iscene[left].get(), scene_img_[left]->imageData,  scene_offset_[left]);
		memcpy(iscene[right].get(), scene_img_[right]->imageData, scene_offset_[right]);
		
        matlab_dump();
	
	}
    ////////////////////////////////////////////////////////////////////////

    //for next loop
    last_elapsed = elapsed_;
  }
}
//-------------------------------------------------------------------------++
  ///
void gaze_reader_VI_t::matlab_dump()
{
    ////////////////////////////////////////////////////////////////////////
    //MATLAB --------------------------------------------------------------+
    //
    MATFile *pmat = 0;
    //MATLAB
    std::string namebase = "./gazelog_";
    //
    namebase += boost::lexical_cast<std::string>(current_sample_);

    namebase += ".mat";
    //
    pmat = matOpen(namebase.c_str(), "w");

	mxArray* mx_scene[2];
	mxArray* mx_eye[2];
	//mxArray* mx_depth;
    //
    //printf("\nrgb\n");
    //-----------------
    mx_scene[left] = 
      matlab::buffer2array<core::uint8_t>::create_from_interleaved(iscene[left].get()
                                                      , matlab::row_major
                                                      , scenedims_[left].row_
                                                      , scenedims_[left].col_);
	mx_scene[right] = 
      matlab::buffer2array<core::uint8_t>::create_from_interleaved(iscene[right].get()
                                                      , matlab::row_major
                                                      , scenedims_[right].row_
                                                      , scenedims_[right].col_);

    //mx_depth  = 
    //  all::matlab::buffer2array<core::single_t>::create_from_planar(
    //    idepth.get()
    //  , matlab::row_major
    //  , scenedims_[left].row_
    //  , scenedims_[left].col_);

    //printf("eye\n");
    //------------------
    mx_eye[left] = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(ieye[left].get()
                                                      , matlab::row_major
                                                      , eyedims_[left].row_
                                                      , eyedims_[left].col_
                                                      , 1); 
	mx_eye[right] = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(ieye[right].get()
                                                      , matlab::row_major
                                                      , eyedims_[right].row_
                                                      , eyedims_[right].col_
                                                      , 1);    
    //------------------
    //printf("time\n");
    mxArray* mx_time = 
      mxCreateScalarDouble(elapsed_);
    //------------------
    //printf("roll, pitch, yaw\n");
    mxArray* mx_roll = 
      mxCreateScalarDouble(ihead.roll.deg());
    mxArray* mx_pitch = 
      mxCreateScalarDouble(ihead.pitch.deg());
    mxArray* mx_yaw = 
      mxCreateScalarDouble(ihead.yaw.deg());
    //------------------

     const char *field_names[] = {  "elapsed",
                                    "scene_left"  ,
									"scene_right",
                                    "depth"    ,
                                    "eye_left"   ,
									"eye_right",
                                    "roll"   ,
                                    "pitch"  ,
                                    "yaw" };
      mwSize dims[2] = {1, 1};

      mxArray* ostruct= mxCreateStructArray(2, dims, 9, field_names);

      int elapsed_field
        , scene_left_field
		, scene_right_field
        , depth_field
        , eye_left_field
		, eye_right_field
        , roll_field
        , pitch_field
        , yaw_field;

      elapsed_field = mxGetFieldNumber(ostruct,   "elapsed");
      scene_left_field   = mxGetFieldNumber(ostruct,   "scene_left");
	  scene_right_field   = mxGetFieldNumber(ostruct,   "scene_right");
      //depth_field   = mxGetFieldNumber(ostruct,   "depth");
      eye_left_field   = mxGetFieldNumber(ostruct,   "eye_left");
	  eye_right_field   = mxGetFieldNumber(ostruct,   "eye_right");
      roll_field    = mxGetFieldNumber(ostruct,   "roll");
      pitch_field   = mxGetFieldNumber(ostruct,   "pitch");
      yaw_field     = mxGetFieldNumber(ostruct,   "yaw");

      //time
      mxSetFieldByNumber( ostruct
                        , 0
                        , elapsed_field
                        , mx_time);
      //scene
      mxSetFieldByNumber( ostruct
                        , 0
                        , scene_left_field
                        , mx_scene[left]);
	  mxSetFieldByNumber( ostruct
                        , 0
                        , scene_right_field
                        , mx_scene[right]);
      //depth
      //mxSetFieldByNumber( ostruct
      //                  , 0
      //                  , depth_field
      //                  , mx_depth);
      //imeye
      mxSetFieldByNumber( ostruct
                        , 0
                        , eye_left_field
                        , mx_eye[left]);
	  mxSetFieldByNumber( ostruct
                        , 0
                        , eye_right_field
                        , mx_eye[right]);
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

      //write to matfile
      matPutVariable(pmat, "gazelog", ostruct);

    //  
    //printf("Close mat\n");
    matClose(pmat);  

    //
    //printf("Destroy array\n");
    mxDestroyArray(mx_scene[left]);
	mxDestroyArray(mx_scene[right]);
    //mxDestroyArray(mx_depth);
    mxDestroyArray(mx_eye[left]);
	mxDestroyArray(mx_eye[right]);
    mxDestroyArray(mx_time);
    mxDestroyArray(mx_roll);
    mxDestroyArray(mx_pitch);
    mxDestroyArray(mx_yaw);

}
//-------------------------------------------------------------------------++
bool gaze_reader_VI_t::eof()
{
  return current_sample_ == nsamples_;
}
//-------------------------------------------------------------------------++
}}//all::gaze