/////////////////////////////////////////////////////////////////////////////
// Name:        urg_2d_canvas.hpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     10/03/2007 12:54:22
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _URG_2D_CANVAS_H_
#define _URG_2D_CANVAS_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "urg_2d_canvas.hpp"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/glcanvas.h"
////@end includes

#include <alcor/core/interval_map_t.hpp>
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
#define ID_GLCANVAS1 10001
#define SYMBOL_URG_2D_CANVAS_STYLE 0
#define SYMBOL_URG_2D_CANVAS_IDNAME ID_GLCANVAS1
#define SYMBOL_URG_2D_CANVAS_SIZE wxDefaultSize
#define SYMBOL_URG_2D_CANVAS_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * urg_2d_canvas class declaration
 */

class urg_2d_canvas: public wxGLCanvas
{    
    DECLARE_CLASS( urg_2d_canvas )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    urg_2d_canvas( wxWindow* parent, wxWindowID id = SYMBOL_URG_2D_CANVAS_IDNAME, const wxPoint& pos = SYMBOL_URG_2D_CANVAS_POSITION, const wxSize& size = SYMBOL_URG_2D_CANVAS_SIZE, long style = SYMBOL_URG_2D_CANVAS_STYLE, const wxString& name = wxT("GLCanvas"), int* attribs = NULL );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_URG_2D_CANVAS_IDNAME, const wxPoint& pos = SYMBOL_URG_2D_CANVAS_POSITION, const wxSize& size = SYMBOL_URG_2D_CANVAS_SIZE, long style = SYMBOL_URG_2D_CANVAS_STYLE, const wxString& name = wxT("GLCanvas"), int* attribs = NULL );

    /// Destructor
    ~urg_2d_canvas();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

	void init_gl();

	void reset_viewport();

	void reset_scan(all::sense::urg_scan_data_ptr);

	void reset_trig_table();

	void update_scan(all::sense::urg_scan_data_ptr);

////@begin urg_2d_canvas event handler declarations

    /// wxEVT_SIZE event handler for ID_GLCANVAS1
    void OnSize( wxSizeEvent& event );

    /// wxEVT_PAINT event handler for ID_GLCANVAS1
    void OnPaint( wxPaintEvent& event );

////@end urg_2d_canvas event handler declarations

////@begin urg_2d_canvas member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end urg_2d_canvas member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin urg_2d_canvas member variables
////@end urg_2d_canvas member variables

	void draw_grid();

	void draw_points();

private:

	bool m_init;

	all::core::interval_map_t m_urg_to_canvas_map;
	
	all::core::single_sarr m_urg_th_cos_table;
	all::core::single_sarr m_urg_th_sin_table;
	

	all::core::single_t min_angle;
	all::core::single_t max_angle;

	all::core::single_sarr m_cos_table;
	all::core::single_sarr m_sin_table;
	int m_trig_table_size;

	static const int m_radius_size = 5;
	int m_radius[m_radius_size];

	all::sense::urg_scan_data_ptr m_curr_scan;

	int m_width;
	int m_height;

	float m_center[2];

};

#endif
    // _URG_2D_CANVAS_H_
