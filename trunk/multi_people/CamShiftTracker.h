#include <stdio.h>
#include "cv.h"


class CamShiftTracker
{
public:

	CamShiftTracker(const IplImage * pImg);
	~CamShiftTracker();
	void    startTracking(IplImage * pImg, CvRect * pRect);
	CvBox2D track(IplImage * pImg);
	IplImage * getBackProjection();

	// Parameter settings
	void setVmin(int vmin);
	void setSmin(int smin);

private:

	// Parameters
	int   nHistBins;                 // number of histogram bins
	float rangesArr[2];          // histogram range
	int vmin, vmax, smin; // limits for calculating hue


	// File-level variables
	IplImage * pHSVImg; // the input image converted to HSV color mode
	IplImage * pHueImg; // the Hue channel of the HSV image
	IplImage * pMask; // this image is used for masking pixels
	IplImage * pProbImg; // the face probability estimates for each pixel
	CvHistogram * pHist; // histogram of hue in the original face image

	CvRect prevFaceRect;  // location of face in previous frame
	CvBox2D faceBox;      // current face-location estimate


	int nFrames;

	// Declarations for internal functions
	void updateHueImage(const IplImage * pImg);

};