/////////////////////////////////////////////////////////////////////////////
// Name:        urg_2d_plot.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     10/03/2007 12:51:34
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "urg_2d_plot.h"
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

#include "urg_2d_plot.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( urg_2d_plot_app )
////@end implement app

/*!
 * urg_2d_plot_app type definition
 */

IMPLEMENT_CLASS( urg_2d_plot_app, wxApp )

/*!
 * urg_2d_plot_app event table definition
 */

BEGIN_EVENT_TABLE( urg_2d_plot_app, wxApp )

////@begin urg_2d_plot_app event table entries
////@end urg_2d_plot_app event table entries

END_EVENT_TABLE()

/*!
 * Constructor for urg_2d_plot_app
 */

urg_2d_plot_app::urg_2d_plot_app()
{
    Init();
}

/*!
 * Member initialisation 
 */

void urg_2d_plot_app::Init()
{
////@begin urg_2d_plot_app member initialisation
////@end urg_2d_plot_app member initialisation
}
/*!
 * Initialisation for urg_2d_plot_app
 */

bool urg_2d_plot_app::OnInit()
{    
////@begin urg_2d_plot_app initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	urg_2d_plot_frame* mainWindow = new urg_2d_plot_frame( NULL, ID_URG_2D_PLOT_FRAME );
	mainWindow->Show(true);
////@end urg_2d_plot_app initialisation

    return true;
}

/*!
 * Cleanup for urg_2d_plot_app
 */
int urg_2d_plot_app::OnExit()
{    
////@begin urg_2d_plot_app cleanup
	return wxApp::OnExit();
////@end urg_2d_plot_app cleanup
}

