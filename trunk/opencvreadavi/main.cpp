#include <cv.h>
#include <highgui.h>
#include <cstdio>
//#include "stdafx.h"

//giusto per disegnare ...
#pragma comment (lib, "highgui.lib")
#pragma comment (lib, "cv.lib")
#pragma comment (lib, "cxcore.lib")

int main(int argc, char* argv[])
{
  char* to_open;

  if (argc>1)
    to_open = argv[1];
  else
    to_open = "videoinput.avi";

  printf("creating capture\n");
  CvCapture* avifile = cvCreateFileCapture( to_open  );
  printf("CApture object created\n");

  IplImage* frame;
  cvNamedWindow(to_open);
  //
  if (avifile!=0)
  {
    double nframes =  cvGetCaptureProperty( avifile,  CV_CAP_PROP_FRAME_COUNT  );
    printf("opened %s, # of frames: %.0f entering loop\n",to_open, nframes);
    for(;;)
    {
      frame = cvQueryFrame( avifile );
      if(frame == 0){printf("IplImage is null\n"); break;}
      cvShowImage(to_open,frame);

      cvWaitKey(100);
    }

    cvReleaseCapture( &avifile  );
  }

  return 0;
}