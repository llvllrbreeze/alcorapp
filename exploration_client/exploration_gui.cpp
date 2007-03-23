/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_gui.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:49:28
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "exploration_gui.h"
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

#include "exploration_gui.h"

////@begin XPM images
////@end XPM images

/*!
 * exploration_gui type definition
 */

IMPLEMENT_CLASS( exploration_gui, wxFrame )

/*!
 * exploration_gui event table definition
 */

BEGIN_EVENT_TABLE( exploration_gui, wxFrame )

////@begin exploration_gui event table entries
////@end exploration_gui event table entries

END_EVENT_TABLE()

/*!
 * exploration_gui constructors
 */

exploration_gui::exploration_gui()
{
    Init();
}

exploration_gui::exploration_gui( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * exploration_gui creator
 */

bool exploration_gui::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin exploration_gui creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end exploration_gui creation
    return true;
}

/*!
 * exploration_gui destructor
 */

exploration_gui::~exploration_gui()
{
////@begin exploration_gui destruction
////@end exploration_gui destruction
}

/*!
 * Member initialisation 
 */

void exploration_gui::Init()
{
////@begin exploration_gui member initialisation
    m_rgb_panel = NULL;
    m_3d_panel = NULL;
////@end exploration_gui member initialisation
}

/*!
 * Control creation for exploration_gui
 */

void exploration_gui::CreateControls()
{    
////@begin exploration_gui content construction
    exploration_gui* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Static"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_rgb_panel = new wx_img_stream_panel_t( "config/xpr_stream_client.ini" );
    m_rgb_panel->Create( itemFrame1, ID_RGB_PANEL, wxDefaultPosition, wxSize(320, 240), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemStaticBoxSizer3->Add(m_rgb_panel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_3d_panel = new wx_img_stream_panel_t( "config/3d_stream_client.ini" );
    m_3d_panel->Create( itemFrame1, ID_3D_PANEL, wxDefaultPosition, wxSize(640, 480), wxNO_BORDER|wxTAB_TRAVERSAL );
    itemStaticBoxSizer3->Add(m_3d_panel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton;
    itemButton7->Create( itemFrame1, ID_BUTTON1, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton8 = new wxButton;
    itemButton8->Create( itemFrame1, ID_BUTTON2, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton;
    itemButton9->Create( itemFrame1, ID_BUTTON3, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end exploration_gui content construction
}

/*!
 * Should we show tooltips?
 */

bool exploration_gui::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap exploration_gui::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin exploration_gui bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end exploration_gui bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon exploration_gui::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin exploration_gui icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end exploration_gui icon retrieval
}
