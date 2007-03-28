/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_machine.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:33:06
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "exploration_machine.h"
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

#include "exploration_machine.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( Exploration_machineApp )
////@end implement app

/*!
 * Exploration_machineApp type definition
 */

IMPLEMENT_CLASS( Exploration_machineApp, wxApp )

/*!
 * Exploration_machineApp event table definition
 */

BEGIN_EVENT_TABLE( Exploration_machineApp, wxApp )

////@begin Exploration_machineApp event table entries
////@end Exploration_machineApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for Exploration_machineApp
 */

Exploration_machineApp::Exploration_machineApp()
{
    Init();
}

/*!
 * Member initialisation 
 */

void Exploration_machineApp::Init()
{
////@begin Exploration_machineApp member initialisation
////@end Exploration_machineApp member initialisation
}
/*!
 * Initialisation for Exploration_machineApp
 */

bool Exploration_machineApp::OnInit()
{    
////@begin Exploration_machineApp initialisation
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
	exploration_machine_gui* mainWindow = new exploration_machine_gui( NULL, ID_EXPLORATION_MACHINE_GUI );
	mainWindow->Show(true);
////@end Exploration_machineApp initialisation

    return true;
}

/*!
 * Cleanup for Exploration_machineApp
 */
int Exploration_machineApp::OnExit()
{    
////@begin Exploration_machineApp cleanup
	return wxApp::OnExit();
////@end Exploration_machineApp cleanup
}

