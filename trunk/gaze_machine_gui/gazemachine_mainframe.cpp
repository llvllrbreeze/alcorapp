/////////////////////////////////////////////////////////////////////////////
// Name:        gazemachine_mainframe.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     25/03/2007 19:46:39
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gazemachine_mainframe.h"
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

#include "gazemachine_mainframe.h"

////@begin XPM images
////@end XPM images

/*!
 * gazemachine_mainframe type definition
 */

IMPLEMENT_CLASS( gazemachine_mainframe, wxFrame )

/*!
 * gazemachine_mainframe event table definition
 */

BEGIN_EVENT_TABLE( gazemachine_mainframe, wxFrame )

////@begin gazemachine_mainframe event table entries
    EVT_BUTTON( ID_CALIB_BUTTON, gazemachine_mainframe::OnCalibButtonClick )

    EVT_BUTTON( ID_GAZELOG_BUTTON, gazemachine_mainframe::OnGazelogButtonClick )

    EVT_BUTTON( ID_STOP_BUTTON, gazemachine_mainframe::OnStopButtonClick )

////@end gazemachine_mainframe event table entries

END_EVENT_TABLE()

/*!
 * gazemachine_mainframe constructors
 */

gazemachine_mainframe::gazemachine_mainframe()
{
    Init();
}

gazemachine_mainframe::gazemachine_mainframe( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * gazemachine_mainframe creator
 */

bool gazemachine_mainframe::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin gazemachine_mainframe creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end gazemachine_mainframe creation
    return true;
}

/*!
 * gazemachine_mainframe destructor
 */

gazemachine_mainframe::~gazemachine_mainframe()
{
////@begin gazemachine_mainframe destruction
////@end gazemachine_mainframe destruction
}

/*!
 * Member initialisation 
 */

void gazemachine_mainframe::Init()
{
////@begin gazemachine_mainframe member initialisation
    caliblog_btn = NULL;
    gazelog_btn = NULL;
    stoplog_btn = NULL;
    m_logname_ = NULL;
////@end gazemachine_mainframe member initialisation

    gaze_.reset(new all::gaze::gaze_machine_t);
}

/*!
 * Control creation for gazemachine_mainframe
 */

void gazemachine_mainframe::CreateControls()
{    
////@begin gazemachine_mainframe content construction
    gazemachine_mainframe* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    caliblog_btn = new wxButton;
    caliblog_btn->Create( itemFrame1, ID_CALIB_BUTTON, _("Calib"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(caliblog_btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    gazelog_btn = new wxButton;
    gazelog_btn->Create( itemFrame1, ID_GAZELOG_BUTTON, _("Log\n"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(gazelog_btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    stoplog_btn = new wxButton;
    stoplog_btn->Create( itemFrame1, ID_STOP_BUTTON, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(stoplog_btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_logname_ = new wxTextCtrl;
    m_logname_->Create( itemFrame1, ID_NAME_TEXTCTRL, _("gazelog.bin"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(m_logname_, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end gazemachine_mainframe content construction
}

/*!
 * Should we show tooltips?
 */

bool gazemachine_mainframe::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap gazemachine_mainframe::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin gazemachine_mainframe bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end gazemachine_mainframe bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon gazemachine_mainframe::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin gazemachine_mainframe icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end gazemachine_mainframe icon retrieval
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CALIB_BUTTON1
 */

void gazemachine_mainframe::OnCalibButtonClick( wxCommandEvent& event )
{
  gaze_->run_machine(gaze::calib);  
  gazelog_btn->Enable(false);
  stoplog_btn->Enable(true);
  caliblog_btn->Enable(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GAZELOG_BUTTON
 */

void gazemachine_mainframe::OnGazelogButtonClick( wxCommandEvent& event )
{ 
  wxString temp = m_logname_->GetValue();
  std::string temps(temp.mb_str());
  gaze_->set_logname( temps );
  gaze_->run_machine(gaze::binlog);
  gazelog_btn->Enable(false);
  stoplog_btn->Enable(true);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STOP_BUTTON
 */

void gazemachine_mainframe::OnStopButtonClick( wxCommandEvent& event )
{
  gaze_->cancel();
  gazelog_btn->Enable(true);
  stoplog_btn->Enable(false);
  caliblog_btn->Enable(true);
}

