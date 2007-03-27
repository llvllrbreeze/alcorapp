/////////////////////////////////////////////////////////////////////////////
// Name:        gaze_machine_gui.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     25/03/2007 19:43:29
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gaze_machine_gui.h"
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

#include "gaze_machine_gui.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( Gaze_machine_guiApp )
////@end implement app

/*!
 * Gaze_machine_guiApp type definition
 */

IMPLEMENT_CLASS( Gaze_machine_guiApp, wxApp )

/*!
 * Gaze_machine_guiApp event table definition
 */

BEGIN_EVENT_TABLE( Gaze_machine_guiApp, wxApp )

////@begin Gaze_machine_guiApp event table entries
////@end Gaze_machine_guiApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for Gaze_machine_guiApp
 */

Gaze_machine_guiApp::Gaze_machine_guiApp()
{
    Init();
}

/*!
 * Member initialisation 
 */

void Gaze_machine_guiApp::Init()
{
////@begin Gaze_machine_guiApp member initialisation
////@end Gaze_machine_guiApp member initialisation
}
/*!
 * Initialisation for Gaze_machine_guiApp
 */

bool Gaze_machine_guiApp::OnInit()
{    
////@begin Gaze_machine_guiApp initialisation
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
	gazemachine_mainframe* mainWindow = new gazemachine_mainframe;
	mainWindow->Create( NULL, gazemachine_mainframe::ID_GAZEMACHINE_MAINFRAME );
	mainWindow->Show(true);
////@end Gaze_machine_guiApp initialisation

    return true;
}

/*!
 * Cleanup for Gaze_machine_guiApp
 */
int Gaze_machine_guiApp::OnExit()
{    
////@begin Gaze_machine_guiApp cleanup
	return wxApp::OnExit();
////@end Gaze_machine_guiApp cleanup
}

