/////////////////////////////////////////////////////////////////////////////
// Name:        urg_2d_plot_frame.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     10/03/2007 12:53:08
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "urg_2d_plot_frame.h"
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
#include "urg_2d_canvas.hpp"
////@end includes

#include "urg_2d_plot_frame.h"

////@begin XPM images
////@end XPM images

/*!
 * urg_2d_plot_frame type definition
 */

IMPLEMENT_CLASS( urg_2d_plot_frame, wxFrame )

/*!
 * urg_2d_plot_frame event table definition
 */

BEGIN_EVENT_TABLE( urg_2d_plot_frame, wxFrame )

////@begin urg_2d_plot_frame event table entries
////@end urg_2d_plot_frame event table entries

	EVT_TIMER(ID_TIMER, urg_2d_plot_frame::on_timer)

END_EVENT_TABLE()

/*!
 * urg_2d_plot_frame constructors
 */

urg_2d_plot_frame::urg_2d_plot_frame()
{
    Init();
}

urg_2d_plot_frame::urg_2d_plot_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * urg_2d_plot_frame creator
 */

bool urg_2d_plot_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin urg_2d_plot_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end urg_2d_plot_frame creation
    return true;
}

/*!
 * urg_2d_plot_frame destructor
 */

urg_2d_plot_frame::~urg_2d_plot_frame()
{
////@begin urg_2d_plot_frame destruction
////@end urg_2d_plot_frame destruction
}

/*!
 * Member initialisation 
 */

void urg_2d_plot_frame::Init()
{
////@begin urg_2d_plot_frame member initialisation
////@end urg_2d_plot_frame member initialisation

	m_urg = new all::sense::urg_laser_t();
	
	if (!m_urg->connect()) {
		exit(1);
	}

	else {
		m_timer = new wxTimer(this, ID_TIMER);
		m_timer->Start(100);
	}
}
/*!
 * Control creation for urg_2d_plot_frame
 */

void urg_2d_plot_frame::CreateControls()
{    
////@begin urg_2d_plot_frame content construction
    urg_2d_plot_frame* itemFrame1 = this;

    m_urg_canvas = new urg_2d_canvas( itemFrame1, ID_GLCANVAS1, wxDefaultPosition, wxDefaultSize, 0 );

////@end urg_2d_plot_frame content constructio

	m_urg_canvas->reset_scan(m_urg->do_scan(all::math::angle(-110, all::math::deg_tag), all::math::angle(110, all::math::deg_tag), 1));

}

/*!
 * Should we show tooltips?
 */

bool urg_2d_plot_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap urg_2d_plot_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin urg_2d_plot_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end urg_2d_plot_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon urg_2d_plot_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin urg_2d_plot_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end urg_2d_plot_frame icon retrieval
}

void urg_2d_plot_frame::on_timer(wxTimerEvent& timer_event) {
	m_urg_canvas->update_scan(m_urg->do_scan(all::math::angle(-110, all::math::deg_tag), all::math::angle(110, all::math::deg_tag), 1));
}
