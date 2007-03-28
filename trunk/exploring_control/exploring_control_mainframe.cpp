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

////@end exploring_control_mainframe content construction
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
