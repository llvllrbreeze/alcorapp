/////////////////////////////////////////////////////////////////////////////
// Name:        wx_test_frame.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     16/03/2007 15:14:44
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TEST_FRAME_H_
#define _WX_TEST_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "wx_test_frame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wx_stream_panel_t;
////@end forward declarations
#include "wx_stream_panel_t.hpp"

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WX_TEST_FRAME 10000
#define SYMBOL_WX_TEST_FRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WX_TEST_FRAME_TITLE _("wx_test_frame")
#define SYMBOL_WX_TEST_FRAME_IDNAME ID_WX_TEST_FRAME
#define SYMBOL_WX_TEST_FRAME_SIZE wxSize(399, 299)
#define SYMBOL_WX_TEST_FRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wx_test_frame class declaration
 */

class wx_test_frame: public wxFrame
{    
    DECLARE_CLASS( wx_test_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wx_test_frame();
    wx_test_frame( wxWindow* parent, wxWindowID id = SYMBOL_WX_TEST_FRAME_IDNAME, const wxString& caption = SYMBOL_WX_TEST_FRAME_TITLE, const wxPoint& pos = SYMBOL_WX_TEST_FRAME_POSITION, const wxSize& size = SYMBOL_WX_TEST_FRAME_SIZE, long style = SYMBOL_WX_TEST_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WX_TEST_FRAME_IDNAME, const wxString& caption = SYMBOL_WX_TEST_FRAME_TITLE, const wxPoint& pos = SYMBOL_WX_TEST_FRAME_POSITION, const wxSize& size = SYMBOL_WX_TEST_FRAME_SIZE, long style = SYMBOL_WX_TEST_FRAME_STYLE );

    /// Destructor
    ~wx_test_frame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wx_test_frame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_WX_TEST_FRAME
    void OnCloseWindow( wxCloseEvent& event );

////@end wx_test_frame event handler declarations

////@begin wx_test_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wx_test_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wx_test_frame member variables
////@end wx_test_frame member variables

	wx_stream_panel_t* m_stream_panel;
};

#endif
    // _WX_TEST_FRAME_H_
