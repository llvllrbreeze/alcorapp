/////////////////////////////////////////////////////////////////////////////
// Name:        shrimp_gui.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     22/02/2007 00:31:58
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "shrimp_gui.h"
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

#include "shrimp_gui.h"

////@begin XPM images

////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( shrimp_gui_app )
////@end implement app

/*!
 * shrimp_gui_app type definition
 */

IMPLEMENT_CLASS( shrimp_gui_app, wxApp )

/*!
 * shrimp_gui_app event table definition
 */

BEGIN_EVENT_TABLE( shrimp_gui_app, wxApp )

////@begin shrimp_gui_app event table entries
////@end shrimp_gui_app event table entries

END_EVENT_TABLE()

/*!
 * Constructor for shrimp_gui_app
 */

shrimp_gui_app::shrimp_gui_app()
{
    Init();
}

/*!
 * Member initialisation 
 */

void shrimp_gui_app::Init()
{
////@begin shrimp_gui_app member initialisation
////@end shrimp_gui_app member initialisation
}
/*!
 * Initialisation for shrimp_gui_app
 */

bool shrimp_gui_app::OnInit()
{    
////@begin shrimp_gui_app initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#define wxUSE_LIBJPEG 0

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
//#if wxUSE_LIBJPEG
//    wxImage::AddHandler(new wxJPEGHandler);
//#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
////@end shrimp_gui_app initialisation

	m_frame = new shrimp_main_frame(NULL);
    m_frame->Show( true );
    SetTopWindow( m_frame );

	return true;

}

/*!
 * Cleanup for shrimp_gui_app
 */
int shrimp_gui_app::OnExit()
{    
////@begin shrimp_gui_app cleanup
    return wxApp::OnExit();
////@end shrimp_gui_app cleanup
}

