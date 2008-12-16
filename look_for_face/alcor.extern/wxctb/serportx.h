/////////////////////////////////////////////////////////////////////////////
// Name:        serportx.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: serportx.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SERIALPORT_X_H
#define _WX_SERIALPORT_X_H

#include <stdio.h>
#include "iobase.h"

#if defined (WIN32)
# define snprintf _snprintf
#endif

/**
   \file serportx.h
*/

#define WXSERIALPORT_NAME_LEN 32

/*!
  \enum wxBaud

  wxBaud covers the valid baudrates. Until wxBAUD_38400 (means a
  baudrate of 38400 baud) should be supported by every PC. In some
  circumstances, greater baudrates require a serial FIFO. But this
  should be built in, in the latest PCs.
*/
enum wxBaud
{
    /*! 150 baud */
    wxBAUD_150=150,
    /*! 300 baud */
    wxBAUD_300=300,       
    /*! 600 baud */
    wxBAUD_600=600,
    /*! 1200 baud */
    wxBAUD_1200=1200,
    /*! 2400 baud */
    wxBAUD_2400=2400,
    /*! 4800 baud */
    wxBAUD_4800=4800,
    /*! 9600 baud */
    wxBAUD_9600=9600,
    /*! 19200 baud */
    wxBAUD_19200=19200,
    /*! 38400 baud */
    wxBAUD_38400=38400,
    /*! 57600 baud */
    wxBAUD_57600=57600,
    /*! 115200 baud */
    wxBAUD_115200=115200,
    /*! 230400 baud */
    wxBAUD_230400=230400,
    /*! 460800 baud */
    wxBAUD_460800=460800,
    /*! 921600 baud */
    wxBAUD_921600=921600
};
/*!
  \enum wxParity

  Defines the different modes of parity checking. Under
  Linux, the struct termios will be set to provide the wanted
  behaviour.
*/
enum wxParity
{
    /*! no parity check */
    wxPARITY_NONE,
    /*! odd parity check */
    wxPARITY_ODD,
    /*! even parity check */
    wxPARITY_EVEN,
    /*! mark (not implemented yet) */
    wxPARITY_MARK,
    /*! space (not implemented yet) */
    wxPARITY_SPACE
};

/*!
  \enum wxSerialLineState

  Defines the different modem control lines. The value for
  each item are defined in /usr/include/bits/ioctl-types.h.
  This is the linux definition. The window version translate
  each item in it's own value.
  modem lines defined in ioctl-types.h
  #define TIOCM_LE	0x001
  #define TIOCM_DTR	0x002
  #define TIOCM_RTS	0x004
  #define TIOCM_ST	0x008
  #define TIOCM_SR	0x010
  #define TIOCM_CTS	0x020
  #define TIOCM_CAR	0x040
  #define TIOCM_RNG	0x080
  #define TIOCM_DSR	0x100
  #define TIOCM_CD	TIOCM_CAR
  #define TIOCM_RI	TIOCM_RNG

*/
enum wxSerialLineState
{
    /*! Data Carrier Detect (read only) */
    wxSERIAL_LINESTATE_DCD = 0x040,
    /*! Clear To Send (read only) */
    wxSERIAL_LINESTATE_CTS = 0x020,
    /*! Data Set Ready (read only) */
    wxSERIAL_LINESTATE_DSR = 0x100,
    /*! Data Terminal Ready (write only) */
    wxSERIAL_LINESTATE_DTR = 0x002,
    /*! Ring Detect (read only) */
    wxSERIAL_LINESTATE_RING = 0x080,
    /*! Request To Send (write only) */
    wxSERIAL_LINESTATE_RTS = 0x004,
    /*! no active line state, use this for clear */
    wxSERIAL_LINESTATE_NULL = 0x000
};

/*!
  \struct wxSerialPort_DCS

  The device control struct for the serial communication class.
  This struct should be used, if you refer advanced parameter.
*/
struct wxSerialPort_DCS
{
    /*! the baudrate */
    wxBaud baud;
    /*! the parity */
    wxParity parity;
    /*! the wordlen */
    unsigned char wordlen;
    /*! count of stopbits */
    unsigned char stopbits;
    /*! rtscts flow control */
    bool rtscts;
    /*! XON/XOFF flow control */
    bool xonxoff;
    wxSerialPort_DCS() {
        baud = wxBAUD_19200;
	   parity = wxPARITY_NONE;
	   wordlen = 8;
	   stopbits = 1;
	   rtscts = false;
	   xonxoff = false;
    };
    char* GetSettings(char* buf,size_t bufsize) {
	   const char ac[5] = {'N','O','E','M','S'};
	   snprintf(buf,bufsize,"%i%c%i %i",
			  wordlen,
			  ac[parity],
			  stopbits,
			  baud);
	   return buf;
    };
}; 

/*!
  \struct wxSerialPort_EINFO

  The internal communication error struct. It contains the number
  of each error (break, framing, overrun and parity) since opening
  the serial port. Each error number will be cleared if the open
  methode was called.
*/
struct wxSerialPort_EINFO
{
    /*! number of breaks */
    int brk;
    /*! number of framing errors */
    int frame;
    /*! number of overrun errors */
    int overrun;
    /*! number of parity errors */
    int parity;
    wxSerialPort_EINFO() {
	brk = frame = overrun = parity = 0;
    };
};

/*!
\enum IOCTL calls for wxSerialPort

The following Ioctl calls are only valid for the wxSerialPort
class.
*/
enum {
    /*!
      Get all numbers of occured communication errors (breaks
      framing, overrun and parity), so the args parameter of
      the Ioctl call must pointed to a wxSerialPort_EINFO
      struct.
    */
    CTB_SER_GETEINFO = CTB_SERIAL,
    /*!
      Get integer 1, if a break occured since the last call
      so the args parameter of the Ioctl methode must pointed
      to an integer value. If there was no break, the result
      is integer 0.
    */
    CTB_SER_GETBRK,
    /*!
      Get integer 1, if a framing occured since the last call
      so the args parameter of the Ioctl methode must pointed
      to an integer value. If there was no break, the result
      is integer 0.
    */
    CTB_SER_GETFRM,
    /*!
      Get integer 1, if a overrun occured since the last call
      so the args parameter of the Ioctl methode must pointed
      to an integer value. If there was no break, the result
      is integer 0.
    */
    CTB_SER_GETOVR,
    /*!
      Get integer 1, if a parity occured since the last call
      so the args parameter of the Ioctl methode must pointed
      to an integer value. If there was no break, the result
      is integer 0.
    */
    CTB_SER_GETPAR,
    /*!
	 Get the number of bytes received by the serial port driver
	 but not yet read by a Read or Readv Operation.
    */
    CTB_SER_GETINQUE
};

/*!
  \class wxSerialPort_x
  wxSerialPort_x is the basic class for serial communication via
  the serial comports. It is also an abstract class and defines
  all necessary methods, which the derivated plattform depended
  classes must be invoke.
*/
class wxSerialPort_x : public wxIOBase
{
protected:
    wxSerialPort_DCS m_dcs;
    char m_devname[WXSERIALPORT_NAME_LEN];
public:
    wxSerialPort_x() {m_devname[0] = '\0';};
    virtual ~wxSerialPort_x() {};

    const char* ClassName() {return "wxSerialPort";};
    
    /*!
      \brief change the linestates according to which bits
      are set/unset in flags.
      \param flags valid line flags are wxSERIAL_LINESTATE_DSR and/or
      wxSERIAL_LINESTATE_RTS
      \return zero on success, -1 if an error occurs
    */
    virtual int ChangeLineState(wxSerialLineState flags) = 0;

    /*! 
      \brief turn off status lines depending upon which bits (DSR and/or RTS)
      are set in flags.
      \param flags valid line flags are wxSERIAL_LINESTATE_DSR and/or
      wxSERIAL_LINESTATE_RTS
      \return zero on success, -1 if an error occurs
    */
    virtual int ClrLineState(wxSerialLineState flags) = 0;

    /*!
      \brief Read the line states of DCD, CTS, DSR and RING
      \param flags the appropiate bits are set in flags according
      to the current status
      \return zero on sucess, otherwise -1
    */
    virtual int GetLineState(wxSerialLineState* flags) = 0;

    /*!
      \brief given the current settings of the connected serial port
      as a string. ctb doesn't require the wxwindow library, it's only
      a supplementation for it. So we don't use the wxString class here.
      \param str covers the settings as a null terminated string
      \param size size of the char buffer, pointed by str.
      \return GetSettingsAsString do not write more than size bytes 
      (including the trailing '\0'), and return -1 if the output was 
      truncated due to this limit. Otherwise the function return the
      number of characters which would have been written to the final
      string.
    */
    virtual int GetSettingsAsString(char* str, size_t size) = 0;

    /*!
      \brief Many operating characteristics are only possible for
      special devices. To avoid the need of a lot of different functions
      and to give the user a uniform interface, all this special
      operating instructions will covered by one Ioctl methode (like
      the linux ioctl call).
      The Ioctl command (cmd) has encoded in it whether the argument 
      is an in parameter or out parameter, and the size of the
      argument args in bytes. Macros and defines used in specifying an
      ioctl request are located in iobase.h and the header file for
      the derivated device (for example serportx.h).
      \param cmd specify the ioctl request.
      \param args is a typeless pointer to a memory location, where
      Ioctl reads the request arguments or write the results.
      Please note, that an invalid memory location or size involving
      a buffer overflow or segmention fault!
    */
    virtual int Ioctl(int cmd,void* args) {return -1;};

    /*!
      \brief Sendbreak transmits a continuous stream of zero-valued
      bits for a specific duration.
      \param duration If duration is zero, it transmits 
      zero-valued bits for at least 0.25 seconds, and  not more that 
      0.5 seconds. If duration is not zero, it sends zero-valued bits
      for  duration*N  seconds, where N is at least 0.25, and not more
      than 0.5.
      \return zero on success, -1 if an error occurs.
    */
    virtual int SendBreak(int duration) = 0;

    /*! \brief Set the baudrate
      \param wxBaud baudrate
      \return zero on success, -1 if an error occurs
    */
    virtual int SetBaudRate(wxBaud baudrate) = 0;

    /*! 
      \brief turn on status lines depending upon which bits (DSR and/or RTS)
      are set in flags.
      \param flags valid line flags are wxSERIAL_LINESTATE_DSR and/or
      wxSERIAL_LINESTATE_RTS
      \return zero on success, -1 if an error occurs
    */
    virtual int SetLineState(wxSerialLineState flags) = 0;
};

/*! 
  \example tdevcua.cpp
  This is a simple example of how to use the wxSerialPort class. You will
  find it in samples/ctb.
  \example tstate.cpp
   very simple example, for checking the modem control state lines.
   You will find it also in samples/ctb. 
 */

#endif
