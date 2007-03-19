/////////////////////////////////////////////////////////////////////////////
// Name:        wx_test_frame.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     16/03/2007 15:14:44
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wx_test_frame.h"
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
#include "wx_stream_panel_t.hpp"
////@end includes

#include "wx_test_frame.h"

////@begin XPM images
////@end XPM images

/*!
 * wx_test_frame type definition
 */

IMPLEMENT_CLASS( wx_test_frame, wxFrame )

/*!
 * wx_test_frame event table definition
 */

BEGIN_EVENT_TABLE( wx_test_frame, wxFrame )

////@begin wx_test_frame event table entries
    EVT_CLOSE( wx_test_frame::OnCloseWindow )

////@end wx_test_frame event table entries

END_EVENT_TABLE()

/*!
 * wx_test_frame constructors
 */

wx_test_frame::wx_test_frame()
{
    Init();
}

wx_test_frame::wx_test_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * wx_test_frame creator
 */

bool wx_test_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wx_test_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end wx_test_frame creation
    return true;
}

/*!
 * wx_test_frame destructor
 */

wx_test_frame::~wx_test_frame()
{
////@begin wx_test_frame destruction
////@end wx_test_frame destruction
}

/*!
 * Member initialisation 
 */

void wx_test_frame::Init()
{
////@begin wx_test_frame member initialisation
////@end wx_test_frame member initialisation
}
/*!
 * Control creation for wx_test_frame
 */

void wx_test_frame::CreateControls()
{    
////@begin wx_test_frame content construction
    wx_test_frame* itemFrame1 = this;

    m_stream_panel = new wx_stream_panel_t;
    m_stream_panel->Create( itemFrame1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

////@end wx_test_frame content construction

}

/*!
 * Should we show tooltips?
 */

bool wx_test_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wx_test_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wx_test_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wx_test_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wx_test_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wx_test_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wx_test_frame icon retrieval
}
/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_WX_TEST_FRAME
 */

void wx_test_frame::OnCloseWindow( wxCloseEvent& event )
{
	Destroy();
}


