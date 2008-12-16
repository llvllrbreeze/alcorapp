#include <stdio.h>
#include "FrameCapturer.h"

FrameCapturer::FrameCapturer()
{
	IplImage * frame;
	capture_ = 0;
	capture_ = cvCaptureFromCAM(0);
	if ( !capture_ )
	{
		fprintf(stderr,"ERROR: Could not open camera stream\n");
		exit(-1);
	}
	cvGrabFrame( capture_ );
	frame = cvRetrieveFrame( capture_ );
	frameSize_.width = frame->width;
	frameSize_.height = frame->height;
	numChannels_ = frame->nChannels;
}

FrameCapturer::~FrameCapturer()
{
}

int FrameCapturer::getFrameHeight()
{
	return frameSize_.height;
}
int FrameCapturer::getFrameWidth()
{
	return frameSize_.width;
}

int FrameCapturer::getNumChannels()
{
	return numChannels_;
}

IplImage * FrameCapturer::getFrame()
{
	if( !cvGrabFrame( capture_ ))
			return 0;
	return cvRetrieveFrame( capture_ );
}

