/////////////////////////////////////////////////////////////////////////////
// Name:        exploring_control_mainframe.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     27/03/2007 03:31:41
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _EXPLORING_CONTROL_MAINFRAME_H_
#define _EXPLORING_CONTROL_MAINFRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "exploring_control_mainframe.h"
#endif

#include "alcor.apps/xpr/task_dispatcher_t.hpp"
#include "alcor/gui/wx_img_stream_panel_t.hpp"
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
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_EXPLORING_CONTROL_MAINFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_EXPLORING_CONTROL_MAINFRAME_TITLE _("Dialog")
#define SYMBOL_EXPLORING_CONTROL_MAINFRAME_IDNAME ID_EXPLORING_CONTROL_MAINFRAME
#define SYMBOL_EXPLORING_CONTROL_MAINFRAME_SIZE wxSize(400, 300)
#define SYMBOL_EXPLORING_CONTROL_MAINFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * exploring_control_mainframe class declaration
 */

class exploring_control_mainframe: public wxFrame
{    
    DECLARE_CLASS( exploring_control_mainframe )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    exploring_control_mainframe();
    exploring_control_mainframe( wxWindow* parent, wxWindowID id = SYMBOL_EXPLORING_CONTROL_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_EXPLORING_CONTROL_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_EXPLORING_CONTROL_MAINFRAME_POSITION, const wxSize& size = SYMBOL_EXPLORING_CONTROL_MAINFRAME_SIZE, long style = SYMBOL_EXPLORING_CONTROL_MAINFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EXPLORING_CONTROL_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_EXPLORING_CONTROL_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_EXPLORING_CONTROL_MAINFRAME_POSITION, const wxSize& size = SYMBOL_EXPLORING_CONTROL_MAINFRAME_SIZE, long style = SYMBOL_EXPLORING_CONTROL_MAINFRAME_STYLE );

    /// Destructor
    ~exploring_control_mainframe();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin exploring_control_mainframe event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_IDLE_BUTTON
    void OnIdleButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESET_BUTTON
    void OnResetButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESUME_BUTTON
    void OnResumeButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPLORE_BUTTON
    void OnExploreButtonClick( wxCommandEvent& event );

////@end exploring_control_mainframe event handler declarations

////@begin exploring_control_mainframe member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end exploring_control_mainframe member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin exploring_control_mainframe member variables
    /// Control identifiers
    enum {
        ID_EXPLORING_CONTROL_MAINFRAME = 10000,
        ID_IDLE_BUTTON = 10001,
        ID_RESET_BUTTON = 10002,
        ID_RESUME_BUTTON = 10003,
        ID_EXPLORE_BUTTON = 10004
    };
////@end exploring_control_mainframe member variables

    boost::shared_ptr<all::xpr::task_dispatcher_t>  dispatcher_;
};

#endif
    // _EXPLORING_CONTROL_MAINFRAME_H_
