/////////////////////////////////////////////////////////////////////////////
// Name:        wx_stream_panel_test.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     16/03/2007 15:10:32
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STREAM_PANEL_TEST_H_
#define _WX_STREAM_PANEL_TEST_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "wx_stream_panel_test.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "wx_test_frame.h"
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
 * wx_stream_panel_test_app class declaration
 */

class wx_stream_panel_test_app: public wxApp
{    
    DECLARE_CLASS( wx_stream_panel_test_app )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wx_stream_panel_test_app();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin wx_stream_panel_test_app event handler declarations

////@end wx_stream_panel_test_app event handler declarations

////@begin wx_stream_panel_test_app member function declarations

////@end wx_stream_panel_test_app member function declarations

////@begin wx_stream_panel_test_app member variables
////@end wx_stream_panel_test_app member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(wx_stream_panel_test_app)
////@end declare app

#endif
    // _WX_STREAM_PANEL_TEST_H_
