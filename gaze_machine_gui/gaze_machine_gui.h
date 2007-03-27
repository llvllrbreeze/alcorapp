/////////////////////////////////////////////////////////////////////////////
// Name:        gaze_machine_gui.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     25/03/2007 19:43:29
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAZE_MACHINE_GUI_H_
#define _GAZE_MACHINE_GUI_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "gaze_machine_gui.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "gazemachine_mainframe.h"
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
 * Gaze_machine_guiApp class declaration
 */

class Gaze_machine_guiApp: public wxApp
{    
    DECLARE_CLASS( Gaze_machine_guiApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    Gaze_machine_guiApp();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin Gaze_machine_guiApp event handler declarations

////@end Gaze_machine_guiApp event handler declarations

////@begin Gaze_machine_guiApp member function declarations

////@end Gaze_machine_guiApp member function declarations

////@begin Gaze_machine_guiApp member variables
////@end Gaze_machine_guiApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(Gaze_machine_guiApp)
////@end declare app

#endif
    // _GAZE_MACHINE_GUI_H_
