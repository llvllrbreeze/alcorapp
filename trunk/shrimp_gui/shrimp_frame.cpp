/////////////////////////////////////////////////////////////////////////////
// Name:        shrimp_frame.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     21/03/2007 14:44:35
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "shrimp_frame.h"
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

#include "shrimp_frame.h"

////@begin XPM images
////@end XPM images

#include <bitset>

/*!
 * shrimp_frame type definition
 */

IMPLEMENT_CLASS( shrimp_frame, wxFrame )

/*!
 * shrimp_frame event table definition
 */

BEGIN_EVENT_TABLE( shrimp_frame, wxFrame )

////@begin shrimp_frame event table entries
    EVT_TOGGLEBUTTON( ID_ON_BUTTON, shrimp_frame::OnOnButtonClick )

    EVT_TOGGLEBUTTON( ID_EM_BUTTON, shrimp_frame::OnEmButtonClick )

    EVT_BUTTON( ID_SCAN_BUTTON, shrimp_frame::OnScanButtonClick )

////@end shrimp_frame event table entries
	
	EVT_TIMER(ID_TIMER, shrimp_frame::on_timer)
	EVT_JOY_BUTTON_DOWN(shrimp_frame::on_joy_click)
	EVT_JOY_MOVE(shrimp_frame::on_joy_move)

END_EVENT_TABLE()

/*!
 * shrimp_frame constructors
 */

shrimp_frame::shrimp_frame()
{
    Init();
}

shrimp_frame::shrimp_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * shrimp_frame creator
 */

bool shrimp_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin shrimp_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end shrimp_frame creation
    return true;
}

/*!
 * shrimp_frame destructor
 */

shrimp_frame::~shrimp_frame()
{
////@begin shrimp_frame destruction
////@end shrimp_frame destruction
}

/*!
 * Member initialisation 
 */

void shrimp_frame::Init()
{
////@begin shrimp_frame member initialisation
    m_status_bar = NULL;
    m_shrimp_tab = NULL;
    m_robot_view_panel = NULL;
    m_speed_gauge = NULL;
    m_speed_text = NULL;
    m_panoramic_view_panel = NULL;
    m_on_button = NULL;
    m_em_button = NULL;
    m_steer_gauge = NULL;
    m_steer_text = NULL;
    m_voltage_text = NULL;
    m_gl_stream = NULL;
////@end shrimp_frame member initialisation

	m_shrimp = new all::act::shrimp_client_t;
	m_shrimp->run_async();

	m_pololu = new all::act::pololu_ctrl_client_t;
	m_pololu->run_async();
	
	m_timer = new wxTimer(this, ID_TIMER);
	m_timer->Start(100);


}
/*!
 * Control creation for shrimp_frame
 */

void shrimp_frame::CreateControls()
{    
////@begin shrimp_frame content construction
    shrimp_frame* itemFrame1 = this;

    m_status_bar = new wxStatusBar;
    m_status_bar->Create( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    m_status_bar->SetFieldsCount(2);
    itemFrame1->SetStatusBar(m_status_bar);

    m_shrimp_tab = new wxNotebook;
    m_shrimp_tab->Create( itemFrame1, ID_SHRIMP_TAB, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );

    wxPanel* itemPanel4 = new wxPanel;
    itemPanel4->Create( m_shrimp_tab, ID_TELEOPERATION_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer6, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer7Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Robot view"));
    wxStaticBoxSizer* itemStaticBoxSizer7 = new wxStaticBoxSizer(itemStaticBoxSizer7Static, wxHORIZONTAL);
    itemBoxSizer6->Add(itemStaticBoxSizer7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_robot_view_panel = new wx_img_stream_panel_t( "config/fire_stream_client.ini" );
    m_robot_view_panel->Create( itemPanel4, ID_ROBOT_VIEW_PANEL, wxDefaultPosition, wxSize(320, 240), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemStaticBoxSizer7->Add(m_robot_view_panel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer6->Add(0, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Speed"));
    wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxVERTICAL);
    itemBoxSizer6->Add(itemStaticBoxSizer10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_speed_gauge = new wxGauge;
    m_speed_gauge->Create( itemPanel4, ID_SPEED_GAUGE, 450, wxDefaultPosition, wxSize(20, 225), wxGA_VERTICAL|wxGA_SMOOTH );
    m_speed_gauge->SetValue(0);
    itemStaticBoxSizer10->Add(m_speed_gauge, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_speed_text = new wxStaticText;
    m_speed_text->Create( itemPanel4, wxID_STATIC, _("450"), wxDefaultPosition, wxSize(-1, 12), 0 );
    itemStaticBoxSizer10->Add(m_speed_text, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    itemBoxSizer6->Add(0, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer14Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Panoramic view"));
    wxStaticBoxSizer* itemStaticBoxSizer14 = new wxStaticBoxSizer(itemStaticBoxSizer14Static, wxHORIZONTAL);
    itemBoxSizer6->Add(itemStaticBoxSizer14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_panoramic_view_panel = new wx_img_stream_panel_t( "config/cmos_stream_client.ini" );
    m_panoramic_view_panel->Create( itemPanel4, ID_PANORAMIC_VIEW_PANEL, wxDefaultPosition, wxSize(320, 240), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemStaticBoxSizer14->Add(m_panoramic_view_panel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer17Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Robot Controls"));
    wxStaticBoxSizer* itemStaticBoxSizer17 = new wxStaticBoxSizer(itemStaticBoxSizer17Static, wxHORIZONTAL);
    itemBoxSizer16->Add(itemStaticBoxSizer17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_on_button = new wxToggleButton;
    m_on_button->Create( itemPanel4, ID_ON_BUTTON, _("Robot On"), wxDefaultPosition, wxSize(100, -1), 0 );
    m_on_button->SetValue(false);
    itemStaticBoxSizer17->Add(m_on_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_em_button = new wxToggleButton;
    m_em_button->Create( itemPanel4, ID_EM_BUTTON, _("Emergency Stop"), wxDefaultPosition, wxSize(100, -1), 0 );
    m_em_button->SetValue(false);
    itemStaticBoxSizer17->Add(m_em_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer16->Add(10, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer21Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Steer"));
    wxStaticBoxSizer* itemStaticBoxSizer21 = new wxStaticBoxSizer(itemStaticBoxSizer21Static, wxHORIZONTAL);
    itemStaticBoxSizer21Static->Enable(false);
    itemBoxSizer16->Add(itemStaticBoxSizer21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_steer_gauge = new wxGauge;
    m_steer_gauge->Create( itemPanel4, ID_STEER_GAUGE, 180, wxDefaultPosition, wxSize(240, 20), wxGA_HORIZONTAL|wxGA_SMOOTH );
    m_steer_gauge->SetValue(90);
    itemStaticBoxSizer21->Add(m_steer_gauge, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_steer_text = new wxStaticText;
    m_steer_text->Create( itemPanel4, wxID_STATIC, _("90"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer21->Add(m_steer_text, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    itemBoxSizer16->Add(110, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer25Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Robot Status"));
    wxStaticBoxSizer* itemStaticBoxSizer25 = new wxStaticBoxSizer(itemStaticBoxSizer25Static, wxHORIZONTAL);
    itemBoxSizer16->Add(itemStaticBoxSizer25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticBox* itemStaticBoxSizer26Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Voltage"));
    wxStaticBoxSizer* itemStaticBoxSizer26 = new wxStaticBoxSizer(itemStaticBoxSizer26Static, wxHORIZONTAL);
    itemStaticBoxSizer25->Add(itemStaticBoxSizer26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_voltage_text = new wxStaticText;
    m_voltage_text->Create( itemPanel4, wxID_STATIC, _T(""), wxDefaultPosition, wxSize(50, -1), 0 );
    itemStaticBoxSizer26->Add(m_voltage_text, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    m_shrimp_tab->AddPage(itemPanel4, _("Teleoperation"));

    wxPanel* itemPanel28 = new wxPanel;
    itemPanel28->Create( m_shrimp_tab, ID_3D_SCAN_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel28->SetSizer(itemBoxSizer29);

    wxStaticBox* itemStaticBoxSizer30Static = new wxStaticBox(itemPanel28, wxID_ANY, _("Laser scan"));
    wxStaticBoxSizer* itemStaticBoxSizer30 = new wxStaticBoxSizer(itemStaticBoxSizer30Static, wxHORIZONTAL);
    itemBoxSizer29->Add(itemStaticBoxSizer30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_gl_stream = new wx_gl_stream_client_t( "config/gl_stream_client.ini" );
    m_gl_stream->Create( itemPanel28, ID_GL_STREAM, wxDefaultPosition, wxSize(640, 350), wxTAB_TRAVERSAL );
    itemStaticBoxSizer30->Add(m_gl_stream, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer29->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer29->Add(itemBoxSizer33, 0, wxALIGN_TOP|wxALL, 5);
    itemBoxSizer33->Add(5, 10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton35 = new wxButton;
    itemButton35->Create( itemPanel28, ID_SCAN_BUTTON, _("Scan"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer33->Add(itemButton35, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_shrimp_tab->AddPage(itemPanel28, _("3D Scan"));

////@end shrimp_frame content construction

	m_joy.SetCapture(this, 100);
}

/*!
 * Should we show tooltips?
 */

bool shrimp_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap shrimp_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin shrimp_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end shrimp_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon shrimp_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin shrimp_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end shrimp_frame icon retrieval
}
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON
 */

void shrimp_frame::OnOnButtonClick( wxCommandEvent& event )
{
	if (!m_shrimp->is_on()) {
		m_shrimp->set_robot_on();
		if (m_shrimp->is_on()) {
			m_on_button->SetLabel(wxString("Robot Off"));
			m_on_button->SetValue(true);
		}
		else {
			m_on_button->SetLabel(wxString("Robot On"));
			m_on_button->SetValue(false);
		}
	}
	else {
		m_shrimp->set_robot_standby();
		if (!m_shrimp->is_on()) {
			m_on_button->SetLabel(wxString("Robot On"));
			m_on_button->SetValue(false);
		}
		else {
			m_on_button->SetLabel(wxString("Robot Off"));
			m_on_button->SetValue(true);
		}
	}
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON3
 */

void shrimp_frame::OnEmButtonClick( wxCommandEvent& event )
{
	if (!m_shrimp->is_em()) {
		m_shrimp->em_stop();
		if (m_shrimp->is_em()) {
			m_em_button->SetLabel(wxString("Emergency Release"));
			m_em_button->SetValue(true);
		}
		else {
			m_em_button->SetLabel(wxString("Emergency Stop"));
			m_em_button->SetValue(false);
		}
			
	}
	else {
		m_shrimp->em_release();
		if (!m_shrimp->is_em()) {
			m_em_button->SetLabel(wxString("Emergency Stop"));
			m_em_button->SetValue(false);
		}
		else {
			m_em_button->SetLabel(wxString("Emergency Release"));
			m_em_button->SetValue(true);
		}

	}
}


void shrimp_frame::on_joy_click(wxJoystickEvent& event) {
	std::bitset<8> joy_button_state;
	joy_button_state = m_joy.GetButtonState();

	if (joy_button_state.test(5)) {
		m_shrimp_tab->AdvanceSelection();
	}

	if (joy_button_state.test(2)) {
		m_joy.reset_mti();
	}


}

void shrimp_frame::on_joy_move(wxJoystickEvent& event) {
	int speed = m_joy.get_speed();
	all::math::angle steer = m_joy.get_steer();

	m_shrimp->set_speed(speed);
	m_shrimp->set_steer(steer);

	if (abs(speed) <50)
		speed = 0;

	//m_speed_gauge->SetForegroundColour(wxColour(255,255,0));
	m_speed_gauge->SetValue(abs(speed));
	m_steer_gauge->SetValue((int)steer.deg() + 90);


}

void shrimp_frame::on_timer(wxTimerEvent& event) {

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

	
	m_speed_text->SetLabel(wxString::Format("%i",m_shrimp->get_speed()));
	m_steer_text->SetLabel(wxString::Format("%i", (int) m_shrimp->get_steer().deg()));

	m_voltage_text->SetLabel(wxString::Format("%f", m_shrimp->get_voltage()));
	
	//get inertial value and set pan tilt position
	all::math::angle pan, tilt;

	m_joy.get_pan_tilt(pan, tilt);
	
	m_pololu->set_pose(6, pan.deg());
	m_pololu->set_pose(7, tilt.deg());

}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCAN_BUTTON
 */

void shrimp_frame::OnScanButtonClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCAN_BUTTON in shrimp_frame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SCAN_BUTTON in shrimp_frame. 
}


