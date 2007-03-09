/////////////////////////////////////////////////////////////////////////////
// Name:        bumblebee_gui.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     04/03/2007 04:18:17
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _BUMBLEBEE_GUI_H_
#define _BUMBLEBEE_GUI_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "bumblebee_gui.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "bumblebee_frame.h"
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
 * bumblebee_gu_iApp class declaration
 */

class bumblebee_gu_iApp: public wxApp
{    
    DECLARE_CLASS( bumblebee_gu_iApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    bumblebee_gu_iApp();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin bumblebee_gu_iApp event handler declarations

////@end bumblebee_gu_iApp event handler declarations

////@begin bumblebee_gu_iApp member function declarations

////@end bumblebee_gu_iApp member function declarations

////@begin bumblebee_gu_iApp member variables
////@end bumblebee_gu_iApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(bumblebee_gu_iApp)
////@end declare app

#endif
    // _BUMBLEBEE_GUI_H_
