/////////////////////////////////////////////////////////////////////////////
// Name:        gazemachine_mainframe.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     25/03/2007 19:46:39
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAZEMACHINE_MAINFRAME_H_
#define _GAZEMACHINE_MAINFRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "gazemachine_mainframe.h"
#endif

#include "alcor.apps/gaze/gaze_machine_t.h"
using namespace all;
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
#define SYMBOL_GAZEMACHINE_MAINFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_GAZEMACHINE_MAINFRAME_TITLE _("Gaze Machine")
#define SYMBOL_GAZEMACHINE_MAINFRAME_IDNAME ID_GAZEMACHINE_MAINFRAME
#define SYMBOL_GAZEMACHINE_MAINFRAME_SIZE wxSize(400, 300)
#define SYMBOL_GAZEMACHINE_MAINFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * gazemachine_mainframe class declaration
 */

class gazemachine_mainframe: public wxFrame
{    
    DECLARE_CLASS( gazemachine_mainframe )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    gazemachine_mainframe();
    gazemachine_mainframe( wxWindow* parent, wxWindowID id = SYMBOL_GAZEMACHINE_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_GAZEMACHINE_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_GAZEMACHINE_MAINFRAME_POSITION, const wxSize& size = SYMBOL_GAZEMACHINE_MAINFRAME_SIZE, long style = SYMBOL_GAZEMACHINE_MAINFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_GAZEMACHINE_MAINFRAME_IDNAME, const wxString& caption = SYMBOL_GAZEMACHINE_MAINFRAME_TITLE, const wxPoint& pos = SYMBOL_GAZEMACHINE_MAINFRAME_POSITION, const wxSize& size = SYMBOL_GAZEMACHINE_MAINFRAME_SIZE, long style = SYMBOL_GAZEMACHINE_MAINFRAME_STYLE );

    /// Destructor
    ~gazemachine_mainframe();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin gazemachine_mainframe event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GAZELOG_BUTTON
    void OnGazelogButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STOP_BUTTON
    void OnStopButtonClick( wxCommandEvent& event );

////@end gazemachine_mainframe event handler declarations

////@begin gazemachine_mainframe member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end gazemachine_mainframe member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin gazemachine_mainframe member variables
    wxButton* gazelog_btn;
    wxButton* stoplog_btn;
    wxTextCtrl* m_logname_;
    /// Control identifiers
    enum {
        ID_GAZEMACHINE_MAINFRAME = 10000,
        ID_GAZELOG_BUTTON = 10002,
        ID_STOP_BUTTON = 10003,
        ID_NAME_TEXTCTRL = 10005
    };
////@end gazemachine_mainframe member variables

    all::gaze::gaze_machine_sptr gaze_;
};

#endif
    // _GAZEMACHINE_MAINFRAME_H_
