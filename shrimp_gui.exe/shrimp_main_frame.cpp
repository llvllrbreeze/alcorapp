/////////////////////////////////////////////////////////////////////////////
// Name:        shrimp_main_frame.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     22/02/2007 00:33:53
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "shrimp_main_frame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/rawbmp.h>
#include <wx/image.h>
#include <wx/dcbuffer.h>

#include <bitset>

////@begin includes
////@end includes

#include "shrimp_main_frame.h"

////@begin XPM images
////@end XPM images

/*!
 * shrimp_main_frame type definition
 */

IMPLEMENT_CLASS( shrimp_main_frame, wxFrame )

/*!
 * shrimp_main_frame event table definition
 */

BEGIN_EVENT_TABLE( shrimp_main_frame, wxFrame )

////@begin shrimp_main_frame event table entries
    EVT_BUTTON( ID_PLAY, shrimp_main_frame::OnPlayClick )

    EVT_BUTTON( ID_STOP, shrimp_main_frame::OnStopClick )

    EVT_TOGGLEBUTTON( ID_ON_BUTTON, shrimp_main_frame::OnOnButtonClick )

    EVT_TOGGLEBUTTON( ID_EM_BUTTON, shrimp_main_frame::OnEmButtonClick )

////@end shrimp_main_frame event table entries
	
	EVT_TIMER(ID_TIMER, shrimp_main_frame::on_timer)

	EVT_JOY_BUTTON_DOWN(shrimp_main_frame::joy_handler)

END_EVENT_TABLE()

/*!
 * shrimp_main_frame constructors
 */

shrimp_main_frame::shrimp_main_frame()
{
    Init();
}

shrimp_main_frame::shrimp_main_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * shrimp_main_frame creator
 */

bool shrimp_main_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin shrimp_main_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end shrimp_main_frame creation
    return true;
}

/*!
 * shrimp_main_frame destructor
 */

shrimp_main_frame::~shrimp_main_frame()
{
////@begin shrimp_main_frame destruction
////@end shrimp_main_frame destruction
}

/*!
 * Member initialisation 
 */

void shrimp_main_frame::Init()
{
////@begin shrimp_main_frame member initialisation
    m_status_bar = NULL;
    m_cam_panel = NULL;
    m_speed_txt = NULL;
    m_steer_txt = NULL;
    m_voltage_txt = NULL;
    m_robot_on_off_button = NULL;
    m_em_on_off_button = NULL;
////@end shrimp_main_frame member initialisation

	m_shrimp = new all::act::shrimp_client_t;
	m_shrimp->run_async();

	m_cam_panel = new all::core::wxpanel_stream_dest_t();

	m_stream_client = new all::core::stream_client_t(*m_cam_panel);
	m_stream_client->run_async();

	m_pololu = new all::act::pololu_ctrl_client_t;
	m_pololu->run_async();
	
	m_timer = new wxTimer(this, ID_TIMER);
	m_timer->Start(100);

	

}
/*!
 * Control creation for shrimp_main_frame
 */

void shrimp_main_frame::CreateControls()
{    
////@begin shrimp_main_frame content construction
    shrimp_main_frame* itemFrame1 = this;

    m_status_bar = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    m_status_bar->SetFieldsCount(2);
    itemFrame1->SetStatusBar(m_status_bar);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer3);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

	m_cam_panel->Create( itemFrame1, ID_CAM_PANEL, wxDefaultPosition, wxSize(320, 240), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    itemBoxSizer4->Add(m_cam_panel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer4->Add(30, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer4->Add(itemBoxSizer7, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer8Static = new wxStaticBox(itemFrame1, wxID_STATIC, _("Speed"));
    wxStaticBoxSizer* itemStaticBoxSizer8 = new wxStaticBoxSizer(itemStaticBoxSizer8Static, wxHORIZONTAL);
    itemBoxSizer7->Add(itemStaticBoxSizer8, 0, wxGROW|wxALL, 5);

    m_speed_txt = new wxStaticText( itemFrame1, wxID_STATIC, _T(""), wxPoint(0, 0), wxSize(50, -1), wxNO_BORDER );
    itemStaticBoxSizer8->Add(m_speed_txt, 0, wxGROW|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(itemFrame1, wxID_STATIC, _("Steer"));
    wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxHORIZONTAL);
    itemBoxSizer7->Add(itemStaticBoxSizer10, 0, wxGROW|wxALL, 5);

    m_steer_txt = new wxStaticText( itemFrame1, wxID_STATIC, _T(""), wxDefaultPosition, wxSize(50, -1), 0 );
    itemStaticBoxSizer10->Add(m_steer_txt, 0, wxGROW|wxALL|wxADJUST_MINSIZE, 5);

    itemBoxSizer7->Add(5, 30, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

    wxStaticBox* itemStaticBoxSizer13Static = new wxStaticBox(itemFrame1, wxID_STATIC, _("Voltage"));
    wxStaticBoxSizer* itemStaticBoxSizer13 = new wxStaticBoxSizer(itemStaticBoxSizer13Static, wxHORIZONTAL);
    itemBoxSizer7->Add(itemStaticBoxSizer13, 0, wxGROW|wxALL, 5);

    m_voltage_txt = new wxStaticText( itemFrame1, wxID_STATIC, _T(""), wxDefaultPosition, wxSize(50, -1), 0 );
    itemStaticBoxSizer13->Add(m_voltage_txt, 0, wxGROW|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticBox* itemStaticBoxSizer15Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Stream control"));
    wxStaticBoxSizer* itemStaticBoxSizer15 = new wxStaticBoxSizer(itemStaticBoxSizer15Static, wxHORIZONTAL);
    itemBoxSizer3->Add(itemStaticBoxSizer15, 0, wxALIGN_LEFT|wxALL, 5);

    wxButton* itemButton16 = new wxButton( itemFrame1, ID_PLAY, _("Start"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer15->Add(itemButton16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton17 = new wxButton( itemFrame1, ID_STOP, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer15->Add(itemButton17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Robot control"));
    wxStaticBoxSizer* itemStaticBoxSizer18 = new wxStaticBoxSizer(itemStaticBoxSizer18Static, wxHORIZONTAL);
    itemBoxSizer3->Add(itemStaticBoxSizer18, 0, wxALIGN_LEFT|wxALL|wxFIXED_MINSIZE, 5);

    m_robot_on_off_button = new wxToggleButton( itemFrame1, ID_ON_BUTTON, _("Robot On/Off"), wxDefaultPosition, wxSize(110, -1), 0 );
    m_robot_on_off_button->SetValue(false);
    itemStaticBoxSizer18->Add(m_robot_on_off_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_em_on_off_button = new wxToggleButton( itemFrame1, ID_EM_BUTTON, _("Emergency On/Off"), wxDefaultPosition, wxSize(110, -1), 0 );
    m_em_on_off_button->SetValue(false);
    itemStaticBoxSizer18->Add(m_em_on_off_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end shrimp_main_frame content construction

	m_joy.SetCapture(this, 100);
}

/*!
 * Should we show tooltips?
 */

bool shrimp_main_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap shrimp_main_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin shrimp_main_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end shrimp_main_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon shrimp_main_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin shrimp_main_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end shrimp_main_frame icon retrieval
}



/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON1
 */

void shrimp_main_frame::OnOnButtonClick( wxCommandEvent& event )
{
	if (!m_shrimp->is_on())
		m_shrimp->set_robot_on();
	else
		m_shrimp->set_robot_standby();

}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON2
 */

void shrimp_main_frame::OnEmButtonClick( wxCommandEvent& event )
{
	if (!m_shrimp->is_em())
		m_shrimp->em_stop();
	else
		m_shrimp->em_release();

}


void shrimp_main_frame::on_timer(wxTimerEvent& event) {
	
	//update status bar

	if (m_shrimp->is_connected()) {
		m_status_bar->SetStatusText("Connected", 0);
	}
	else {
		m_status_bar->SetStatusText("Not connected", 0);
	}

	if (m_shrimp->is_on())
		if (m_shrimp->is_em())
			m_status_bar->SetStatusText("Emergency mode", 1);
		else
			m_status_bar->SetStatusText("Robot on", 1);
	else
		m_status_bar->SetStatusText("Robot off", 1);

	
	


	//m_speed_txt->SetLabel(wxString::Format("%i",m_shrimp->get_speed()));
	//m_steer_txt->SetLabel(wxString::Format("%i",m_shrimp->get_steer().deg()));

	int speed = m_joy.get_speed();
	all::math::angle steer = m_joy.get_steer();

	all::math::angle pan, tilt;

	m_joy.get_pan_tilt(pan, tilt);

	m_shrimp->set_speed(speed);
	Sleep(1);
	m_shrimp->set_steer(steer);

	m_pololu->set_pose(0, pan.deg());

	Sleep(1);

	m_pololu->set_pose(1, tilt.deg());

	//m_steer_txt->SetLabel(wxString::Format("%f", m_pololu->get_pose(0)));
	//m_speed_txt->SetLabel(wxString::Format("%f", m_pololu->get_pose(1)));
	
	//m_steer_txt->SetLabel(wxString::Format("%f", pan.deg()));
	//m_speed_txt->SetLabel(wxString::Format("%f", tilt.deg()));
	//m_speed_txt->SetLabel(wxString::Format("%i",speed));
	//m_steer_txt->SetLabel(wxString::Format("%f",steer.deg()));

}

void shrimp_main_frame::joy_handler(wxJoystickEvent& event) {
	
	m_joy.reset_mti();
	/*std::bitset<8> joy_button_state;

	joy_button_state = m_joy.GetButtonState();*/

	//get speed and steer from joystick
	//int speed = m_joy.get_speed();
	//all::math::angle steer = m_joy.get_steer();

	//m_shrimp->set_speed(speed);
	//m_shrimp->set_steer(steer);
	//
	//m_speed_txt->SetLabel(wxString::Format("%i",speed));
	//m_steer_txt->SetLabel(wxString::Format("%f",steer.deg()));
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PLAY
 */

void shrimp_main_frame::OnPlayClick( wxCommandEvent& event )
{
	if (m_stream_client->is_connected())
		m_stream_client->start_receive();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STOP
 */

void shrimp_main_frame::OnStopClick( wxCommandEvent& event )
{
	m_stream_client->stop_receive();

}


