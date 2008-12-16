#include "cv.h"
#include "highgui.h"
#include "CamShiftTracker.h"
#include "CV.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

class Face
{
private:
	int id;
	bool tracking;
	CamShiftTracker tracker;
	
public:
	Face(int i, IplImage *pImg, CvRect * faceRect);
	int getId();
	bool isTracking();
	void setTracking(bool t);
	CvBox2D getPosition(IplImage * currFrame);
};