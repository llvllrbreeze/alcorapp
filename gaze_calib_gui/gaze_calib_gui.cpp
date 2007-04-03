/////////////////////////////////////////////////////////////////////////////
// Name:        gaze_calib_gui.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     02/04/2007 02:18:48
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gaze_calib_gui.h"
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

#include "gaze_calib_gui.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( Gaze_calib_gui_App )
////@end implement app

/*!
 * Gaze_calib_gui_App type definition
 */

IMPLEMENT_CLASS( Gaze_calib_gui_App, wxApp )

/*!
 * Gaze_calib_gui_App event table definition
 */

BEGIN_EVENT_TABLE( Gaze_calib_gui_App, wxApp )

////@begin Gaze_calib_gui_App event table entries
////@end Gaze_calib_gui_App event table entries

END_EVENT_TABLE()

/*!
 * Constructor for Gaze_calib_gui_App
 */

Gaze_calib_gui_App::Gaze_calib_gui_App()
{
    Init();
}

/*!
 * Member initialisation 
 */

void Gaze_calib_gui_App::Init()
{
////@begin Gaze_calib_gui_App member initialisation
////@end Gaze_calib_gui_App member initialisation
}
/*!
 * Initialisation for Gaze_calib_gui_App
 */

bool Gaze_calib_gui_App::OnInit()
{    
////@begin Gaze_calib_gui_App initialisation
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
	calib_frame* mainWindow = new calib_frame;
	mainWindow->Create( NULL, calib_frame::ID_CALIB_FRAME );
	mainWindow->Show(true);
////@end Gaze_calib_gui_App initialisation

    return true;
}

/*!
 * Cleanup for Gaze_calib_gui_App
 */
int Gaze_calib_gui_App::OnExit()
{    
////@begin Gaze_calib_gui_App cleanup
	return wxApp::OnExit();
////@end Gaze_calib_gui_App cleanup
}

