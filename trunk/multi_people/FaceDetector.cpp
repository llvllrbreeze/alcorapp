#include "FaceDetector.h"

#define ADD_SEARCH_BORDER 10

FaceDetector::FaceDetector()
{
	storage_ = 0;
	faceCascade_ = 0;
	eyeCascade_ = 0;
	mouthCascade_ = 0;
}

FaceDetector::~FaceDetector()
{
	cvReleaseImage( &gray_ );
}

bool FaceDetector::init(CvSize imgSize)
{
	gray_ = cvCreateImage( imgSize, 8, 1 );
	storage_ = cvCreateMemStorage(0);
	faceCascade_ = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt2.xml", 0, 0, 0 );
	if (!faceCascade_)
	{
		fprintf( stderr, "ERROR: Could not load face classifier cascade\n" );
		exit(-1);
	}
	eyeCascade_ = (CvHaarClassifierCascade*)cvLoad("haarcascade_eye.xml", 0, 0, 0 );
	if (!eyeCascade_)
	{
		fprintf( stderr, "ERROR: Could not load eye classifier cascade\n" );
		exit(-1);
	}
	mouthCascade_ = (CvHaarClassifierCascade*)cvLoad("haarcascade_mouth_norm32x28.xml", 0, 0, 0 );
	if (!mouthCascade_)
	{
		fprintf( stderr, "ERROR: Could not load mouth classifier cascade\n" );
		exit(-1);
	}
	return true;
}

int FaceDetector::detect( IplImage* img, CvSize minFaceSize )
{
	//IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
	/*IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
	cvRound (img->height/scale)),
	8, 1 );*/

	cvCvtColor( img, gray_, CV_BGR2GRAY );
	/*cvResize( gray, small_img, CV_INTER_LINEAR );*/
	cvEqualizeHist( gray_, gray_ );
	cvClearMemStorage( storage_ );


	// double t = (double)cvGetTickCount();
	//CvSeq* faces = cvHaarDetectObjects( small_img, cascade_, storage_,
	//                                    1.1, 3, /*CV_HAAR_DO_CANNY_PRUNING*/ CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH ,
	//                                    cvSize(60,60) );
	faces_ = cvHaarDetectObjects( gray_, faceCascade_, storage_,
		1.1, 40, CV_HAAR_DO_CANNY_PRUNING,
		minFaceSize );
	// t = (double)cvGetTickCount() - t;
	//printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

	// cvReleaseImage( &gray_ );
	//  cvReleaseImage( &small_img );
	return faces_->total;
}

CvSeq* FaceDetector::getFaces()
{
	return faces_;
}

bool FaceDetector::detectFaceFeatures(IplImage* img, CvSize minFaceSize, CvRect * roi)
{
	cvCvtColor( img, gray_, CV_BGR2GRAY );
	cvEqualizeHist( gray_, gray_ );
	cvClearMemStorage( storage_ );

	cvResetImageROI(gray_);
	cvSetImageROI(gray_, *roi );
	CvSize minFeatureSize;
	//minFeatureSize.width = minFaceSize.width / 5;
	//minFeatureSize.height = minFaceSize.height / 10;
	minFeatureSize.width = 5;
	minFeatureSize.height = 5;

	eyes_ = cvHaarDetectObjects( gray_, eyeCascade_, storage_,
		1.1, 1, CV_HAAR_DO_CANNY_PRUNING,
		minFeatureSize );
	if (eyes_->total >= 2)
	{
		cvClearMemStorage( storage_ );
		mouths_ = cvHaarDetectObjects( gray_, mouthCascade_, storage_,
			1.1, 1, CV_HAAR_DO_CANNY_PRUNING,
			minFeatureSize );
		if (mouths_->total >= 1)
		{
			cvResetImageROI(gray_);
			return true;
		}

	}

	cvResetImageROI(gray_);
	return false;
}

CvSeq* FaceDetector::getEyes()
{
	return eyes_;
}

CvSeq* FaceDetector::getMouths()
{
	return mouths_;
}