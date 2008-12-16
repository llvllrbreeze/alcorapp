#include "wxctb/iobase.h"
#include "wxctb/serport.h"
#include "wxctb/timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PTU46_BUFFER_LEN 256

// command defines
#define PTU46_PAN 'p'
#define PTU46_TILT 't'
#define PTU46_MIN 'n'
#define PTU46_MAX 'x'
#define PTU46_MIN_SPEED 'l'
#define PTU46_MAX_SPEED 'u'
#define PTU46_VELOCITY 'v'
#define PTU46_POSITION 'i'

//
// Pan-Tilt Control Class
//

class PTU46
{
    public:
	PTU46(char * port, wxBaud rate);
	~PTU46();

	// get count/degree resolution
	float getRes (char type);
	// get position limit
	int getLimit (char type, char limType);

	// get/set position in degrees
	float getPos (char type);
	bool setPos  (char type, float pos, bool block = false);
	// coordinated
	bool setPanTilt (float pan, float tilt, bool block = false);

	// get/set speed in degrees/sec
	bool setSpeed  (char type, float speed);
	float getSpeed (char type);

	// get/set move mode
	bool setMode (char type);
	char getMode ();

	bool sendImmediatePositionCommand();
	bool sendHalt(bool wait);

	bool startMonitoring();

	void clearInput();

	// Position Limits
	int TMin_, TMax_, PMin_, PMax_;
	// Speed Limits
	int TSMin_, TSMax_, PSMin_, PSMax_;

    protected:
	// pan and tilt resolution
	float tr_ , pr_ ;

	// serial port device
	wxSerialPort* dev;

	// read buffer
	char * receivedString;
	
};