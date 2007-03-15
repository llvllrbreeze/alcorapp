/////////////////////////////////////////////////////////////////////////////
// Name:        bumblebee_frame.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     09/03/2007 01:01:27
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _BUMBLEBEE_FRAME_H_
#define _BUMBLEBEE_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "bumblebee_frame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
////@end includes

#include <boost/function.hpp>
#include <boost/bind.hpp>
//
//#include "alcor/sense/bumblebee_driver_t.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class point_cloud_canvas;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_BUMBLEBEE_FRAME 10000
#define SYMBOL_BUMBLEBEE_FRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_BUMBLEBEE_FRAME_TITLE _("bumblebee_frame")
#define SYMBOL_BUMBLEBEE_FRAME_IDNAME ID_BUMBLEBEE_FRAME
#define SYMBOL_BUMBLEBEE_FRAME_SIZE wxSize(400, 300)
#define SYMBOL_BUMBLEBEE_FRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * bumblebee_frame class declaration
 */

class bumblebee_frame: public wxFrame
{    
    DECLARE_CLASS( bumblebee_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    bumblebee_frame( );
    bumblebee_frame( wxWindow* parent, wxWindowID id = SYMBOL_BUMBLEBEE_FRAME_IDNAME, const wxString& caption = SYMBOL_BUMBLEBEE_FRAME_TITLE, const wxPoint& pos = SYMBOL_BUMBLEBEE_FRAME_POSITION, const wxSize& size = SYMBOL_BUMBLEBEE_FRAME_SIZE, long style = SYMBOL_BUMBLEBEE_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_BUMBLEBEE_FRAME_IDNAME, const wxString& caption = SYMBOL_BUMBLEBEE_FRAME_TITLE, const wxPoint& pos = SYMBOL_BUMBLEBEE_FRAME_POSITION, const wxSize& size = SYMBOL_BUMBLEBEE_FRAME_SIZE, long style = SYMBOL_BUMBLEBEE_FRAME_STYLE );

    /// Initialises member variables
    void Init();


    /// Creates the controls and sizers
    void CreateControls();

////@begin bumblebee_frame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_BUMBLEBEE_FRAME
    void OnCloseWindow( wxCloseEvent& event );

////@end bumblebee_frame event handler declarations

////@begin bumblebee_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end bumblebee_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin bumblebee_frame member variables
    point_cloud_canvas* point_cloud_view;
////@end bumblebee_frame member variables

    //all::sense::bumblebee_driver_ptr_t bee;
};

#endif
    // _BUMBLEBEE_FRAME_H_
