/////////////////////////////////////////////////////////////////////////////
// Name:        bumblebee_gui.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     04/03/2007 04:18:17
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "bumblebee_gui.h"
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

#include "bumblebee_gui.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( bumblebee_gu_iApp )
////@end implement app

/*!
 * bumblebee_gu_iApp type definition
 */

IMPLEMENT_CLASS( bumblebee_gu_iApp, wxApp )

/*!
 * bumblebee_gu_iApp event table definition
 */

BEGIN_EVENT_TABLE( bumblebee_gu_iApp, wxApp )

////@begin bumblebee_gu_iApp event table entries
////@end bumblebee_gu_iApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for bumblebee_gu_iApp
 */

bumblebee_gu_iApp::bumblebee_gu_iApp()
{
    Init();
}

/*!
 * Member initialisation 
 */

void bumblebee_gu_iApp::Init()
{
////@begin bumblebee_gu_iApp member initialisation
////@end bumblebee_gu_iApp member initialisation
}
/*!
 * Initialisation for bumblebee_gu_iApp
 */

bool bumblebee_gu_iApp::OnInit()
{    
////@begin bumblebee_gu_iApp initialisation
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
	bumblebee_frame* mainWindow = new bumblebee_frame( NULL, ID_BUMBLEBEE_FRAME );
	mainWindow->Show(true);
////@end bumblebee_gu_iApp initialisation

    return true;
}

/*!
 * Cleanup for bumblebee_gu_iApp
 */
int bumblebee_gu_iApp::OnExit()
{    
////@begin bumblebee_gu_iApp cleanup
	return wxApp::OnExit();
////@end bumblebee_gu_iApp cleanup
}

