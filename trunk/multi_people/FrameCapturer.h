#include "cv.h"
#include "highgui.h"

class FrameCapturer
{
private:
	CvSize frameSize_;
	int numChannels_;
	CvCapture* capture_;
public:
	FrameCapturer();
	~FrameCapturer();
	int getFrameHeight();
	int getFrameWidth();
	int getNumChannels();
	IplImage * getFrame();
};