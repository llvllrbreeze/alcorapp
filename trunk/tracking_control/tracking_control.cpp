/////////////////////////////////////////////////////////////////////////////
// Name:        tracking_control.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     05/03/2007 23:15:00
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "tracking_control.h"
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

#include "tracking_control.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( tracking_control_app )
////@end implement app

/*!
 * tracking_control_app type definition
 */

IMPLEMENT_CLASS( tracking_control_app, wxApp )

/*!
 * tracking_control_app event table definition
 */

BEGIN_EVENT_TABLE( tracking_control_app, wxApp )

////@begin tracking_control_app event table entries
////@end tracking_control_app event table entries

END_EVENT_TABLE()

/*!
 * Constructor for tracking_control_app
 */

tracking_control_app::tracking_control_app()
{
    Init();
}

/*!
 * Member initialisation 
 */

void tracking_control_app::Init()
{
////@begin tracking_control_app member initialisation
////@end tracking_control_app member initialisation
}
/*!
 * Initialisation for tracking_control_app
 */

bool tracking_control_app::OnInit()
{    
////@begin tracking_control_app initialisation
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
	tracking_control_frame* mainWindow = new tracking_control_frame( NULL, ID_TRACKING_CONTROL_FRAME );
	mainWindow->Show(true);
////@end tracking_control_app initialisation

    return true;
}

/*!
 * Cleanup for tracking_control_app
 */
int tracking_control_app::OnExit()
{    
////@begin tracking_control_app cleanup
	return wxApp::OnExit();
////@end tracking_control_app cleanup
}

