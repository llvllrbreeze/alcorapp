/////////////////////////////////////////////////////////////////////////////
// Name:        urg_2d_frame.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     10/03/2007 12:59:19
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "urg_2d_frame.h"
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

#include "urg_2d_frame.h"

////@begin XPM images

////@end XPM images

/*!
 * urg_2d_frame type definition
 */

IMPLEMENT_CLASS( urg_2d_frame, wxFrame )

/*!
 * urg_2d_frame event table definition
 */

BEGIN_EVENT_TABLE( urg_2d_frame, wxFrame )

////@begin urg_2d_frame event table entries
////@end urg_2d_frame event table entries

END_EVENT_TABLE()

/*!
 * urg_2d_frame constructors
 */

urg_2d_frame::urg_2d_frame()
{
    Init();
}

urg_2d_frame::urg_2d_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * urg_2d_frame creator
 */

bool urg_2d_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin urg_2d_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end urg_2d_frame creation
    return true;
}

/*!
 * urg_2d_frame destructor
 */

urg_2d_frame::~urg_2d_frame()
{
////@begin urg_2d_frame destruction
////@end urg_2d_frame destruction
}

/*!
 * Member initialisation 
 */

void urg_2d_frame::Init()
{
////@begin urg_2d_frame member initialisation
////@end urg_2d_frame member initialisation
}
/*!
 * Control creation for urg_2d_frame
 */

void urg_2d_frame::CreateControls()
{    
////@begin urg_2d_frame content construction
    urg_2d_frame* itemFrame1 = this;

////@end urg_2d_frame content construction
}

/*!
 * Should we show tooltips?
 */

bool urg_2d_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap urg_2d_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin urg_2d_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end urg_2d_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon urg_2d_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin urg_2d_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end urg_2d_frame icon retrieval
}
