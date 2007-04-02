/////////////////////////////////////////////////////////////////////////////
// Name:        gaze_calib_gui.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     02/04/2007 02:18:48
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAZE_CALIB_GUI_H_
#define _GAZE_CALIB_GUI_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "gaze_calib_gui.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
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
 * Gaze_calib_gui_App class declaration
 */

class Gaze_calib_gui_App: public wxApp
{    
    DECLARE_CLASS( Gaze_calib_gui_App )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    Gaze_calib_gui_App();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin Gaze_calib_gui_App event handler declarations
////@end Gaze_calib_gui_App event handler declarations

////@begin Gaze_calib_gui_App member function declarations
////@end Gaze_calib_gui_App member function declarations

////@begin Gaze_calib_gui_App member variables
////@end Gaze_calib_gui_App member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(Gaze_calib_gui_App)
////@end declare app

#endif
    // _GAZE_CALIB_GUI_H_
