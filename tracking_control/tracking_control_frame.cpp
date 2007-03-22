/////////////////////////////////////////////////////////////////////////////
// Name:        tracking_control_frame.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     05/03/2007 23:16:16
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "tracking_control_frame.h"
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
#include "../../alcor/core/wx_image_panel.h"
////@end includes

#include "tracking_control_frame.h"

////@begin XPM images
////@end XPM images

/*!
 * tracking_control_frame type definition
 */

IMPLEMENT_CLASS( tracking_control_frame, wxFrame )

/*!
 * tracking_control_frame event table definition
 */

BEGIN_EVENT_TABLE( tracking_control_frame, wxFrame )

////@begin tracking_control_frame event table entries
    EVT_CLOSE( tracking_control_frame::OnCloseWindow )

    EVT_BUTTON( ID_BUTTON_TRACK, tracking_control_frame::OnButtonTrackClick )

    EVT_BUTTON( ID_BUTTON_SETUP, tracking_control_frame::OnButtonSetupClick )

    EVT_BUTTON( ID_SEND_ROI_BUTTON, tracking_control_frame::OnSendRoiButtonClick )

    EVT_BUTTON( ID_BUTTON_IDLE, tracking_control_frame::OnButtonIdleClick )

    EVT_BUTTON( ID_BUTTON_RESET, tracking_control_frame::OnButtonResetClick )

////@end tracking_control_frame event table entries

END_EVENT_TABLE()

/*!
 * tracking_control_frame constructors
 */

tracking_control_frame::tracking_control_frame( )
{
    Init();
}

tracking_control_frame::tracking_control_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * tracking_control_frame creator
 */

bool tracking_control_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin tracking_control_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end tracking_control_frame creation
    return true;
}

/*!
 * Member initialisation 
 */

void tracking_control_frame::Init()
{
////@begin tracking_control_frame member initialisation
    track_button = NULL;
    setup_button = NULL;
    send_roi_button = NULL;
    idle_button = NULL;
    reset_button = NULL;
    m_scala_resize = NULL;
////@end tracking_control_frame member initialisation
}
/*!
 * Control creation for tracking_control_frame
 */

void tracking_control_frame::CreateControls()
{    
////@begin tracking_control_frame content construction
    tracking_control_frame* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_image_panel = new wx_image_panel( itemFrame1, ID__IMAGE_PANEL, wxDefaultPosition, wxSize(320, 240), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer3->Add(m_image_panel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    track_button = new wxButton( itemFrame1, ID_BUTTON_TRACK, _("Track"), wxDefaultPosition, wxSize(-1, 35), 0 );
    track_button->SetBackgroundColour(wxColour(219, 240, 213));
    track_button->Enable(false);
    itemBoxSizer3->Add(track_button, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Control"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
    itemStaticBoxSizer6Static->SetForegroundColour(wxColour(227, 239, 240));
    itemBoxSizer2->Add(itemStaticBoxSizer6, 0, wxALIGN_TOP|wxALL, 5);

    setup_button = new wxButton( itemFrame1, ID_BUTTON_SETUP, _("Setup"), wxDefaultPosition, wxSize(80, 40), 0 );
    setup_button->SetDefault();
    itemStaticBoxSizer6->Add(setup_button, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    send_roi_button = new wxButton( itemFrame1, ID_SEND_ROI_BUTTON, _("Send ROI"), wxDefaultPosition, wxDefaultSize, 0 );
    send_roi_button->Enable(false);
    itemStaticBoxSizer6->Add(send_roi_button, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemStaticBoxSizer6->Add(5, 10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    idle_button = new wxButton( itemFrame1, ID_BUTTON_IDLE, _("Idle"), wxDefaultPosition, wxSize(80, 40), 0 );
    idle_button->Enable(false);
    itemStaticBoxSizer6->Add(idle_button, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemStaticBoxSizer6->Add(5, 10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    reset_button = new wxButton( itemFrame1, ID_BUTTON_RESET, _("Reset"), wxDefaultPosition, wxSize(80, 40), 0 );
    reset_button->Enable(false);
    itemStaticBoxSizer6->Add(reset_button, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemStaticBoxSizer6->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_scala_resize = new wxSpinCtrl( itemFrame1, ID_SPINCTRL_SCALA, _T("50"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 25, 100, 50 );
    if (ShowToolTips())
        m_scala_resize->SetToolTip(_("Image Scale Factor"));
    itemStaticBoxSizer6->Add(m_scala_resize, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxSlider* itemSlider15 = new wxSlider( itemFrame1, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer6->Add(itemSlider15, 0, wxGROW|wxALL, 5);

////@end tracking_control_frame content construction

    dispatcher.reset(new all::trm::task_dispatcher("config/trm_service.ini"));
    dispatcher->run_async();
}

/*!
 * Should we show tooltips?
 */

bool tracking_control_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap tracking_control_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin tracking_control_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end tracking_control_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon tracking_control_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin tracking_control_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end tracking_control_frame icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SETUP
 */

void tracking_control_frame::OnButtonSetupClick( wxCommandEvent& WXUNUSED(event) )
{
  m_image_panel->clear_selection();
  m_image_panel->SetEnableDragging(true);

  setup_button->Enable(false);
  track_button->Enable(false);
  idle_button->Enable(false);
  reset_button->Enable(true); 
  send_roi_button->Enable(true);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESET
 */

void tracking_control_frame::OnButtonResetClick( wxCommandEvent& WXUNUSED(event))
{
  dispatcher->send_event(all::trm::etag::RESET);
  setup_button->Enable(true);
  track_button->Enable(false);
  idle_button->Enable(false);
  reset_button->Enable(false); 
  send_roi_button->Enable(false);

    m_image_panel->clear_selection();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON3
 */

void tracking_control_frame::OnButtonIdleClick( wxCommandEvent& WXUNUSED(event) )
{
  dispatcher->send_event(all::trm::etag::IDLETRACK);
  setup_button->Enable(false);
  track_button->Enable(true);
  idle_button->Enable(false);
  reset_button->Enable(true); 
  send_roi_button->Enable(false);
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_TRACKING_CONTROL_FRAME
 */

void tracking_control_frame::OnCloseWindow( wxCloseEvent& WXUNUSED(event) )
{
  dispatcher->stop();
   Destroy(); 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void tracking_control_frame::OnButtonTrackClick( wxCommandEvent& event )
{
  dispatcher->send_event(all::trm::etag::TRACK);   
  track_button->Enable(false);
  idle_button->Enable(true);
  reset_button->Enable(true);
  send_roi_button->Enable(false); 

  m_image_panel->clear_selection();
  m_image_panel->SetEnableDragging(false);
}



/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SEND_ROI_BUTTON
 */

void tracking_control_frame::OnSendRoiButtonClick( wxCommandEvent& event )
{
  track_button->Enable(true);
  idle_button->Enable(true);
  reset_button->Enable(true);
  send_roi_button->Enable(false);

  int r = m_image_panel->Getget_topleft_y();
  int c = m_image_panel->Getget_topleft_x();
  int h = m_image_panel->Getget_roi_height();
  int w = m_image_panel->Getget_roi_width();

  int scala  = m_scala_resize->GetValue();

  dispatcher->send_roi(r,c,h,w, scala);

  m_image_panel->clear_selection();
  m_image_panel->SetEnableDragging(false);
}


