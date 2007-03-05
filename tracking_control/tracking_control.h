/////////////////////////////////////////////////////////////////////////////
// Name:        tracking_control.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     05/03/2007 23:15:00
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TRACKING_CONTROL_H_
#define _TRACKING_CONTROL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "tracking_control.h"
#endif

/*!
 * Includes
 */

//#define WIN_LEAN_AND_MEAN

////@begin includes
#include "wx/image.h"
#include "tracking_control_frame.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * tracking_control_app class declaration
 */

class tracking_control_app: public wxApp
{    
    DECLARE_CLASS( tracking_control_app )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    tracking_control_app();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin tracking_control_app event handler declarations

////@end tracking_control_app event handler declarations

////@begin tracking_control_app member function declarations

////@end tracking_control_app member function declarations

////@begin tracking_control_app member variables
////@end tracking_control_app member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(tracking_control_app)
////@end declare app

#endif
    // _TRACKING_CONTROL_H_
