//
//#include <stdio.h>
//#include "cv.h"
//#include "highgui.h"
//
//#include "FaceDetector.h"
//
//
//
//void main()
//{
//
//
//	IplImage *TheImageFromTheCamera, *frame_copy;
//
//	
//
//	/*
//	** Start up the camera and open a window for output.
//	*/
//	CvCapture *capture = cvCaptureFromCAM(0);
//	cvNamedWindow( "Source", 1 );
//
//	cvGrabFrame(capture );
//	TheImageFromTheCamera = cvRetrieveFrame( capture );
//
//	frame_copy = cvCreateImage(cvSize(TheImageFromTheCamera->width, TheImageFromTheCamera->height),IPL_DEPTH_8U,TheImageFromTheCamera->nChannels);
//
//	FaceDetector detector;
//	detector.init(cvSize(frame_copy->width,frame_copy->height));
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
//	CvRect face;
//
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
//		numFaces = detector.detect(frame_copy, minFaceSize);
//
//		if(numFaces == 1)
//		{
//
//			CvRect * r = (CvRect*)cvGetSeqElem(detector.getFaces(), 0);
//			face.height = r->height;
//			face.width = r->width;
//			face.x = r->x;
//			face.y = r->y;
//			
//			
//			if(detector.detectFaceFeatures(frame_copy, minFaceSize, &face))
//			{
//				
//				lEyeRect = (CvRect*) cvGetSeqElem(detector.getEyes(), 0);
//				rEyeRect = (CvRect*) cvGetSeqElem(detector.getEyes(), 1);
//				mouthRect = (CvRect*) cvGetSeqElem(detector.getMouths(), 0);
//
//				
//				cvRectangle(frame_copy, cvPoint(lEyeRect->x, lEyeRect->y), cvPoint(lEyeRect->x+lEyeRect->width, lEyeRect->y+lEyeRect->height), CV_RGB(0, 255, 0), 2, 8 , 0);
//				cvRectangle(frame_copy, cvPoint(rEyeRect->x, rEyeRect->y), cvPoint(rEyeRect->x+rEyeRect->width, rEyeRect->y+rEyeRect->height), CV_RGB(0, 255, 0), 2, 8 , 0);
//				cvRectangle(frame_copy, cvPoint(mouthRect->x, mouthRect->y), cvPoint(mouthRect->x+mouthRect->width, mouthRect->y+mouthRect->height), CV_RGB(0, 0, 255), 2, 8 , 0);
//			}
//			cvRectangle(frame_copy, cvPoint(faceRect->x, faceRect->y), cvPoint(faceRect->x+faceRect->width, faceRect->y+faceRect->height), CV_RGB(255, 0, 0), 2, 8 , 0);
//		}
//		
//		cvShowImage( "Source", frame_copy );
//		
//
//		char c = cvWaitKey(50);
//		if(c == 27)
//			break;
//
//	}
//
//	cvReleaseCapture(&capture);
//	cvDestroyWindow("Source");
//	return;
//}
