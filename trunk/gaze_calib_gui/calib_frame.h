/////////////////////////////////////////////////////////////////////////////
// Name:        calib_frame.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     02/04/2007 02:22:23
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CALIB_FRAME_H_
#define _CALIB_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "calib_frame.h"
#endif

#include "alcor.apps/gaze/gaze_machine_VI_t.h"
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
#define SYMBOL_CALIB_FRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CALIB_FRAME_TITLE _("calib_frame")
#define SYMBOL_CALIB_FRAME_IDNAME ID_CALIB_FRAME
#define SYMBOL_CALIB_FRAME_SIZE wxSize(400, 300)
#define SYMBOL_CALIB_FRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * calib_frame class declaration
 */

class calib_frame: public wxFrame
{    
    DECLARE_CLASS( calib_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    calib_frame();
    calib_frame( wxWindow* parent, wxWindowID id = SYMBOL_CALIB_FRAME_IDNAME, const wxString& caption = SYMBOL_CALIB_FRAME_TITLE, const wxPoint& pos = SYMBOL_CALIB_FRAME_POSITION, const wxSize& size = SYMBOL_CALIB_FRAME_SIZE, long style = SYMBOL_CALIB_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CALIB_FRAME_IDNAME, const wxString& caption = SYMBOL_CALIB_FRAME_TITLE, const wxPoint& pos = SYMBOL_CALIB_FRAME_POSITION, const wxSize& size = SYMBOL_CALIB_FRAME_SIZE, long style = SYMBOL_CALIB_FRAME_STYLE );

    /// Destructor
    ~calib_frame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin calib_frame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_CALIB_FRAME
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DO_CALIB_BUTTON
    void OnDoCalibButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STOP_BUTTON
    void OnStopButtonClick( wxCommandEvent& event );

////@end calib_frame event handler declarations

////@begin calib_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end calib_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin calib_frame member variables
    wxButton* go_calib_btn;
    /// Control identifiers
    enum {
        ID_CALIB_FRAME = 10000,
        ID_DO_CALIB_BUTTON = 10001,
        ID_STOP_BUTTON = 10002
    };
////@end calib_frame member variables

    all::gaze::gaze_machine_sptr gaze_;
};

#endif
    // _CALIB_FRAME_H_
