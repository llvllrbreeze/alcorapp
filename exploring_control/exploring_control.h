/////////////////////////////////////////////////////////////////////////////
// Name:        exploring_control.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     27/03/2007 03:31:00
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _EXPLORING_CONTROL_H_
#define _EXPLORING_CONTROL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "exploring_control.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "exploring_control_mainframe.h"
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
 * Exploring_controlApp class declaration
 */

class Exploring_controlApp: public wxApp
{    
    DECLARE_CLASS( Exploring_controlApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    Exploring_controlApp();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin Exploring_controlApp event handler declarations

////@end Exploring_controlApp event handler declarations

////@begin Exploring_controlApp member function declarations

////@end Exploring_controlApp member function declarations

////@begin Exploring_controlApp member variables
////@end Exploring_controlApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(Exploring_controlApp)
////@end declare app

#endif
    // _EXPLORING_CONTROL_H_
