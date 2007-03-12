/////////////////////////////////////////////////////////////////////////////
// Name:        urg_2d_plot_frame.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     10/03/2007 12:53:08
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _URG_2D_PLOT_FRAME_H_
#define _URG_2D_PLOT_FRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "urg_2d_plot_frame.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
////@end includes

#include <alcor/sense/urg_laser_t.hpp>
/*!
 * Forward declarations
 */

////@begin forward declarations
class urg_2d_canvas;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_URG_2D_PLOT_FRAME 10000
#define SYMBOL_URG_2D_PLOT_FRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_URG_2D_PLOT_FRAME_TITLE _("urg_2d_plot_frame")
#define SYMBOL_URG_2D_PLOT_FRAME_IDNAME ID_URG_2D_PLOT_FRAME
#define SYMBOL_URG_2D_PLOT_FRAME_SIZE wxSize(400, 300)
#define SYMBOL_URG_2D_PLOT_FRAME_POSITION wxDefaultPosition
////@end control identifiers

#define ID_TIMER 10005
/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * urg_2d_plot_frame class declaration
 */

class urg_2d_plot_frame: public wxFrame
{    
    DECLARE_CLASS( urg_2d_plot_frame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    urg_2d_plot_frame();
    urg_2d_plot_frame( wxWindow* parent, wxWindowID id = SYMBOL_URG_2D_PLOT_FRAME_IDNAME, const wxString& caption = SYMBOL_URG_2D_PLOT_FRAME_TITLE, const wxPoint& pos = SYMBOL_URG_2D_PLOT_FRAME_POSITION, const wxSize& size = SYMBOL_URG_2D_PLOT_FRAME_SIZE, long style = SYMBOL_URG_2D_PLOT_FRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_URG_2D_PLOT_FRAME_IDNAME, const wxString& caption = SYMBOL_URG_2D_PLOT_FRAME_TITLE, const wxPoint& pos = SYMBOL_URG_2D_PLOT_FRAME_POSITION, const wxSize& size = SYMBOL_URG_2D_PLOT_FRAME_SIZE, long style = SYMBOL_URG_2D_PLOT_FRAME_STYLE );

    /// Destructor
    ~urg_2d_plot_frame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

	void on_timer(wxTimerEvent&);

////@begin urg_2d_plot_frame event handler declarations

////@end urg_2d_plot_frame event handler declarations

////@begin urg_2d_plot_frame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end urg_2d_plot_frame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin urg_2d_plot_frame member variables
    urg_2d_canvas* m_urg_canvas;
////@end urg_2d_plot_frame member variables

	all::sense::urg_laser_t* m_urg;

	wxTimer* m_timer;

	
};

#endif
    // _URG_2D_PLOT_FRAME_H_
