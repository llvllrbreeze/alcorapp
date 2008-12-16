
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

#include "FaceDetector.h"



void main()
{


	IplImage *TheImageFromTheCamera, *frame_copy, *cropped, * gray;

	CvRect * r, m;

	/*
	** Start up the camera and open a window for output.
	*/
	CvCapture *capture = cvCaptureFromCAM(0);
	cvNamedWindow( "Source", 1 );
	cvNamedWindow( "Cropped", 1 );

	cvGrabFrame(capture );
	TheImageFromTheCamera = cvRetrieveFrame( capture );

	frame_copy = cvCreateImage(cvSize(TheImageFromTheCamera->width, TheImageFromTheCamera->height),IPL_DEPTH_8U,TheImageFromTheCamera->nChannels);
	gray = cvCreateImage(cvSize(TheImageFromTheCamera->width, TheImageFromTheCamera->height),IPL_DEPTH_8U,1);
	cropped = cvCreateImage(cvSize(92, 112),IPL_DEPTH_8U,1);


	FaceDetector detector;
	detector.init(cvSize(frame_copy->width,frame_copy->height));

	if(!TheImageFromTheCamera)
		printf("ERROR grabbing image!\n");

	if( TheImageFromTheCamera->origin == IPL_ORIGIN_TL )
		cvCopy( TheImageFromTheCamera, frame_copy, 0 );
	else
		cvFlip( TheImageFromTheCamera, frame_copy, 0 );

	CvSize minFaceSize;
	minFaceSize.height = frame_copy->width/10; 
	minFaceSize.width = frame_copy->width/10; 


	int numFaces, cx, cy;

	while(1)
	{
		/*
		** Get a frame from the camera
		*/
		cvGrabFrame(capture );
		TheImageFromTheCamera = cvRetrieveFrame( capture );
		if( TheImageFromTheCamera->origin == IPL_ORIGIN_TL )
			cvCopy( TheImageFromTheCamera, frame_copy, 0 );
		else
			cvFlip( TheImageFromTheCamera, frame_copy, 0 );

		numFaces = detector.detect(frame_copy,minFaceSize);

		if(numFaces == 1)
		{
			r = (CvRect*) cvGetSeqElem(detector.getFaces(), 0);
			
			cx = r->x + r->width / 2;
			cy = r->y + r->height / 2;
			m.x = cx - 46;
			m.width = 92;
			m.y = cy - 56;
			m.height = 112;
			cvCvtColor(frame_copy, gray, CV_RGB2GRAY);
			cvSetImageROI(gray,m);
			//cvCopy(frame_copy,cropped,0);
			cvResize(gray,cropped,CV_INTER_LINEAR);
			cvResetImageROI(gray);

			IplImage * mask = cvCreateImage(cvSize(cropped->width,cropped->height),8,1);
			cvZero(mask);
			cvEllipse(mask,cvPoint(mask->width/2, mask->height/2),cvSize(mask->width/2,(mask->height*3)/5),0,0,360,cvScalar(255),CV_FILLED,0,0);
			cvAnd(mask, cropped ,cropped);

			cvShowImage( "Cropped", cropped);
			cvRectangle(frame_copy, cvPoint(r->x, r->y), cvPoint(r->x+r->width, r->y+r->height), CV_RGB(255, 0, 0), 2, 8 , 0);
		}
		
		cvShowImage( "Source", frame_copy );
		

		char c = cvWaitKey(10);
		if(c == 27)
			break;

	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("Source");
	cvDestroyWindow("Cropped");
	return;
}


void segmentAFace(IplImage * origIntImage, IplImage * redIntImage, CvPoint centerCoord, CvPoint lEyeCoord, CvPoint rEyeCoord)
{
	double eyeDist = sqrt( (double) ((lEyeCoord.x - rEyeCoord.x)*(lEyeCoord.x - rEyeCoord.x) +
							(lEyeCoord.y - rEyeCoord.y)*(lEyeCoord.y - rEyeCoord.y)) );
	const double d = 35;
	
	IplImage * mask = cvCreateImage(cvSize(redIntImage->width,redIntImage->height),8,1);
	cvZero(mask);
	cvEllipse(mask,cvPoint(mask->width/2, mask->height/2),cvSize(mask->width,mask->height),0,0,360,cvScalar(255),CV_FILLED,0,0);
	
	double scale = eyeDist / d;

	CvRect roi;

	roi.x = centerCoord.x - (origIntImage->width * scale) / 2;
	roi.y = centerCoord.y - (origIntImage->height * scale) / 2;
	roi.width = origIntImage->width * scale;
	roi.height = origIntImage->height * scale;

	cvSetImageROI(origIntImage, roi);
	cvResize(origIntImage, redIntImage,CV_INTER_LINEAR);
	cvResetImageROI(origIntImage);

	cvAnd(redIntImage,mask,redIntImage);
}