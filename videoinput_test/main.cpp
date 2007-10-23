#include "alcor.extern/videoinput/videoinput.h"
#include <cv.h>
#include <highgui.h>

//#include "stdafx.h"

//giusto per disegnare ...
#pragma comment (lib, "highgui.lib")
#pragma comment (lib, "cv.lib")
#pragma comment (lib, "cxcore.lib")
//NOTA! messa in alcor.lib
#pragma comment(lib, "videoInput.lib")

int main()
{
  //optional static function to list devices
	//for silent listDevices use listDevices(true);
	int numDevices = videoInput::listDevices();

  //create a videoInput object
	videoInput VI;
  //
  int device1 = 0; 

  //
  VI.setupDevice(device1, 640, 480);
	//to get a settings dialog for the device
	VI.showSettingsWindow(device1);
  //
	int width 	= VI.getWidth(device1);
	int height 	= VI.getHeight(device1);
	int size	  = VI.getSize(device1);

  //
  unsigned char* frame = new unsigned char[VI.getSize(device1)];

  //
  IplImage* my_cvimage  = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  IplImage* my_image    = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	//
  cvNamedWindow("videoinput");

  for(;;)
    {

	//to get the data from the device first check if the data is new
	if(VI.isFrameNew(device1))
	{
    if ( VI.getPixels(device1, frame, false) )//returns pixels as a BGR (for openCV) unsigned char array  
    {
      //
      memcpy(my_cvimage->imageData, frame, my_cvimage->imageSize);

      //cvConvertImage(my_cvimage, my_image, CV_CVTIMG_FLIP);

      cvShowImage("videoinput", my_cvimage);
    }
	}

  if( cvWaitKey(10) >= 0 )
      break;
  }
	//Shut down devices properly
	VI.stopDevice(device1);

  //
  delete [] frame;
  //cvReleaseImage(my_cvimage);
}