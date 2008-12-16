
#include "PTU46.h"

// Constructor opens the serial port, and read the config info from it
PTU46::PTU46(char * port, wxBaud rate)
{
	int timeout = 1000; //msec
	tr_ = pr_ = 1;

	receivedString = ( char * ) malloc(PTU46_BUFFER_LEN * sizeof(char));
	memset(receivedString,0,sizeof(*receivedString));
	// open the serial port
	// device is a serial port
	dev = new wxSerialPort();
	// try to open the given port
	if(dev->Open(port) < 0) {
		printf("Cannot open %s\n",port);
		delete dev;
		return;
	}
	// set the baudrate
	dev->SetBaudRate(rate);

	// now set up the pan tilt camera
	dev->Write(" ",1); // terse feedback
	sleepms(200);
	while( dev->Read(receivedString,1) == 1){}

	dev->Write("ft ",3); // terse feedback
	dev->Write("ed ",3); // disable echo
	dev->Write("ci ",3);; // position mode

	// delay here so data has arrived at serial port so we can flush it
	sleepms(200);
	while( dev->Read(receivedString,1) == 1){}

	// get pan tilt encoder res
	tr_ = getRes(PTU46_TILT);
	pr_ = getRes(PTU46_PAN);

	PMin_ = getLimit(PTU46_PAN, PTU46_MIN);
	PMax_ = getLimit(PTU46_PAN, PTU46_MAX);
	TMin_ = getLimit(PTU46_TILT, PTU46_MIN);
	TMax_ = getLimit(PTU46_TILT, PTU46_MAX);
	PSMin_ = getLimit(PTU46_PAN, PTU46_MIN_SPEED);
	PSMax_ = getLimit(PTU46_PAN, PTU46_MAX_SPEED);
	TSMin_ = getLimit(PTU46_TILT, PTU46_MIN_SPEED);
	TSMax_ = getLimit(PTU46_TILT, PTU46_MAX_SPEED);

	if (tr_ <= 0 || pr_ <= 0 || PMin_ == 0 || PMax_ == 0 || TMin_ == 0 || TMax_ == 0)
	{
		fprintf(stderr,"Error: Wrong Pan Tilt Limits\n");
	}
}


PTU46::~PTU46()
{
	dev->Close();
	delete dev;
}

// get count/degree resolution
float PTU46::getRes(char type)
{
	char cmd[4] = " r ";
	cmd[0] = type;

	clearInput();
	// get pan res
	int len = 0;
	dev->Write(cmd,3);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len < 3 || receivedString[0] != '*')
	{
		fprintf(stderr,"Error getting pan-tilt res\n");
		return -1;
	}

	receivedString[len] = '\0';
	return strtod(&receivedString[2],NULL)/3600;
}

// get position limit
int PTU46::getLimit(char type, char LimType)
{
	char cmd[4] = "   ";
	cmd[0] = type;
	cmd[1] = LimType;

	clearInput();
	// get limit
	int len = 0;
	dev->Write(cmd,3);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len < 3 || receivedString[0] != '*')
	{
		fprintf(stderr,"Error getting pan-tilt limit\n");
		return -1;
	}

	receivedString[len] = '\0';
	return strtol(&receivedString[2],NULL,0);
}



// get position in degrees
float PTU46::getPos (char type)
{
	char cmd[4] = " p ";
	cmd[0] = type;

	clearInput();
	// get pan pos
	int len = 0;
	dev->Write(cmd,3);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len < 3 || receivedString[0] != '*')
	{
		fprintf(stderr,"Error getting pan-tilt pos\n");
		return -1;
	}

	receivedString[len] = '\0';

	return strtod (&receivedString[2],NULL) * (type == PTU46_TILT ? tr_ : pr_);
}


// set position in degrees
bool PTU46::setPos  (char type, float pos, bool block)
{
	// get raw encoder count to move
	int Count = static_cast<int> (pos/(type == PTU46_TILT ? tr_ : pr_));

	// Check limits
	if (Count < (type == PTU46_TILT ? TMin_ : PMin_) || Count > (type == PTU46_TILT ? TMax_ : PMax_))
	{
		fprintf (stderr,"Pan Tilt Value out of Range: %c %f(%d) (%d-%d)\n", type, pos, Count, (type == PTU46_TILT ? TMin_ : PMin_),(type == PTU46_TILT ? TMax_ : PMax_));
		return false;
	}

	char cmd[16];
	int strlen = snprintf (cmd,16,"%cp%d ",type,Count);

	// set pos
	int len = 0;

	dev->Write (cmd,strlen);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len <= 0 || receivedString[0] != '*')
	{
		fprintf(stderr,"Error setting pan-tilt pos\n");
		return false;
	}

	if (block)
		while (abs(getPos (type) - pos) > 1 );

	return true;
}

bool PTU46::setPanTilt (float pan, float tilt, bool block)
{
	// get raw encoder count to move
	int panStep = static_cast<int> (pan/pr_);
	int tiltStep = static_cast<int> (tilt/tr_);

	// Check limits
	if ( panStep < PMin_ || panStep > PMax_ || tiltStep < TMin_ || tiltStep > TMax_ )
	{
		fprintf (stderr,"Pan Tilt Value out of Range");
		return false;
	}

	char cmd[32];
	int strlen = snprintf (cmd,32,"s pp%d tp%d a ",panStep,tiltStep);

	// set pos
	int len = 0;

	dev->Write (cmd,strlen);

	if (block)
	{
		for(int i = 0; i < 4; i ++)
		{
			memset(receivedString,0,sizeof(*receivedString));
			len = dev->ReadUntilRegEx(&receivedString,"\r\n",5000);
			if (len <= 0 || receivedString[0] != '*')
			{
				fprintf(stderr,"Error setting coordinated pan-tilt pos\n");
				return false;
			}
		}
		while (abs(getPos(PTU46_PAN) - pan) > 1 || abs(getPos(PTU46_TILT) - tilt) > 1 );
	}
	return true;
}

// get speed in degrees/sec
float PTU46::getSpeed (char type)
{
	char cmd[4] = " s ";
	cmd[0] = type;

	clearInput();
	// get speed
	int len = 0;
	dev->Write(cmd,3);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len < 3 || receivedString[0] != '*')
	{
		fprintf (stderr,"Error getting pan-tilt speed\n");
		return -1;
	}

	receivedString[len] = '\0';

	return strtod(&receivedString[2],NULL) * (type == PTU46_TILT ? tr_ : pr_);
}



// set speed in degrees/sec
bool PTU46::setSpeed (char type, float pos)
{
	// get raw encoder speed to move
	int Count = static_cast<int> (pos/(type == PTU46_TILT ? tr_ : pr_));
	// Check limits
	if (abs(Count) < (type == PTU46_TILT ? TSMin_ : PSMin_) || abs(Count) > (type == PTU46_TILT ? TSMax_ : PSMax_))
	{
		fprintf (stderr,"Pan Tilt Speed Value out of Range: %c %f(%d) (%d-%d)\n", type, pos, Count, (type == PTU46_TILT ? TSMin_ : PSMin_),(type == PTU46_TILT ? TSMax_ : PSMax_));
		return false;
	}

	char cmd[16];
	int strlen = snprintf(cmd,16,"%cs%d ",type,Count);

	// set speed
	int len = 0;
	dev->Write (cmd, strlen);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len <= 0 || receivedString[0] != '*')
	{
		fprintf (stderr,"Error setting pan-tilt speed\n");
		return false;
	}
	return true;
}


// set movement mode (position/velocity)
bool PTU46::setMode (char type)
{
	char cmd[4] = "c  ";
	cmd[1] = type;

	// set mode
	int len = 0;
	dev->Write (cmd, 3);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len <= 0 || receivedString[0] != '*')
	{
		fprintf (stderr,"Error setting pan-tilt move mode\n");
		return false;
	}
	return true;
}

// get position in degrees
char PTU46::getMode ()
{
	clearInput();
	// get pan tilt mode
	int len = 0;
	dev->Write ("c ", 2);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len < 3 || receivedString[0] != '*')
	{
		fprintf (stderr,"Error getting mode\n");
		return -1;
	}

	if (receivedString[2] == 'p')
		return PTU46_VELOCITY;
	else if (receivedString[2] == 'i')
		return PTU46_POSITION;
	else
		return -1;
}

bool PTU46::sendImmediatePositionCommand()
{
	// get pan tilt mode
	int len = 0;
	dev->Write ("i ", 2);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len < 3 || receivedString[0] != '*')
	{
		fprintf (stderr,"Error getting mode\n");
		return false;
	}
	return true;
}

bool PTU46::sendHalt(bool wait)
{
	// halt current execution
	int len = 0;
	dev->Write ("h ", 2);
	if (wait)
	{
		memset(receivedString,0,sizeof(*receivedString));
		len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

		if (len < 3 || receivedString[0] != '*')
		{
			fprintf (stderr,"Error halting PTU\n");
			return false;
		}
	}
	return true;
}

bool PTU46::startMonitoring()
{
	// halt current execution
	int len = 0;
	dev->Write ("m ", 2);
	memset(receivedString,0,sizeof(*receivedString));
	len = dev->ReadUntilRegEx(&receivedString,"\r\n",1000);

	if (len < 3 || receivedString[0] != '*')
	{
		fprintf (stderr,"Error halting PTU\n");
		return false;
	}
	return true;
}

void PTU46::clearInput()
{
	while( dev->Read(receivedString,1) == 1){}
}