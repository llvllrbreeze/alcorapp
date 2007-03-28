/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_machine.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:33:06
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _EXPLORATION_MACHINE_H_
#define _EXPLORATION_MACHINE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "exploration_machine.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "exploration_machine_gui.h"
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
 * Exploration_machineApp class declaration
 */

class Exploration_machineApp: public wxApp
{    
    DECLARE_CLASS( Exploration_machineApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    Exploration_machineApp();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin Exploration_machineApp event handler declarations

////@end Exploration_machineApp event handler declarations

////@begin Exploration_machineApp member function declarations

////@end Exploration_machineApp member function declarations

////@begin Exploration_machineApp member variables
////@end Exploration_machineApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(Exploration_machineApp)
////@end declare app

#endif
    // _EXPLORATION_MACHINE_H_
