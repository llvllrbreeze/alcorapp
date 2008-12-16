#include "detector.h"

FaceDetector::FaceDetector()
{
	storage_ = 0;
	cascade_ = 0;
}

FaceDetector::~FaceDetector()
{
}

bool FaceDetector::init(const char * fileName)
{
	storage_ = cvCreateMemStorage(0);
	cascade_ = (CvHaarClassifierCascade*)cvLoad( fileName, 0, 0, 0 );
	if (!cascade_)
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return false;
	}
	return true;
}

bool FaceDetector::detect( IplImage* img, int * center_x, int * center_y, int * width, int * height, double scale, CvSize minFaceSize )
{
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    /*IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)),
									8, 1 );*/

    cvCvtColor( img, gray, CV_BGR2GRAY );
    /*cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );*/
    cvClearMemStorage( storage_ );


       // double t = (double)cvGetTickCount();
        //CvSeq* faces = cvHaarDetectObjects( small_img, cascade_, storage_,
        //                                    1.1, 3, /*CV_HAAR_DO_CANNY_PRUNING*/ CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH ,
        //                                    cvSize(60,60) );
		 CvSeq* faces = cvHaarDetectObjects( gray, cascade_, storage_,
                                            1.1, 60,  /*CV_HAAR_DO_CANNY_PRUNING*/ CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH ,
											minFaceSize );
       // t = (double)cvGetTickCount() - t;
        //printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

		if(faces->total > 0)
		{
			resultRect_ = ( CvRect* )cvGetSeqElem( faces, 0 );
			/**center_x = resultRect_->x*scale;
			*center_y = resultRect_->y*scale;
			*width = resultRect_->width*scale;
			*height = resultRect_->height*scale;*/
			*center_x = resultRect_->x;
			*center_y = resultRect_->y;
			*width = resultRect_->width;
			*height = resultRect_->height;
			cvReleaseImage( &gray );
			//cvReleaseImage( &small_img );
			return true;
		}
    cvReleaseImage( &gray );
  //  cvReleaseImage( &small_img );
	return false;
}