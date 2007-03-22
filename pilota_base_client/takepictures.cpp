/////////////////////////////////////////////////////////////////////////////
// Name:        takepictures.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     22/03/2007 09:58:18
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "takepictures.h"
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

#include "takepictures.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( TakepicturesApp )
////@end implement app

/*!
 * TakepicturesApp type definition
 */

IMPLEMENT_CLASS( TakepicturesApp, wxApp )

/*!
 * TakepicturesApp event table definition
 */

BEGIN_EVENT_TABLE( TakepicturesApp, wxApp )

////@begin TakepicturesApp event table entries
////@end TakepicturesApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for TakepicturesApp
 */

TakepicturesApp::TakepicturesApp()
{
    Init();
}

/*!
 * Member initialisation 
 */

void TakepicturesApp::Init()
{
////@begin TakepicturesApp member initialisation
////@end TakepicturesApp member initialisation
}
/*!
 * Initialisation for TakepicturesApp
 */

bool TakepicturesApp::OnInit()
{    
////@begin TakepicturesApp initialisation
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
	take_pictures_frame* mainWindow = new take_pictures_frame( NULL, ID_TAKE_PICTURES_FRAME );
	mainWindow->Show(true);
////@end TakepicturesApp initialisation

    return true;
}

/*!
 * Cleanup for TakepicturesApp
 */
int TakepicturesApp::OnExit()
{    
////@begin TakepicturesApp cleanup
	return wxApp::OnExit();
////@end TakepicturesApp cleanup
}

