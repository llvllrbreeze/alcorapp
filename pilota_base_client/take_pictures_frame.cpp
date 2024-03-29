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

	EVT_BUTTON( ID_BUTTON_SET_WANDERING, take_pictures_frame::OnButtonSetWanderingClick )

	EVT_BUTTON( ID_BUTTON_SET_WANDERING_SP, take_pictures_frame::OnButtonSetWanderingSpClick )

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
	msg_label = NULL;
}

/*!
 * Control creation for take_pictures_frame
 */

void take_pictures_frame::CreateControls()
{    
////@begin take_pictures_frame content construction
    // Generated by DialogBlocks, 03/10/2007 15:45:37 (unregistered)

    take_pictures_frame* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_stream_panel = new wx_img_stream_panel_t( "config/bumblebee_stream_client.ini" );
    m_stream_panel->Create( itemFrame1, ID_PANEL_IMAGE, wxDefaultPosition, wxSize(320, 240), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer3->Add(m_stream_panel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer7Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Ptu"));
    wxStaticBoxSizer* itemStaticBoxSizer7 = new wxStaticBoxSizer(itemStaticBoxSizer7Static, wxVERTICAL);
    itemBoxSizer6->Add(itemStaticBoxSizer7, 0, wxALIGN_TOP|wxALL, 5);

    wxButton* itemButton8 = new wxButton;
    itemButton8->Create( itemFrame1, ID_BUTTON_CENTER, _("Center Ptu"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer7->Add(itemButton8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton;
    itemButton9->Create( itemFrame1, ID_BUTTON_RESET, _("Reset Ptu"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer7->Add(itemButton9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemStaticBoxSizer7->Add(5, 15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    speed_control = new wxSlider;
    speed_control->Create( itemFrame1, ID_SLIDER_VEL, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer7->Add(speed_control, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer12Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Robot"));
    wxStaticBoxSizer* itemStaticBoxSizer12 = new wxStaticBoxSizer(itemStaticBoxSizer12Static, wxVERTICAL);
    itemBoxSizer6->Add(itemStaticBoxSizer12, 0, wxALIGN_TOP|wxALL, 5);

    wxButton* itemButton13 = new wxButton;
    itemButton13->Create( itemFrame1, ID_BUTTON_SET_WANDERING, _("Wandering"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer12->Add(itemButton13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton14 = new wxButton;
    itemButton14->Create( itemFrame1, ID_BUTTON_SET_WANDERING_SP, _("Wandering SP"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer12->Add(itemButton14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton15 = new wxButton;
    itemButton15->Create( itemFrame1, ID_BUTTON_STOP_ROBOT, _("Stop P3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer12->Add(itemButton15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer16Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Msg Area"));
    wxStaticBoxSizer* itemStaticBoxSizer16 = new wxStaticBoxSizer(itemStaticBoxSizer16Static, wxHORIZONTAL);
    itemBoxSizer5->Add(itemStaticBoxSizer16, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton18 = new wxButton;
    itemButton18->Create( itemFrame1, ID_BUTTON_TAKE_PICTURE, _("Take Picture"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton18, 0, wxGROW|wxALL, 5);

    // Connect events and objects
    m_stream_panel->Connect(ID_PANEL_IMAGE, wxEVT_LEFT_DOWN, wxMouseEventHandler(take_pictures_frame::OnLeftDown), NULL, this);
////@end take_pictures_frame content construction

	msg_label = new wxStaticText;
    msg_label->Create( itemFrame1, wxID_MSG_LABEL, _("Messaggio"), wxDefaultPosition, wxSize(200, 40), 0 );
    itemStaticBoxSizer16->Add(msg_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

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
    pilota.move_ptu_to_XY(event.GetX(),event.GetY());
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
	msg_label->SetLabel(_T("PTU set to (0,0)."));
  pilota.set_pantilt(0,0);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESET
 */

void take_pictures_frame::OnButtonResetClick( wxCommandEvent& event )
{
	msg_label->SetLabel(_T("PTU reset."));
  pilota.reset();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_STOP_ROBOT
 */

void take_pictures_frame::OnButtonStopRobotClick( wxCommandEvent& event )
{
	msg_label->SetLabel(_T("The robot is stopped."));
  pilota.stop();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_WANDERING
 */

void take_pictures_frame::OnButtonSetWanderingClick( wxCommandEvent& event )
{	
	msg_label->SetLabel(_T("Wandering..."));
	pilota.enable_wander();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SET_WANDERING_SP
 */

void take_pictures_frame::OnButtonSetWanderingSpClick( wxCommandEvent& event )
{	
	msg_label->SetLabel(_T("Wandering Special..."));
	pilota.enable_wander_special();
}

/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER1
 */

void take_pictures_frame::OnSliderVelUpdated( wxCommandEvent& event )
{
  int vel = speed_control->GetValue();
  //std::string sliderv = "slider: ";
  //sliderv += boost::lexical_cast<std::string>(vel);

  //wxMessageBox(_T(sliderv));

  double velsetpoint = (vel/100.0) * 300;
  pilota.set_vel(velsetpoint);
}
