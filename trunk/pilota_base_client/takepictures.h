/////////////////////////////////////////////////////////////////////////////
// Name:        takepictures.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     22/03/2007 09:58:18
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TAKEPICTURES_H_
#define _TAKEPICTURES_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "takepictures.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "take_pictures_frame.h"
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
 * TakepicturesApp class declaration
 */

class TakepicturesApp: public wxApp
{    
    DECLARE_CLASS( TakepicturesApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    TakepicturesApp();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin TakepicturesApp event handler declarations

////@end TakepicturesApp event handler declarations

////@begin TakepicturesApp member function declarations

////@end TakepicturesApp member function declarations

////@begin TakepicturesApp member variables
////@end TakepicturesApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(TakepicturesApp)
////@end declare app

#endif
    // _TAKEPICTURES_H_
