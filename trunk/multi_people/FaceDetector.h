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
	IplImage* gray_;
	CvMemStorage* storage_;
	CvHaarClassifierCascade * faceCascade_, * eyeCascade_, * mouthCascade_ ;
	CvSeq * faces_, * eyes_, * mouths_;

	

public:
	FaceDetector( );
	~FaceDetector();
	bool init(CvSize imgSize);
	int detect(IplImage * img, CvSize minFaceSize);
	CvSeq* getFaces();
	bool detectFaceFeatures(IplImage * img, CvSize minFaceSize, CvRect * roi);
	CvSeq* getEyes();
	CvSeq* getMouths();

};
