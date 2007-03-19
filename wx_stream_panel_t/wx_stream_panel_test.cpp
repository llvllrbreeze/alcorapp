/////////////////////////////////////////////////////////////////////////////
// Name:        wx_stream_panel_test.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     16/03/2007 15:10:32
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wx_stream_panel_test.h"
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

#include "wx_stream_panel_test.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( wx_stream_panel_test_app )
////@end implement app

/*!
 * wx_stream_panel_test_app type definition
 */

IMPLEMENT_CLASS( wx_stream_panel_test_app, wxApp )

/*!
 * wx_stream_panel_test_app event table definition
 */

BEGIN_EVENT_TABLE( wx_stream_panel_test_app, wxApp )

////@begin wx_stream_panel_test_app event table entries
////@end wx_stream_panel_test_app event table entries

END_EVENT_TABLE()

/*!
 * Constructor for wx_stream_panel_test_app
 */

wx_stream_panel_test_app::wx_stream_panel_test_app()
{
    Init();
}

/*!
 * Member initialisation 
 */

void wx_stream_panel_test_app::Init()
{
////@begin wx_stream_panel_test_app member initialisation
////@end wx_stream_panel_test_app member initialisation
}
/*!
 * Initialisation for wx_stream_panel_test_app
 */

bool wx_stream_panel_test_app::OnInit()
{    
////@begin wx_stream_panel_test_app initialisation
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
	wx_test_frame* mainWindow = new wx_test_frame;
	mainWindow->Create( NULL, ID_WX_TEST_FRAME );
	mainWindow->Show(true);
////@end wx_stream_panel_test_app initialisation

    return true;
}

/*!
 * Cleanup for wx_stream_panel_test_app
 */
int wx_stream_panel_test_app::OnExit()
{    
////@begin wx_stream_panel_test_app cleanup
	return wxApp::OnExit();
////@end wx_stream_panel_test_app cleanup
}

