/////////////////////////////////////////////////////////////////////////////
// Name:        calib_frame.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     02/04/2007 02:22:23
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "calib_frame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include "alcor.apps/gaze/gaze_machine_VI_t.h"
using namespace all;

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "calib_frame.h"

////@begin XPM images
////@end XPM images

/*!
 * calib_frame type definition
 */

IMPLEMENT_CLASS( calib_frame, wxFrame )

/*!
 * calib_frame event table definition
 */

BEGIN_EVENT_TABLE( calib_frame, wxFrame )

////@begin calib_frame event table entries
    EVT_CLOSE( calib_frame::OnCloseWindow )

    EVT_BUTTON( ID_DO_CALIB_BUTTON, calib_frame::OnDoCalibButtonClick )

    EVT_BUTTON( ID_STOP_BUTTON, calib_frame::OnStopButtonClick )

////@end calib_frame event table entries

END_EVENT_TABLE()

/*!
 * calib_frame constructors
 */

calib_frame::calib_frame()
{
    Init();
}

calib_frame::calib_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * calib_frame creator
 */

bool calib_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin calib_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end calib_frame creation
    return true;
}

/*!
 * calib_frame destructor
 */

calib_frame::~calib_frame()
{
////@begin calib_frame destruction
////@end calib_frame destruction
}

/*!
 * Member initialisation 
 */

void calib_frame::Init()
{
////@begin calib_frame member initialisation
    go_calib_btn = NULL;
////@end calib_frame member initialisation
  gaze_.reset(new all::gaze::gaze_machine_VI_t);

}

/*!
 * Control creation for calib_frame
 */

void calib_frame::CreateControls()
{    
////@begin calib_frame content construction
    calib_frame* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    go_calib_btn = new wxButton;
    go_calib_btn->Create( itemFrame1, ID_DO_CALIB_BUTTON, _("Calibrate"), wxDefaultPosition, wxSize(90, 80), 0 );
    itemBoxSizer2->Add(go_calib_btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer2->Add(5, 5, 0, wxGROW|wxALL, 5);

    wxButton* itemButton5 = new wxButton;
    itemButton5->Create( itemFrame1, ID_STOP_BUTTON, _("Save"), wxDefaultPosition, wxSize(90, 30), 0 );
    itemBoxSizer2->Add(itemButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end calib_frame content construction
}

/*!
 * Should we show tooltips?
 */

bool calib_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap calib_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin calib_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end calib_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon calib_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin calib_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end calib_frame icon retrieval
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DO_CALIB_BUTTON
 */

void calib_frame::OnDoCalibButtonClick( wxCommandEvent& event )
{
  gaze_->run_machine(gaze::calib);
  go_calib_btn->Enable(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STOP_BUTTON
 */

void calib_frame::OnStopButtonClick( wxCommandEvent& event )
{
  gaze_->save_calib();
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_CALIB_FRAME
 */

void calib_frame::OnCloseWindow( wxCloseEvent& event )
{
  gaze_->cancel();
  core::BOOST_SLEEP(200);
  Destroy();
}

