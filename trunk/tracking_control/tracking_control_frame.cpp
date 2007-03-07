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

    EVT_BUTTON( ID_BUTTON_SETUP, tracking_control_frame::OnButtonSetupClick )

    EVT_BUTTON( ID_BUTTON_IDLE, tracking_control_frame::OnButtonIdleClick )

    EVT_BUTTON( ID_BUTTON_RESET, tracking_control_frame::OnButtonResetClick )

    EVT_BUTTON( ID_BUTTON_TRACK, tracking_control_frame::OnButtonTrackClick )

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
////@end tracking_control_frame member initialisation
}
/*!
 * Control creation for tracking_control_frame
 */

void tracking_control_frame::CreateControls()
{    
////@begin tracking_control_frame content construction
    tracking_control_frame* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Control"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton4 = new wxButton( itemFrame1, ID_BUTTON_SETUP, _("Setup"), wxDefaultPosition, wxSize(80, 40), 0 );
    itemButton4->SetDefault();
    itemStaticBoxSizer3->Add(itemButton4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemStaticBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemFrame1, ID_BUTTON_IDLE, _("Idle"), wxDefaultPosition, wxSize(80, 40), 0 );
    itemStaticBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemStaticBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton8 = new wxButton( itemFrame1, ID_BUTTON_RESET, _("Reset"), wxDefaultPosition, wxSize(80, 40), 0 );
    itemStaticBoxSizer3->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemFrame1, ID_BUTTON_TRACK, _("Track"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton10, 0, wxGROW|wxALL, 5);

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
  dispatcher->send_event(all::trm::etag::SETUP);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESET
 */

void tracking_control_frame::OnButtonResetClick( wxCommandEvent& WXUNUSED(event))
{
  dispatcher->send_event(all::trm::etag::RESET);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON3
 */

void tracking_control_frame::OnButtonIdleClick( wxCommandEvent& WXUNUSED(event) )
{
  dispatcher->send_event(all::trm::etag::IDLETRACK);
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_TRACKING_CONTROL_FRAME
 */

void tracking_control_frame::OnCloseWindow( wxCloseEvent& WXUNUSED(event) )
{
   Destroy(); 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TRACK
 */

void tracking_control_frame::OnButtonTrackClick( wxCommandEvent& event )
{
  dispatcher->send_event(all::trm::etag::TRACK);
}


