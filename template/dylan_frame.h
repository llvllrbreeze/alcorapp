#include "wx/wx.h" 
#include "wx/joystick.h"
//#include "alcor/act/pilota_sim.hpp"
#include "alcor/act/p3_client_t.h"

class MyFrame: public wxFrame
{
private :
	wxTextCtrl* msg;
	//pilota_sim pilota;
	wxJoystick* pad;
	all::act::p3_client_t* pilota_c;

public:

	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnStartWandering(wxCommandEvent& event);
	void OnStopRobot(wxCommandEvent& event);

	void OnJoystick(wxJoystickEvent& event);

	DECLARE_EVENT_TABLE()
};

enum
{
ID_Quit = 1,
ID_About,
ID_COMMAND_PANEL = 1001,
ID_BTN_STOP = 1002,
ID_BTN_SW = 1003,
ID_BUTTON_STATIC,
ID_GLCANVAS,
ID_TEXTCTRL,
};


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Quit, MyFrame::OnQuit)
EVT_MENU(ID_About, MyFrame::OnAbout)
EVT_BUTTON(ID_BTN_SW, MyFrame::OnStartWandering)
EVT_BUTTON(ID_BTN_STOP, MyFrame::OnStopRobot)
EVT_JOYSTICK_EVENTS(MyFrame::OnJoystick)
END_EVENT_TABLE()