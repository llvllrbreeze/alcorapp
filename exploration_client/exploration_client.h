/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_client.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:48:46
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _EXPLORATION_CLIENT_H_
#define _EXPLORATION_CLIENT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "exploration_client.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "exploration_gui.h"
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
 * Exploration_clientApp class declaration
 */

class Exploration_clientApp: public wxApp
{    
    DECLARE_CLASS( Exploration_clientApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    Exploration_clientApp();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin Exploration_clientApp event handler declarations

////@end Exploration_clientApp event handler declarations

////@begin Exploration_clientApp member function declarations

////@end Exploration_clientApp member function declarations

////@begin Exploration_clientApp member variables
////@end Exploration_clientApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(Exploration_clientApp)
////@end declare app

#endif
    // _EXPLORATION_CLIENT_H_
