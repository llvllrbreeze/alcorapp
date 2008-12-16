////#include <vector>
////#include <stdio.h>
////#include "cv.h"
////#include "highgui.h"
////#include "CamShiftTracker.h"
////#include "FaceDetector.h"
////
////
////
////void main()
////{
////
////
////	IplImage *TheImageFromTheCamera, *frame_copy;
////	IplImage *TheBackImageForDebugging;
////	CvRect * r;
////
////	int tracking = 0, bp = 0, si = 0;
////
////
////
////	/*
////	** Start up the camera and open a window for output.
////	*/
////	CvCapture *capture = cvCaptureFromCAM(0);
////	cvNamedWindow( "Source", 1 );
////	cvNamedWindow( "Backprojection", 1 );
////
////
////	cvGrabFrame(capture );
////	TheImageFromTheCamera = cvRetrieveFrame( capture );
////
////	frame_copy = cvCreateImage(cvSize(TheImageFromTheCamera->width, TheImageFromTheCamera->height),IPL_DEPTH_8U,TheImageFromTheCamera->nChannels);
////
////	FaceDetector detector;
////	detector.init(cvSize(frame_copy->width,frame_copy->height));
////
////	std::vector<CamShiftTracker *> faces;
////
////
////	if(!TheImageFromTheCamera)
////		printf("ERROR grabbing image!\n");
////
////	if( TheImageFromTheCamera->origin == IPL_ORIGIN_TL )
////		cvCopy( TheImageFromTheCamera, frame_copy, 0 );
////	else
////		cvFlip( TheImageFromTheCamera, frame_copy, 0 );
////
////	CvSize minFaceSize;
////	minFaceSize.height = frame_copy->width/10; 
////	minFaceSize.width = frame_copy->width/10; 
////
////
////	int i;
////	CvBox2D faceBox;
////	int numFaces;
////
////	while(1)
////	{
////		/*
////		** Get a frame from the camera
////		*/
////		cvGrabFrame(capture );
////		TheImageFromTheCamera = cvRetrieveFrame( capture );
////		if( TheImageFromTheCamera->origin == IPL_ORIGIN_TL )
////			cvCopy( TheImageFromTheCamera, frame_copy, 0 );
////		else
////			cvFlip( TheImageFromTheCamera, frame_copy, 0 );
////
////
////		if(tracking)
////		{	
////			for( size_t j = 0; j < faces.size(); j ++)
////			{
////				/*
////				** The moment we all came here for....
////				*/
////				faceBox = faces.at(j)->track(TheImageFromTheCamera);
////				if (bp)
////					cvShowImage("Backprojection", faces.at(j)->getBackProjection());
////				if(si)
////				{
////					cvSaveImage("backprojection.tiff",faces.at(j)->getBackProjection());
////					si = 0;
////				}
////				/*
////				** Get the bounding box and find the center of it.
////				*/
////
////				cvEllipseBox(frame_copy, faceBox,CV_RGB(255,0,0), 3, CV_AA, 0 );
////			}
////		}
////		else
////		{
////			numFaces = detector.detect(frame_copy, 1.3, minFaceSize);
////
////			if(numFaces == 1)
////			{
////			for ( i = 0; i < ( numFaces > 0 ? numFaces : 0); i++ )
////			{
////				r = (CvRect*) cvGetSeqElem(detector.getFaces(), i);
////				cvRectangle(frame_copy, cvPoint(r->x, r->y), cvPoint(r->x+r->width, r->y+r->height), CV_RGB(255, 0, 0), 2, 8 , 0);
////				CamShiftTracker * tracker = new CamShiftTracker(frame_copy);
////
////				// Set Camshift parameters
////				tracker->setVmin(50);
////				tracker->setSmin(50);
////
////				tracker->startTracking(frame_copy, r);
////				faces.push_back(tracker);
////
////				tracking = 1;
////			}
////			}
////
////		}
////		cvShowImage( "Source", frame_copy );
////
////
////		char c = cvWaitKey(10);
////		if(c == 27)
////			break;
////		else if (c == 'b')
////			bp ^= 1;
////		else if (c == 's')
////		{
////			si = 1; 
////		}
////			
////	}
////	for( size_t j = 0; j < faces.size(); j ++)
////	{
////		delete faces.at(j);
////	}
////	cvReleaseCapture(&capture);
////	cvDestroyWindow("Source");
////	cvDestroyWindow("Backprojection");
////	return;
////}
//
//#include <vector>
//#include <stdio.h>
//#include "cv.h"
//#include "highgui.h"
//#include "CamShiftTracker.h"
//#include "FaceDetector.h"
//
//
//
//void main()
//{
//
//
//	IplImage *TheImageFromTheCamera, *frame_copy;
//	IplImage *TheBackImageForDebugging;
//	CvRect * r;
//
//	int tracking = 0, bp = 0, si = 0;
//
//
//
//	/*
//	** Start up the camera and open a window for output.
//	*/
//	CvCapture *capture = cvCaptureFromCAM(0);
//	cvNamedWindow( "Source", 1 );
//	cvNamedWindow( "Backprojection", 1 );
//
//
//	cvGrabFrame(capture );
//	TheImageFromTheCamera = cvRetrieveFrame( capture );
//
//	frame_copy = cvCreateImage(cvSize(TheImageFromTheCamera->width, TheImageFromTheCamera->height),IPL_DEPTH_8U,TheImageFromTheCamera->nChannels);
//
//	FaceDetector detector;
//	detector.init("haarcascade_frontalface_alt2.xml",cvSize(frame_copy->width,frame_copy->height));
//
//	std::vector<CamShiftTracker *> faces;
//
//
//	if(!TheImageFromTheCamera)
//		printf("ERROR grabbing image!\n");
//
//	if( TheImageFromTheCamera->origin == IPL_ORIGIN_TL )
//		cvCopy( TheImageFromTheCamera, frame_copy, 0 );
//	else
//		cvFlip( TheImageFromTheCamera, frame_copy, 0 );
//
//	CvSize minFaceSize;
//	minFaceSize.height = frame_copy->width/10; 
//	minFaceSize.width = frame_copy->width/10; 
//
//
//	int i;
//	CvBox2D faceBox;
//	int numFaces;
//
//	while(1)
//	{
//		/*
//		** Get a frame from the camera
//		*/
//		cvGrabFrame(capture );
//		TheImageFromTheCamera = cvRetrieveFrame( capture );
//		if( TheImageFromTheCamera->origin == IPL_ORIGIN_TL )
//			cvCopy( TheImageFromTheCamera, frame_copy, 0 );
//		else
//			cvFlip( TheImageFromTheCamera, frame_copy, 0 );
//
//
//		if(tracking)
//		{	
//			for( size_t j = 0; j < faces.size(); j ++)
//			{
//				/*
//				** The moment we all came here for....
//				*/
//				faceBox = faces.at(j)->track(TheImageFromTheCamera);
//				if (bp)
//					cvShowImage("Backprojection", faces.at(j)->getBackProjection());
//				if(si)
//				{
//					cvSaveImage("backprojection.tiff",faces.at(j)->getBackProjection());
//					si = 0;
//				}
//				/*
//				** Get the bounding box and find the center of it.
//				*/
//
//				cvEllipseBox(frame_copy, faceBox,CV_RGB(255,0,0), 3, CV_AA, 0 );
//			}
//		}
//		else
//		{
//			numFaces = detector.detect(frame_copy, 1.3, minFaceSize);
//
//			if(numFaces == 1)
//			{
//			for ( i = 0; i < ( numFaces > 0 ? numFaces : 0); i++ )
//			{
//				r = (CvRect*) cvGetSeqElem(detector.getFaces(), i);
//				cvRectangle(frame_copy, cvPoint(r->x, r->y), cvPoint(r->x+r->width, r->y+r->height), CV_RGB(255, 0, 0), 2, 8 , 0);
//				CamShiftTracker * tracker = new CamShiftTracker(frame_copy);
//
//				// Set Camshift parameters
//				tracker->setVmin(50);
//				tracker->setSmin(50);
//
//				tracker->startTracking(frame_copy, r);
//				faces.push_back(tracker);
//
//				tracking = 1;
//			}
//			}
//
//		}
//		cvShowImage( "Source", frame_copy );
//
//
//		char c = cvWaitKey(10);
//		if(c == 27)
//			break;
//		else if (c == 'b')
//			bp ^= 1;
//		else if (c == 's')
//		{
//			si = 1; 
//		}
//			
//	}
//	for( size_t j = 0; j < faces.size(); j ++)
//	{
//		delete faces.at(j);
//	}
//	cvReleaseCapture(&capture);
//	cvDestroyWindow("Source");
//	cvDestroyWindow("Backprojection");
//	return;
//}
