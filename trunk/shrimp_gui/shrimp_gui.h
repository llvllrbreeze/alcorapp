/////////////////////////////////////////////////////////////////////////////
// Name:        shrimp_gui.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     22/03/2007 14:53:11
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SHRIMP_GUI_H_
#define _SHRIMP_GUI_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "shrimp_gui.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "shrimp_frame.h"
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
 * shrimp_gui_app class declaration
 */

class shrimp_gui_app: public wxApp
{    
    DECLARE_CLASS( shrimp_gui_app )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    shrimp_gui_app();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin shrimp_gui_app event handler declarations
////@end shrimp_gui_app event handler declarations

////@begin shrimp_gui_app member function declarations
////@end shrimp_gui_app member function declarations

////@begin shrimp_gui_app member variables
////@end shrimp_gui_app member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(shrimp_gui_app)
////@end declare app

#endif
    // _SHRIMP_GUI_H_
