#include "cv.h"
#include "highgui.h"
#include "detector.h"
#include "PTU46.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

class FaceFinder
{
private:
	int x, y, w, h;
    CvCapture* capture;
    IplImage *frame, *frame_copy;
	FaceDetector detector;
	PTU46 ptu;
	CvSize minFaceSize;
	void centerFace();
public:
	
	FaceFinder();
	~FaceFinder();
	bool init();
	void findFace(float pan_speed);
};