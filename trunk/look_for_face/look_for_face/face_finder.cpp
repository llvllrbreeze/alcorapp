#include "face_finder.h"

FaceFinder::FaceFinder() : ptu(wxCOM7,wxBAUD_9600)
{
	capture = 0;
	frame_copy = 0;
}

FaceFinder::~FaceFinder()
{
	ptu.clearInput();
	//cvDestroyWindow("result");
}

bool FaceFinder::init()
{
	if ( !detector.init("haarcascade_frontalface_alt2.xml"))
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		return false;
	}
	capture = cvCaptureFromCAM(0);
	if ( !capture )
	{
		fprintf(stderr,"ERROR: Could not open camera stream\n");
		return false;
	}
	cvGrabFrame( capture );
	frame = cvRetrieveFrame( capture );
	CvSize minFaceSize = cvSize(frame->width/20, frame->width/20);
	return true;
}

void FaceFinder::findFace(float pan_speed)
{
	ptu.setSpeed(PTU46_PAN,pan_speed);
	ptu.startMonitoring();
	for(;;)
	{
		if( !cvGrabFrame( capture ))
			break;
		frame = cvRetrieveFrame( capture );
		if( !frame )
			break;
		if( !frame_copy )
			frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
			IPL_DEPTH_8U, frame->nChannels );
		if( frame->origin == IPL_ORIGIN_TL )
			cvCopy( frame, frame_copy, 0 );
		else
			cvFlip( frame, frame_copy, 0 );

		if( detector.detect( frame_copy, &x, &y, &w, &h, 1.3, minFaceSize ) )
		{
			ptu.sendHalt(false);
			cvRectangle(frame_copy, cvPoint(x,y),cvPoint(x+w,y+h),CV_RGB(255,0,0),1,8,0);
			cvNamedWindow( "result", 1 );
			cvShowImage( "result", frame_copy );
			break;
		}
	}
	if(cvWaitKey(-1) >= 0)
		cvDestroyWindow("result");
	exit(0);
}

void FaceFinder::centerFace()
{
	ptu.setSpeed(PTU46_PAN, ptu.getLimit(PTU46_PAN, PTU46_MAX_SPEED));
	ptu.setSpeed(PTU46_TILT, ptu.getLimit(PTU46_TILT, PTU46_MAX_SPEED));
	// TODO
	/*float frame_center_x = frame->width / 2, frame_center_y = frame->height / 2;
	float phi = 5, theta = 5;
	if( detector.detect( frame_copy, &x, &y, &w, &h, 1.3, minFaceSize ) )
	{
		while( sqrt( (x+w/2 - frame_center_x)*(x+w/2 - frame_center_x) + (y+h/2 - frame_center_y)*(y+h/2 - frame_center_y)) > 5 )
		{
			
		}
	}*/
}
