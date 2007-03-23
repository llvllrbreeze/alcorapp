/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_client.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:48:46
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "exploration_client.h"
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

#include "exploration_client.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( Exploration_clientApp )
////@end implement app

/*!
 * Exploration_clientApp type definition
 */

IMPLEMENT_CLASS( Exploration_clientApp, wxApp )

/*!
 * Exploration_clientApp event table definition
 */

BEGIN_EVENT_TABLE( Exploration_clientApp, wxApp )

////@begin Exploration_clientApp event table entries
////@end Exploration_clientApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for Exploration_clientApp
 */

Exploration_clientApp::Exploration_clientApp()
{
    Init();
}

/*!
 * Member initialisation 
 */

void Exploration_clientApp::Init()
{
////@begin Exploration_clientApp member initialisation
////@end Exploration_clientApp member initialisation
}
/*!
 * Initialisation for Exploration_clientApp
 */

bool Exploration_clientApp::OnInit()
{    
////@begin Exploration_clientApp initialisation
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
	exploration_gui* mainWindow = new exploration_gui;
	mainWindow->Create( NULL, exploration_gui::ID_EXPLORATION_GUI );
	mainWindow->Show(true);
////@end Exploration_clientApp initialisation

    return true;
}

/*!
 * Cleanup for Exploration_clientApp
 */
int Exploration_clientApp::OnExit()
{    
////@begin Exploration_clientApp cleanup
	return wxApp::OnExit();
////@end Exploration_clientApp cleanup
}

