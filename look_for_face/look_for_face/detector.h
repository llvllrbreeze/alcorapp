#include "cv.h"
#include "highgui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

class FaceDetector
{
private:
	CvMemStorage* storage_;
	CvHaarClassifierCascade* cascade_;
	CvRect * resultRect_;

public:
	FaceDetector( );
	~FaceDetector();
	bool init(const char *);
	bool detect(IplImage * img, int * center_x, int * center_y, int * width, int * height, double scale, CvSize minFaceSize);
};
