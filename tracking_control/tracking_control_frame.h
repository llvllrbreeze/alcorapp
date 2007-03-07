/////////////////////////////////////////////////////////////////////////////
// Name:        tracking_control_frame.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     05/03/2007 23:16:16
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _TRACKING_CONTROL_FRAME_H_
#define _TRACKING_CONTROL_FRAME_H_


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "tracking_control_frame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
////@end includes

#include "alcor.apps/trm/task_dispatcher.h"


/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_TRACKING_CONTROL_FRAME 10000
#define ID_BUTTON_SETUP 10001
#define ID_BUTTON_IDLE 10003
#define ID_BUTTON_RESET 10002
#define ID_BUTTON_TRACK 10004
#define SYMBOL_TRACKING_CONTROL_FRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACKING_CONTROL_FRAME_TITLE _("Tracking Control")
#define SYMBOL_TRACKING_CONTROL_FRAME_IDNAME ID_TRACKING_CONTROL_FRAME
#define SYMBOL_TRACKING_CONTROL_FRAME_SIZE wxSize(400, 300)
#define SYMBOL_TRACKING_CONTROL_FRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * tracking_control_frame class declaration
 */

class tracking_control_frame: public wxFrame
{    
    DECLARE_CLASS( tracking_control_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    tracking_control_frame( );
    tracking_control_frame( wxWindow* parent, wxWindowID id = SYMBOL_TRACKING_CONTROL_FRAME_IDNAME, const wxString& caption = SYMBOL_TRACKING_CONTROL_FRAME_TITLE, const wxPoint& pos = SYMBOL_TRACKING_CONTROL_FRAME_POSITION, const wxSize& size = SYMBOL_TRACKING_CONTROL_FRAME_SIZE, long style = SYMBOL_TRACKING_CONTROL_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACKING_CONTROL_FRAME_IDNAME, const wxString& caption = SYMBOL_TRACKING_CONTROL_FRAME_TITLE, const wxPoint& pos = SYMBOL_TRACKING_CONTROL_FRAME_POSITION, const wxSize& size = SYMBOL_TRACKING_CONTROL_FRAME_SIZE, long style = SYMBOL_TRACKING_CONTROL_FRAME_STYLE );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin tracking_control_frame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_TRACKING_CONTROL_FRAME
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SETUP
    void OnButtonSetupClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_IDLE
    void OnButtonIdleClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESET
    void OnButtonResetClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TRACK
    void OnButtonTrackClick( wxCommandEvent& event );

////@end tracking_control_frame event handler declarations

////@begin tracking_control_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end tracking_control_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin tracking_control_frame member variables
////@end tracking_control_frame member variables

    all::trm::task_dispatcher_sptr dispatcher;
};

#endif
    // _TRACKING_CONTROL_FRAME_H_
