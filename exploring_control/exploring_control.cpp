/////////////////////////////////////////////////////////////////////////////
// Name:        exploring_control.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     27/03/2007 03:31:00
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "exploring_control.h"
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

#include "exploring_control.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( Exploring_controlApp )
////@end implement app

/*!
 * Exploring_controlApp type definition
 */

IMPLEMENT_CLASS( Exploring_controlApp, wxApp )

/*!
 * Exploring_controlApp event table definition
 */

BEGIN_EVENT_TABLE( Exploring_controlApp, wxApp )

////@begin Exploring_controlApp event table entries
////@end Exploring_controlApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for Exploring_controlApp
 */

Exploring_controlApp::Exploring_controlApp()
{
    Init();
}

/*!
 * Member initialisation 
 */

void Exploring_controlApp::Init()
{
////@begin Exploring_controlApp member initialisation
////@end Exploring_controlApp member initialisation
}
/*!
 * Initialisation for Exploring_controlApp
 */

bool Exploring_controlApp::OnInit()
{    
////@begin Exploring_controlApp initialisation
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
	exploring_control_mainframe* mainWindow = new exploring_control_mainframe;
	mainWindow->Create( NULL, exploring_control_mainframe::ID_EXPLORING_CONTROL_MAINFRAME );
	mainWindow->Show(true);
////@end Exploring_controlApp initialisation

    return true;
}

/*!
 * Cleanup for Exploring_controlApp
 */
int Exploring_controlApp::OnExit()
{    
////@begin Exploring_controlApp cleanup
	return wxApp::OnExit();
////@end Exploring_controlApp cleanup
}

