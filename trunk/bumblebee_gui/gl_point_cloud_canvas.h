/////////////////////////////////////////////////////////////////////////////
// Name:        gl_point_cloud_canvas.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     09/03/2007 01:01:41
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _GL_POINT_CLOUD_CANVAS_H_
#define _GL_POINT_CLOUD_CANVAS_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "gl_point_cloud_canvas.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/glcanvas.h"
////@end includes

#include "trackball.h"
#include <fstream>
#include <boost/function.hpp>
#include <boost/timer.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include "alcor.extern/CImg/CImg.h"
namespace cimglib=cimg_library;

#include "alcor/sense/bumblebee_driver_t.h"
#include "alcor/core/core.h"
#include "alcor/core/stream_server_t.hpp"
#include "opengl_source_t.hpp"

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
#define ID_POINTCLOUD_CANVAS 10001
#define SYMBOL_POINT_CLOUD_CANVAS_STYLE wxNO_BORDER
#define SYMBOL_POINT_CLOUD_CANVAS_IDNAME ID_POINTCLOUD_CANVAS
#define SYMBOL_POINT_CLOUD_CANVAS_SIZE wxSize(800, 600)
#define SYMBOL_POINT_CLOUD_CANVAS_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * point_cloud_canvas class declaration
 */

class point_cloud_canvas: public wxGLCanvas
{    
    DECLARE_CLASS( point_cloud_canvas )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    point_cloud_canvas( wxWindow* parent, wxWindowID id = SYMBOL_POINT_CLOUD_CANVAS_IDNAME, const wxPoint& pos = SYMBOL_POINT_CLOUD_CANVAS_POSITION, const wxSize& size = SYMBOL_POINT_CLOUD_CANVAS_SIZE, long style = SYMBOL_POINT_CLOUD_CANVAS_STYLE, const wxString& name = wxT("GLCanvas"), int* attribs = NULL );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_POINT_CLOUD_CANVAS_IDNAME, const wxPoint& pos = SYMBOL_POINT_CLOUD_CANVAS_POSITION, const wxSize& size = SYMBOL_POINT_CLOUD_CANVAS_SIZE, long style = SYMBOL_POINT_CLOUD_CANVAS_STYLE, const wxString& name = wxT("GLCanvas"), int* attribs = NULL );

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    /// InitGL
    void initGL();

    ///
    void draw_cimg(GLenum ebuf);

    ///
    enum 
    {
      ID_TIMER_EVENT = 7117182
    };
    ///on_timer
	  void on_timer(wxTimerEvent&);


////@begin point_cloud_canvas event handler declarations

    /// wxEVT_SIZE event handler for ID_POINTCLOUD_CANVAS
    void OnSize( wxSizeEvent& event );

    /// wxEVT_PAINT event handler for ID_POINTCLOUD_CANVAS
    void OnPaint( wxPaintEvent& event );

    /// All mouse events event handler for ID_POINTCLOUD_CANVAS
    void OnMouse( wxMouseEvent& event );

////@end point_cloud_canvas event handler declarations

////@begin point_cloud_canvas member function declarations

    bool GetInitiialized() const { return m_initialized ; }
    void SetInitiialized(bool value) { m_initialized = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end point_cloud_canvas member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin point_cloud_canvas member variables
private:
    bool m_initialized;
    wxTimer* m_timer;
////@end point_cloud_canvas member variables

    /// reset_projection_mode
    void  reset_projection_mode();
    float beginx, beginy;       // position of mouse
    float quat[4];              // orientation of object
    float zoom;
    float fov;
    float xcam;
    float ycam;
    float zcam;
    std::ofstream logfile;
    boost::timer  extimer;
    boost::minstd_rand generator;

    all::sense::bumblebee_driver_ptr_t bee;
    all::core::single_sarr depthmap;
    all::core::uint8_sarr  rgbmap;

  //VIEW
  boost::shared_ptr<cimglib::CImgDisplay>          rgb_win;
  boost::shared_ptr<cimglib::CImg<all::core::uint8_t> > rgb_cimg;
  all::core::uint8_sarr   framebuffer;
  all::core::uint8_sarr   myimage;

  //Server
  //all::core::opengl_source_t m_source;
  boost::shared_ptr <all::core::stream_server_t > server_ptr;
  boost::shared_ptr <all::core::opengl_source_t > source_ptr; 
};

#endif
    // _GL_POINT_CLOUD_CANVAS_H_
