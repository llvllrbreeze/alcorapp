#include "dylan_app.h"
#include "dylan_frame.h"
#include "wx/glcanvas.h"

IMPLEMENT_APP(MyApp) 

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame( _T("DYLAN - (Dynamic Localizing And Navigation)"), wxPoint(150,50), wxSize(360,500) );
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
} 

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	
	pilota_c = new all::act::p3_client_t("config/p3_conf.ini");
	pilota_c->run_async();

pad = new wxJoystick(wxJOYSTICK1);
if(pad->IsOk())
	pad->SetCapture(this,1);


wxMenu *menuFile = new wxMenu;

menuFile->Append( ID_About, _T("&About...") );
menuFile->AppendSeparator();
menuFile->Append( ID_Quit, _T("E&xit") );

wxMenuBar *menuBar = new wxMenuBar;
menuBar->Append( menuFile, _T("&File") );

SetMenuBar( menuBar );

CreateStatusBar();
SetStatusText( _T("AlcorLAB - Dylan GUI") );

//--


    wxPanel* itemPanel2 = new wxPanel( this, ID_COMMAND_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

	wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Robot Behaviors"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Map"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxHORIZONTAL);

    itemPanel2->SetSizer(verticalSizer);
	verticalSizer->Add(itemStaticBoxSizer3,0,wxALIGN_CENTER_VERTICAL|wxALL,5);
	verticalSizer->Add(itemStaticBoxSizer4,0,wxALIGN_CENTER_VERTICAL|wxALL,5);

    wxButton* itemButton4 = new wxButton( itemPanel2, ID_BTN_SW, _("Start Wandering"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(itemButton4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemPanel2, ID_BTN_STOP, _("STOP DYLAN"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton5->SetForegroundColour(wxColour(255, 255, 255));
    itemButton5->SetBackgroundColour(wxColour(255, 0, 0));
	itemButton5->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Verdana")));
    itemStaticBoxSizer3->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	MyFrame::msg = new wxTextCtrl( itemPanel2, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(msg, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);


    wxGLCanvas* mapBox = new wxGLCanvas(itemPanel2,ID_GLCANVAS,wxDefaultPosition,wxSize(200,200));
	itemStaticBoxSizer4->Add(mapBox,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);

	if(pilota_c->is_connected()) MyFrame::msg->SetValue(_("Connesso"));
	else MyFrame::msg->SetValue(_("NON Connesso"));
}

void MyFrame::OnStartWandering(wxCommandEvent& WXUNUSED(event))
{
	//pilota.wandering();
		pilota_c->enable_wander_mode();
		MyFrame::msg->SetValue(_("wandering"));
}

void MyFrame::OnStopRobot(wxCommandEvent& WXUNUSED(event))
{
	pilota_c->enable_stop_mode();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	//pilota.cancel();
	Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
wxMessageBox(_T("AlcorLAB - Dylan (Dynamic Localizing And Navigation) USER INTERFACE"),
_T("DylanGUI"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnJoystick(wxJoystickEvent& event){
	wxPoint posJ = wxPoint(pad->GetPosition());
	posJ = posJ- wxPoint(32767,32767);
	int i = pad->GetPOVPosition();
	if(event.IsMove()){
		MyFrame::msg->SetValue(wxString::Format("x:%d y:%d",posJ.x,posJ.y));
		pilota_c->set_vel(-100*posJ.y/32767);
		pilota_c->set_rot_vel(-5*posJ.x/32767);
		//pilota_c->get_odometry();
		//pilota.set_vel(-1000*posJ.y/32767);
		//pilota.set_rot_vel(-10*posJ.x/32767);
	}
	//MyFrame::msg->SetValue(wxString::Format("%d",i));
	//MyFrame::msg->SetValue(wxString::Format("%d",pad->HasPOV()));
	
}