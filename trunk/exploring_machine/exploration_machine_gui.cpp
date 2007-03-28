/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_machine_gui.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:33:51
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "exploration_machine_gui.h"
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
#include "tridim_scene_canvas.h"
////@end includes

#include "exploration_machine_gui.h"

////@begin XPM images
////@end XPM images

/*!
 * exploration_machine_gui type definition
 */

IMPLEMENT_CLASS( exploration_machine_gui, wxFrame )

/*!
 * exploration_machine_gui event table definition
 */

BEGIN_EVENT_TABLE( exploration_machine_gui, wxFrame )

////@begin exploration_machine_gui event table entries
////@end exploration_machine_gui event table entries

END_EVENT_TABLE()

/*!
 * exploration_machine_gui constructors
 */

exploration_machine_gui::exploration_machine_gui()
{
    Init();
}

exploration_machine_gui::exploration_machine_gui( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * exploration_machine_gui creator
 */

bool exploration_machine_gui::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin exploration_machine_gui creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end exploration_machine_gui creation
    return true;
}

/*!
 * exploration_machine_gui destructor
 */

exploration_machine_gui::~exploration_machine_gui()
{
////@begin exploration_machine_gui destruction
////@end exploration_machine_gui destruction
}

/*!
 * Member initialisation 
 */

void exploration_machine_gui::Init()
{
////@begin exploration_machine_gui member initialisation
////@end exploration_machine_gui member initialisation
}

/*!
 * Control creation for exploration_machine_gui
 */

void exploration_machine_gui::CreateControls()
{    
////@begin exploration_machine_gui content construction
    exploration_machine_gui* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    tri_scene_canvas* itemGLCanvas3 = new tri_scene_canvas( itemFrame1, tri_scene_canvas::ID_GLCANVAS1, wxDefaultPosition, wxSize(600, 400), 0 );
    itemBoxSizer2->Add(itemGLCanvas3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end exploration_machine_gui content construction
}

/*!
 * Should we show tooltips?
 */

bool exploration_machine_gui::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap exploration_machine_gui::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin exploration_machine_gui bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end exploration_machine_gui bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon exploration_machine_gui::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin exploration_machine_gui icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end exploration_machine_gui icon retrieval
}
