/////////////////////////////////////////////////////////////////////////////
// Name:        take_pictures_frame.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     22/03/2007 09:59:17
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "take_pictures_frame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "take_pictures_frame.h"

////@begin XPM images
////@end XPM images

/*!
 * take_pictures_frame type definition
 */

IMPLEMENT_CLASS( take_pictures_frame, wxFrame )

/*!
 * take_pictures_frame event table definition
 */

BEGIN_EVENT_TABLE( take_pictures_frame, wxFrame )

////@begin take_pictures_frame event table entries
    EVT_BUTTON( ID_BUTTON_CENTER, take_pictures_frame::OnButtonCenterClick )

    EVT_BUTTON( ID_BUTTON_RESET, take_pictures_frame::OnButtonResetClick )

    EVT_SLIDER( ID_SLIDER_VEL, take_pictures_frame::OnSliderVelUpdated )

    EVT_BUTTON( ID_BUTTON_STOP_ROBOT, take_pictures_frame::OnButtonStopRobotClick )

    EVT_BUTTON( ID_BUTTON_TAKE_PICTURE, take_pictures_frame::OnButtonTakePictureClick )

////@end take_pictures_frame event table entries

END_EVENT_TABLE()

/*!
 * take_pictures_frame constructors
 */

take_pictures_frame::take_pictures_frame()
{
    Init();
}

take_pictures_frame::take_pictures_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * take_pictures_frame creator
 */

bool take_pictures_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin take_pictures_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end take_pictures_frame creation
    return true;
}

/*!
 * take_pictures_frame destructor
 */

take_pictures_frame::~take_pictures_frame()
{
////@begin take_pictures_frame destruction
////@end take_pictures_frame destruction
}

/*!
 * Member initialisation 
 */

void take_pictures_frame::Init()
{
////@begin take_pictures_frame member initialisation
    m_stream_panel = NULL;
    speed_control = NULL;
////@end take_pictures_frame member initialisation
}

/*!
 * Control creation for take_pictures_frame
 */

void take_pictures_frame::CreateControls()
{    
////@begin take_pictures_frame content construction
    take_pictures_frame* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_stream_panel = new wx_img_stream_panel_t( "bumbebee_stream_client.ini" );
    m_stream_panel->Create( itemFrame1, ID_PANEL_IMAGE, wxDefaultPosition, wxSize(320, 240), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer3->Add(m_stream_panel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer5Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Static"));
    wxStaticBoxSizer* itemStaticBoxSizer5 = new wxStaticBoxSizer(itemStaticBoxSizer5Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer5, 0, wxALIGN_TOP|wxALL, 5);

    wxButton* itemButton6 = new wxButton;
    itemButton6->Create( itemFrame1, ID_BUTTON_CENTER, _("Center Ptu"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton;
    itemButton7->Create( itemFrame1, ID_BUTTON_RESET, _("Reset Ptu"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemStaticBoxSizer5->Add(5, 15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    speed_control = new wxSlider;
    speed_control->Create( itemFrame1, ID_SLIDER_VEL, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer5->Add(speed_control, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton;
    itemButton10->Create( itemFrame1, ID_BUTTON_STOP_ROBOT, _("Stop P3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer5->Add(itemButton10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton11 = new wxButton;
    itemButton11->Create( itemFrame1, ID_BUTTON_TAKE_PICTURE, _("Take Picture"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton11, 0, wxGROW|wxALL, 5);

    // Connect events and objects
    m_stream_panel->Connect(ID_PANEL_IMAGE, wxEVT_LEFT_DOWN, wxMouseEventHandler(take_pictures_frame::OnLeftDown), NULL, this);
////@end take_pictures_frame content construction

}

/*!
 * Should we show tooltips?
 */

bool take_pictures_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap take_pictures_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin take_pictures_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end take_pictures_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon take_pictures_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin take_pictures_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end take_pictures_frame icon retrieval
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TAKE_PICTURE
 */

void take_pictures_frame::OnButtonTakePictureClick( wxCommandEvent& event )
{
pilota.trigger_operation();
}

/*!
 * wxEVT_LEFT_DOWN event handler for ID_PANEL1
 */

void take_pictures_frame::OnLeftDown( wxMouseEvent& event )
{
  double  xoffset  = (event.GetX()- 160)/160.0;
  double  yoffset  = (event.GetY()- 120)/120.0;

  if (event.ControlDown())
  {
    float pan  = -xoffset * 110;
    float tilt = -yoffset * 30;

    pilota.set_pantilt(pan, tilt);
  }
  else
  {
    double dhead = -xoffset * 25;
    pilota.set_dheading(dhead);
  }
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON2
 */

void take_pictures_frame::OnButtonCenterClick( wxCommandEvent& event )
{
  pilota.set_pantilt(0,0);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESET
 */

void take_pictures_frame::OnButtonResetClick( wxCommandEvent& event )
{
  pilota.reset();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_STOP_ROBOT
 */

void take_pictures_frame::OnButtonStopRobotClick( wxCommandEvent& event )
{
  pilota.stop();
}

/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER1
 */

void take_pictures_frame::OnSliderVelUpdated( wxCommandEvent& event )
{
  int vel = speed_control->GetValue();
  double velsetpoint = vel/100 * 300;
  pilota.set_vel(velsetpoint);
}

