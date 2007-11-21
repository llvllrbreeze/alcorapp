#include <cv.h>
#include <highgui.h>
#include <cstdio>
//#include "stdafx.h"

//giusto per disegnare ...
#pragma comment (lib, "highgui.lib")
#pragma comment (lib, "cv.lib")
#pragma comment (lib, "cxcore.lib")

#define DO_KLT_TRACK

#ifdef DO_KLT_TRACK
#include "alcor/core/klt_tracker_t.h"
#include <string>
#pragma comment (lib,"libklt.lib")
using namespace all;
#define _STRNG(_some_c_str_) std::string(_some_c_str_)
#endif


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

#ifdef DO_KLT_TRACK
  all::core::klt_tracker_t klt;
  KLT_FeatureList FL;
  klt.init(_STRNG("config/klt_conf.ini") );
  IplImage* current;
  char* greywin = "GRAY IMAGE";
  cvNamedWindow(greywin);
#else
    cvNamedWindow(to_open);
#endif

  IplImage* frame;

  //
  if (avifile!=0)
  {
    double nframes =  cvGetCaptureProperty( avifile,  CV_CAP_PROP_FRAME_COUNT  );

#ifdef DO_KLT_TRACK
    frame = cvQueryFrame( avifile );
    printf("MAIN:: creating IplImages\n");

    current = cvCreateImage(cvSize(frame->width,frame->height), frame->depth, 1);

    cvConvertImage(frame,   current, CV_BGR2GRAY|CV_CVTIMG_FLIP);

    klt.set_first_image((unsigned char*)current->imageData);

#endif

    printf("opened %s, # of frames: %.0f entering loop\n", to_open, nframes);
    for(;;)
    {
      frame = cvQueryFrame( avifile );
      if(frame == 0){printf("IplImage is null\n"); break;}

#ifdef DO_KLT_TRACK
      cvConvertImage(frame, current, CV_BGR2GRAY|CV_CVTIMG_FLIP);

      cvShowImage(greywin, current);
      klt.track((unsigned char*)current->imageData);

      FL = klt.status();
      printf("MAIN::Tracked %d features\n\n", FL->nFeatures);
      //klt.log_status();
#else
      cvShowImage(to_open,frame);
#endif

      cvWaitKey(1);
    }

#ifdef DO_KLT_TRACK    
    cvReleaseImage(&current);
#endif

    cvReleaseCapture( &avifile  );
  }

  return 0;
}