/////////////////////////////////////////////////////////////////////////////
// Name:        exploring_control_mainframe.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     27/03/2007 03:31:41
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "exploring_control_mainframe.h"
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

#include "exploring_control_mainframe.h"

////@begin XPM images
////@end XPM images

/*!
 * exploring_control_mainframe type definition
 */

IMPLEMENT_CLASS( exploring_control_mainframe, wxFrame )

/*!
 * exploring_control_mainframe event table definition
 */

BEGIN_EVENT_TABLE( exploring_control_mainframe, wxFrame )

////@begin exploring_control_mainframe event table entries
    EVT_BUTTON( ID_IDLE_BUTTON, exploring_control_mainframe::OnIdleButtonClick )

    EVT_BUTTON( ID_RESET_BUTTON, exploring_control_mainframe::OnResetButtonClick )

    EVT_BUTTON( ID_RESUME_BUTTON, exploring_control_mainframe::OnResumeButtonClick )

    EVT_BUTTON( ID_EXPLORE_BUTTON, exploring_control_mainframe::OnExploreButtonClick )

////@end exploring_control_mainframe event table entries

END_EVENT_TABLE()

/*!
 * exploring_control_mainframe constructors
 */

exploring_control_mainframe::exploring_control_mainframe()
{
    Init();
}

exploring_control_mainframe::exploring_control_mainframe( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * exploring_control_mainframe creator
 */

bool exploring_control_mainframe::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin exploring_control_mainframe creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end exploring_control_mainframe creation
    return true;
}

/*!
 * exploring_control_mainframe destructor
 */

exploring_control_mainframe::~exploring_control_mainframe()
{
////@begin exploring_control_mainframe destruction
////@end exploring_control_mainframe destruction
}

/*!
 * Member initialisation 
 */

void exploring_control_mainframe::Init()
{
////@begin exploring_control_mainframe member initialisation
////@end exploring_control_mainframe member initialisation
}

/*!
 * Control creation for exploring_control_mainframe
 */

void exploring_control_mainframe::CreateControls()
{    
////@begin exploring_control_mainframe content construction
    exploring_control_mainframe* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Dispatcher"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton4 = new wxButton;
    itemButton4->Create( itemFrame1, ID_IDLE_BUTTON, _("Idle"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(itemButton4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton5 = new wxButton;
    itemButton5->Create( itemFrame1, ID_RESET_BUTTON, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton;
    itemButton6->Create( itemFrame1, ID_RESUME_BUTTON, _("Resume"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton;
    itemButton7->Create( itemFrame1, ID_EXPLORE_BUTTON, _("Explore"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer3->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end exploring_control_mainframe content construction
    dispatcher_.reset(new all::xpr::task_dispatcher_t("config/xpr_service.ini"));
}

/*!
 * Should we show tooltips?
 */

bool exploring_control_mainframe::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap exploring_control_mainframe::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin exploring_control_mainframe bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end exploring_control_mainframe bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon exploring_control_mainframe::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin exploring_control_mainframe icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end exploring_control_mainframe icon retrieval
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_IDLE_BUTTON
 */

void exploring_control_mainframe::OnIdleButtonClick( wxCommandEvent& event )
{
  dispatcher_->send_event(all::xpr::etag::IDLE);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EXPLORE_BUTTON
 */

void exploring_control_mainframe::OnExploreButtonClick( wxCommandEvent& event )
{
  dispatcher_->send_event(all::xpr::etag::EXPLORE);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESUME_BUTTON
 */

void exploring_control_mainframe::OnResumeButtonClick( wxCommandEvent& event )
{
  dispatcher_->send_event(all::xpr::etag::RESUME);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESET_BUTTON
 */

void exploring_control_mainframe::OnResetButtonClick( wxCommandEvent& event )
{
  dispatcher_->send_event(all::xpr::etag::RESET);
}

