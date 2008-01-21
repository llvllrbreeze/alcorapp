/////////////////////////////////////////////////////////////////////////////
// Name:        urg_2d_canvas.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     10/03/2007 12:54:22
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "urg_2d_canvas.hpp"
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

#include "urg_2d_canvas.hpp"
#include <GL/glu.h>
#include <vector>

#include <alcor/core/core.h>

////@begin XPM images
////@end XPM images

/*!
 * urg_2d_canvas type definition
 */

IMPLEMENT_CLASS( urg_2d_canvas, wxGLCanvas )

/*!
 * urg_2d_canvas event table definition
 */

BEGIN_EVENT_TABLE( urg_2d_canvas, wxGLCanvas )

////@begin urg_2d_canvas event table entries
    EVT_SIZE( urg_2d_canvas::OnSize )
    EVT_PAINT( urg_2d_canvas::OnPaint )

////@end urg_2d_canvas event table entries

END_EVENT_TABLE()

/*!
 * urg_2d_canvas constructors
 */

urg_2d_canvas::urg_2d_canvas( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* attribs ):
  wxGLCanvas(parent, id, pos, size, style, name, attribs)
{
    Init();
    Create(parent, id, pos, size, style, name, attribs);
}

/*!
 * urg_2d_canvas creator
 */

bool urg_2d_canvas::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* attribs )
{
////@begin urg_2d_canvas creation
    CreateControls();
////@end urg_2d_canvas creation

    
	return true;
}

/*!
 * urg_2d_canvas destructor
 */

urg_2d_canvas::~urg_2d_canvas()
{
////@begin urg_2d_canvas destruction
////@end urg_2d_canvas destruction
}

/*!
 * Member initialisation 
 */

void urg_2d_canvas::Init()
{
////@begin urg_2d_canvas member initialisation
////@end urg_2d_canvas member initialisation

	m_init = false;
	
	
	//set trigonometric table for circle drawing
	min_angle = all::sense::urg_scan_data_t::step2angle(20).rad();
	max_angle = all::sense::urg_scan_data_t::step2angle(740).rad();

	m_curr_scan.reset(new all::sense::urg_scan_data_t());

	m_curr_scan->start_step = 80;
	m_curr_scan->end_step = 690;

	m_curr_scan->cc = 1;

	for (int i=0; i<(m_curr_scan->end_step - m_curr_scan->start_step); i++) {
		m_curr_scan->scan_points.push_back(3100);
	}

	reset_scan(m_curr_scan);


	init_gl();

	//reset_trig_table();
	//reset_viewport();

}

/*!
 * Control creation for urg_2d_canvas
 */

void urg_2d_canvas::CreateControls()
{    
////@begin urg_2d_canvas content construction
    urg_2d_canvas* itemGLCanvas1 = this;

////@end urg_2d_canvas content construction
}

void urg_2d_canvas::init_gl() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glPointSize(2.0f);
}

void urg_2d_canvas::reset_scan(all::sense::urg_scan_data_ptr scan) {
	m_urg_th_cos_table.reset(new all::core::single_t[scan->scan_points.size()]);
	m_urg_th_sin_table.reset(new all::core::single_t[scan->scan_points.size()]);

	for (int i=0; i<scan->scan_points.size(); i++) {
		double rad_angle = scan->get_angle(i).rad();
		m_urg_th_cos_table[i] = cos(rad_angle);
		m_urg_th_sin_table[i] = sin(rad_angle);
	}

	min_angle = all::sense::urg_scan_data_t::step2angle(scan->start_step).rad();
	max_angle = all::sense::urg_scan_data_t::step2angle(scan->end_step).rad();

	reset_trig_table();
	reset_viewport();

}

void urg_2d_canvas::reset_trig_table() {
	m_trig_table_size = (max_angle - min_angle) * 10 + 1;
	
	m_sin_table.reset(new all::core::single_t[m_trig_table_size]);
	m_cos_table.reset(new all::core::single_t[m_trig_table_size]);

	float curr_angle = min_angle;
	for (int i =0; i<m_trig_table_size-1; i++) {
		m_cos_table[i] = cos(curr_angle);
		m_sin_table[i] = sin(curr_angle);
		curr_angle+=0.1;
	}
	m_cos_table[m_trig_table_size-1] = cos (max_angle);
	m_sin_table[m_trig_table_size-1] = sin(max_angle);
}


void urg_2d_canvas::update_scan(all::sense::urg_scan_data_ptr scan) {
	m_curr_scan = scan;
	Refresh(false);
}

void urg_2d_canvas::reset_viewport() {
    
	GetClientSize(&m_width, &m_height);
    SetCurrent();
    glViewport(0, 0, (GLint) m_width, (GLint) m_height);
  
	// Orthographic pixel coordinate space 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, m_width, 0, m_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	
	//setting mapping from urg coordinate to viewport
	
	m_center[0] = float(m_width) / 2;
	
	
	if (abs(min_angle) > max_angle) {

		if (min_angle < -3.14 / 2) 
			m_center[1] = (float(m_height)*abs(m_cos_table[0]))/(1-(m_cos_table[0]));
		else
			m_center[1] = 20;
	}else {
		if (max_angle > -3.14 / 2) 
			m_center[1] = (float(m_height)*abs(m_cos_table[m_trig_table_size-1]))/(1-(m_cos_table[m_trig_table_size-1]));
		else
			m_center[1] = 20;
	}


	if ((m_width - m_center[0]) > (m_height - m_center[1]))
		m_urg_to_canvas_map.set_range(0, 5000, 0, (0.95)*m_height-m_center[1]);
	else
		m_urg_to_canvas_map.set_range(0, 5000, 0, (0.95)*m_width-m_center[0]);

	glTranslatef( m_center[0], m_center[1], 0);

	
	m_radius[0] = m_urg_to_canvas_map.x_to_y(1000);

	for (int i = 1; i<m_radius_size; i++) {
		m_radius[i] = (i+1) * m_radius[0];
	}

}

/*!
 * Should we show tooltips?
 */

bool urg_2d_canvas::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap urg_2d_canvas::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin urg_2d_canvas bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end urg_2d_canvas bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon urg_2d_canvas::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin urg_2d_canvas icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end urg_2d_canvas icon retrieval
}
/*!
 * wxEVT_PAINT event handler for ID_GLCANVAS1
 */

void urg_2d_canvas::OnPaint( wxPaintEvent& event )
{
////@begin wxEVT_PAINT event handler for ID_GLCANVAS1 in urg_2d_canvas.
    // Before editing this code, remove the block markers.
    wxPaintDC dc(this);
////@end wxEVT_PAINT event handler for ID_GLCANVAS1 in urg_2d_canvas. 
	SetCurrent();

	draw_grid();

	draw_points();

	// Flush
    glFlush();

    // Swap
    SwapBuffers();
}


/*!
 * wxEVT_SIZE event handler for ID_GLCANVAS1
 */

void urg_2d_canvas::OnSize( wxSizeEvent& event )
{
	reset_viewport();
}


void urg_2d_canvas::draw_grid() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT);

	glColor4f(1.0f, 0.0f, 0.1f, 1.0f);
    

	float axis_length = m_radius[m_radius_size-1] + 10;
    
	glBegin(GL_LINES);   
		glVertex2f(0, 0);
		glVertex2f(0, axis_length);
    glEnd();

	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(axis_length * m_sin_table[0], axis_length * m_cos_table[0]);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(axis_length * m_sin_table[m_trig_table_size-1], axis_length * m_cos_table[m_trig_table_size-1]);
	glEnd();


	//circles
	for (int i = 0; i<m_radius_size; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j=0; j<m_trig_table_size; j++) {
			glVertex2f (m_radius[i]*m_sin_table[j], m_radius[i]*m_cos_table[j]);
		}
		glEnd();
	}

	glBegin(GL_TRIANGLES);
		glColor4f(0.5f, 0.4f, 0.1f, 1.0f);
		glVertex2f(-10,-20);
		glVertex2f(10,-20);
		glVertex2f(0,0);
	glEnd();
}

void urg_2d_canvas::draw_points() {

	if (m_curr_scan) {
		float curr_point[2];
		float next_point[2];

		std::size_t n_points = m_curr_scan->scan_points.size() - 1;
		for (int i=0; i<n_points; i++) {
			curr_point[0] = m_urg_to_canvas_map.x_to_y(m_curr_scan->scan_points[i]) * m_urg_th_sin_table[i];
			curr_point[1] = m_urg_to_canvas_map.x_to_y(m_curr_scan->scan_points[i]) * m_urg_th_cos_table[i];
		
			next_point[0] = m_urg_to_canvas_map.x_to_y(m_curr_scan->scan_points[i+1]) * m_urg_th_sin_table[i+1];
			next_point[1] = m_urg_to_canvas_map.x_to_y(m_curr_scan->scan_points[i+1]) * m_urg_th_cos_table[i+1];

			glBegin(GL_TRIANGLES);
				glColor4f(0.0f,0.0f,1.0f,0.3f);
				glVertex2f(0, 0);
				glVertex2f(curr_point[0], curr_point[1]);
				glVertex2f(next_point[0], next_point[1]);
			glEnd();

			glBegin(GL_POINTS);
				glColor4f(0.0f, 0.5f, 0.5f, 1.0f);
				glVertex2f(curr_point[0], curr_point[1]);
			glEnd();
		}
	}
}
