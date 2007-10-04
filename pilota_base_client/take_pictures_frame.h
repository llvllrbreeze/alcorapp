/////////////////////////////////////////////////////////////////////////////
// Name:        take_pictures_frame.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     22/03/2007 09:59:17
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////


#ifndef _TAKE_PICTURES_FRAME_H_
#define _TAKE_PICTURES_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "take_pictures_frame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
////@end includes
//-----------------------------------------------------------------------++
#include "pilota_base_client.hpp"
#include "alcor/gui/wx_img_stream_panel_t.hpp"
/*!
 * Forward declarations
 */

////@begin forward declarations
class wx_img_stream_panel_t;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_TAKE_PICTURES_FRAME 10000
#define ID_PANEL_IMAGE 10001
#define ID_BUTTON_CENTER 10003
#define ID_BUTTON_RESET 10004
#define ID_SLIDER_VEL 10005
#define ID_BUTTON_SET_WANDERING 10008
#define ID_BUTTON_SET_WANDERING_SP 10009
#define ID_BUTTON_STOP_ROBOT 10006
#define wxID_MSG_LABEL 10007
#define ID_BUTTON_TAKE_PICTURE 10002
#define SYMBOL_TAKE_PICTURES_FRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_TAKE_PICTURES_FRAME_TITLE _("Do")
#define SYMBOL_TAKE_PICTURES_FRAME_IDNAME ID_TAKE_PICTURES_FRAME
#define SYMBOL_TAKE_PICTURES_FRAME_SIZE wxSize(400, 300)
#define SYMBOL_TAKE_PICTURES_FRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * take_pictures_frame class declaration
 */

class take_pictures_frame: public wxFrame
{    
    DECLARE_CLASS( take_pictures_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    take_pictures_frame();
    take_pictures_frame( wxWindow* parent, wxWindowID id = SYMBOL_TAKE_PICTURES_FRAME_IDNAME, const wxString& caption = SYMBOL_TAKE_PICTURES_FRAME_TITLE, const wxPoint& pos = SYMBOL_TAKE_PICTURES_FRAME_POSITION, const wxSize& size = SYMBOL_TAKE_PICTURES_FRAME_SIZE, long style = SYMBOL_TAKE_PICTURES_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TAKE_PICTURES_FRAME_IDNAME, const wxString& caption = SYMBOL_TAKE_PICTURES_FRAME_TITLE, const wxPoint& pos = SYMBOL_TAKE_PICTURES_FRAME_POSITION, const wxSize& size = SYMBOL_TAKE_PICTURES_FRAME_SIZE, long style = SYMBOL_TAKE_PICTURES_FRAME_STYLE );

    /// Destructor
    ~take_pictures_frame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin take_pictures_frame event handler declarations

    /// wxEVT_LEFT_DOWN event handler for ID_PANEL_IMAGE
    void OnLeftDown( wxMouseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CENTER
    void OnButtonCenterClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESET
    void OnButtonResetClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER_VEL
    void OnSliderVelUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_STOP_ROBOT
    void OnButtonStopRobotClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TAKE_PICTURE
    void OnButtonTakePictureClick( wxCommandEvent& event );

////@end take_pictures_frame event handler declarations

/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_WANDERING
    void OnButtonSetWanderingClick( wxCommandEvent& event );

/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_WANDERING_SP
    void OnButtonSetWanderingSpClick( wxCommandEvent& event );

////@begin take_pictures_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end take_pictures_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin take_pictures_frame member variables
    wx_img_stream_panel_t* m_stream_panel;
    wxSlider* speed_control;
////@end take_pictures_frame member variables
    pilota_base_client pilota;

	wxStaticText* msg_label;
};

#endif
    // _TAKE_PICTURES_FRAME_H_
