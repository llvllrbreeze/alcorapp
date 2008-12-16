#include "Face.h"

Face::Face(int i, IplImage *pImg, CvRect * faceRect): tracker(pImg)
{
	id = i;
	tracker.setVmin(10);
	tracker.setSmin(30);
	tracker.startTracking(pImg, faceRect);



}

int Face::getId()
{
	return id;
}

bool Face::isTracking()
{
	return tracking;
}

void Face::setTracking(bool t)
{
	tracking = t;
}

CvBox2D Face::getPosition(IplImage * currFrame)
{
	return tracker.track(currFrame);
}