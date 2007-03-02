/////////////////////////////////////////////////////////////////////////////
// Name:        shrimp_main_frame.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     22/02/2007 00:33:53
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SHRIMP_MAIN_FRAME_H_
#define _SHRIMP_MAIN_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "shrimp_main_frame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/statusbr.h"
#include "wx/tglbtn.h"
////@end includes

#include <alcor/act/shrimp_client_t.hpp>

#include <alcor/core/stream_client_t.hpp>
#include <alcor/act/pololu_ctrl_client_t.hpp>

#include <alcor/core/image_utils.h>

#include "shrimp_joystick.h"
#include <alcor/core/wxpanel_stream_dest_t.hpp>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxStatusBar;
class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SHRIMP_MAIN_FRAME 10000
#define ID_STATUSBAR 10001
#define ID_CAM_PANEL 10002
#define ID_PLAY 10005
#define ID_STOP 10006
#define ID_ON_BUTTON 10003
#define ID_EM_BUTTON 10004
#define SYMBOL_SHRIMP_MAIN_FRAME_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxSTATIC_BORDER
#define SYMBOL_SHRIMP_MAIN_FRAME_TITLE _("shrimp")
#define SYMBOL_SHRIMP_MAIN_FRAME_IDNAME ID_SHRIMP_MAIN_FRAME
#define SYMBOL_SHRIMP_MAIN_FRAME_SIZE wxSize(500, 450)
#define SYMBOL_SHRIMP_MAIN_FRAME_POSITION wxDefaultPosition
////@end control identifiers

#define ID_TIMER 10005

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * shrimp_main_frame class declaration
 */

class shrimp_main_frame: public wxFrame
{    
    DECLARE_CLASS( shrimp_main_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    shrimp_main_frame();
    shrimp_main_frame( wxWindow* parent, wxWindowID id = SYMBOL_SHRIMP_MAIN_FRAME_IDNAME, const wxString& caption = SYMBOL_SHRIMP_MAIN_FRAME_TITLE, const wxPoint& pos = SYMBOL_SHRIMP_MAIN_FRAME_POSITION, const wxSize& size = SYMBOL_SHRIMP_MAIN_FRAME_SIZE, long style = SYMBOL_SHRIMP_MAIN_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SHRIMP_MAIN_FRAME_IDNAME, const wxString& caption = SYMBOL_SHRIMP_MAIN_FRAME_TITLE, const wxPoint& pos = SYMBOL_SHRIMP_MAIN_FRAME_POSITION, const wxSize& size = SYMBOL_SHRIMP_MAIN_FRAME_SIZE, long style = SYMBOL_SHRIMP_MAIN_FRAME_STYLE );

    /// Destructor
    ~shrimp_main_frame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin shrimp_main_frame event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PLAY
    void OnPlayClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STOP
    void OnStopClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ON_BUTTON
    void OnOnButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_EM_BUTTON
    void OnEmButtonClick( wxCommandEvent& event );

////@end shrimp_main_frame event handler declarations

////@begin shrimp_main_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end shrimp_main_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();


////@begin shrimp_main_frame member variables
    wxStatusBar* m_status_bar;
    //wxPanel* m_cam_panel;
    wxStaticText* m_speed_txt;
    wxStaticText* m_steer_txt;
    wxStaticText* m_voltage_txt;
    wxToggleButton* m_robot_on_off_button;
    wxToggleButton* m_em_on_off_button;
////@end shrimp_main_frame member variables

	
	all::core::wxpanel_stream_dest_t* m_cam_panel;


public:

	void on_timer(wxTimerEvent&);
	void joy_handler(wxJoystickEvent&);

private:
	all::act::shrimp_client_t* m_shrimp;
	shrimp_joystick m_joy;
	
	wxTimer* m_timer;

	all::core::stream_client_t* m_stream_client;

	all::act::pololu_ctrl_client_t* m_pololu;

	
};

#endif
    // _SHRIMP_MAIN_FRAME_H_
