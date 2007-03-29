/////////////////////////////////////////////////////////////////////////////
// Name:        shrimp_frame.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     21/03/2007 14:44:35
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SHRIMP_FRAME_H_
#define _SHRIMP_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "shrimp_frame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/statusbr.h"
#include "wx/notebook.h"
#include "wx/tglbtn.h"
////@end includes

#include <alcor/gui/wx_img_stream_panel_t.hpp>
#include <alcor/gui/wx_gl_stream_client_t.hpp>

#include <alcor/act/pololu_ctrl_client_t.hpp>
#include <alcor/act/shrimp_client_t.hpp>

#include "shrimp_joystick.h"
/*!
 * Forward declarations
 */

////@begin forward declarations
class wxStatusBar;
class wxNotebook;
class wx_img_stream_panel_t;
class wxToggleButton;
class wx_gl_stream_client_t;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_SHRIMP_FRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SHRIMP_FRAME_TITLE _("shrimp_frame")
#define SYMBOL_SHRIMP_FRAME_IDNAME ID_SHRIMP_FRAME
#define SYMBOL_SHRIMP_FRAME_SIZE wxSize(800, 490)
#define SYMBOL_SHRIMP_FRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * shrimp_frame class declaration
 */

class shrimp_frame: public wxFrame
{    
    DECLARE_CLASS( shrimp_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    shrimp_frame();
    shrimp_frame( wxWindow* parent, wxWindowID id = SYMBOL_SHRIMP_FRAME_IDNAME, const wxString& caption = SYMBOL_SHRIMP_FRAME_TITLE, const wxPoint& pos = SYMBOL_SHRIMP_FRAME_POSITION, const wxSize& size = SYMBOL_SHRIMP_FRAME_SIZE, long style = SYMBOL_SHRIMP_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SHRIMP_FRAME_IDNAME, const wxString& caption = SYMBOL_SHRIMP_FRAME_TITLE, const wxPoint& pos = SYMBOL_SHRIMP_FRAME_POSITION, const wxSize& size = SYMBOL_SHRIMP_FRAME_SIZE, long style = SYMBOL_SHRIMP_FRAME_STYLE );

    /// Destructor
    ~shrimp_frame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin shrimp_frame event handler declarations

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ON_BUTTON
    void OnOnButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_EM_BUTTON
    void OnEmButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCAN_BUTTON
    void OnScanButtonClick( wxCommandEvent& event );

////@end shrimp_frame event handler declarations

////@begin shrimp_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end shrimp_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin shrimp_frame member variables
    wxStatusBar* m_status_bar;
    wxNotebook* m_shrimp_tab;
    wx_img_stream_panel_t* m_robot_view_panel;
    wxGauge* m_speed_gauge;
    wxStaticText* m_speed_text;
    wx_img_stream_panel_t* m_panoramic_view_panel;
    wxToggleButton* m_on_button;
    wxToggleButton* m_em_button;
    wxGauge* m_steer_gauge;
    wxStaticText* m_steer_text;
    wxStaticText* m_voltage_text;
    wx_gl_stream_client_t* m_gl_stream;
    /// Control identifiers
    enum {
        ID_SHRIMP_FRAME = 10000,
        ID_STATUSBAR = 10001,
        ID_SHRIMP_TAB = 10002,
        ID_TELEOPERATION_PANEL = 10003,
        ID_ROBOT_VIEW_PANEL = 10005,
        ID_SPEED_GAUGE = 10009,
        ID_PANORAMIC_VIEW_PANEL = 10007,
        ID_ON_BUTTON = 10011,
        ID_EM_BUTTON = 10012,
        ID_STEER_GAUGE = 10006,
        ID_3D_SCAN_PANEL = 10004,
        ID_GL_STREAM = 10008,
        ID_SCAN_BUTTON = 10010
    };
////@end shrimp_frame member variables
	enum {
		ID_TIMER = 20000
	};

public:
	void on_timer(wxTimerEvent&);
	void on_joy_move(wxJoystickEvent&);
	void on_joy_click(wxJoystickEvent&);

private:

	shrimp_joystick m_joy;
	wxTimer* m_timer;

	all::act::shrimp_client_t* m_shrimp;
	all::act::pololu_ctrl_client_t* m_pololu;
};

#endif
    // _SHRIMP_FRAME_H_
