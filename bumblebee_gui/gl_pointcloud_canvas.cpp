/////////////////////////////////////////////////////////////////////////////
// Name:        gl_pointcloud_canvas.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     09/03/2007 01:01:41
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gl_point_cloud_canvas.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <GL/glu.h>



#include <ctime> 


////@begin includes
////@end includes

#include "gl_point_cloud_canvas.h"

////@begin XPM images
////@end XPM images

/*!
 * point_cloud_canvas type definition
 */

IMPLEMENT_CLASS( point_cloud_canvas, wxGLCanvas )

/*!
 * point_cloud_canvas event table definition
 */

BEGIN_EVENT_TABLE( point_cloud_canvas, wxGLCanvas )

////@begin point_cloud_canvas event table entries
    EVT_SIZE( point_cloud_canvas::OnSize )
    EVT_PAINT( point_cloud_canvas::OnPaint )
    EVT_MOUSE_EVENTS( point_cloud_canvas::OnMouse )

////@end point_cloud_canvas event table entries
    EVT_TIMER(point_cloud_canvas::ID_TIMER_EVENT, point_cloud_canvas::on_timer)
END_EVENT_TABLE()

/*!
 * point_cloud_canvas constructors
 */

point_cloud_canvas::point_cloud_canvas( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* attribs ):
  wxGLCanvas(parent, id, pos, size, style, name, attribs)
{
    Init();
    Create(parent, id, pos, size, style, name, attribs);
}

/*!
 * point_cloud_canvas creator
 */

bool point_cloud_canvas::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* attribs )
{
////@begin point_cloud_canvas creation
    CreateControls();
////@end point_cloud_canvas creation
    return true;
}

/*!
 * Member initialisation 
 */

void point_cloud_canvas::Init()
{
////@begin point_cloud_canvas member initialisation
    m_initialized = false;
    m_timer = 0;
////@end point_cloud_canvas member initialisation

    //trackball
    beginx = 0.0f;
    beginy = 0.0f;
    zoom   = 80.0f;
    fov    = 65.f;
    xcam = 0.0f;
    ycam = 0.0f;
    zcam = -20.0f;
    trackball(quat, 0.0f, 0.0f, 0.0f, 0.0f);

    m_timer = new wxTimer(this, ID_TIMER_EVENT);
    m_timer->Start(100);
    generator.seed(static_cast<unsigned int>(std::time(0)));
    logfile.open("gl_log.txt", std::ios::out);
}
/*!
 * Control creation for point_cloud_canvas
 */

void point_cloud_canvas::CreateControls()
{    
////@begin point_cloud_canvas content construction
    point_cloud_canvas* itemGLCanvas1 = this;
////@end point_cloud_canvas content construction
}

/*!
 * wxEVT_SIZE event handler for ID_POINTCLOUD_CANVAS
 */

void point_cloud_canvas::OnSize( wxSizeEvent& event )
{
    // this is also necessary to update the context on some platforms
    //wxGLCanvas::OnSize(event);
    // Reset the OpenGL view aspect
    reset_projection_mode();
}

/*!
 * wxEVT_PAINT event handler for ID_POINTCLOUD_CANVAS
 */

void point_cloud_canvas::OnPaint( wxPaintEvent& event )
{
  //boost::timer timer;

////@begin wxEVT_PAINT event handler for ID_POINTCLOUD_CANVAS in point_cloud_canvas.
    // Before editing this code, remove the block markers.
    wxPaintDC dc(this);
////@end wxEVT_PAINT event handler for ID_POINTCLOUD_CANVAS in point_cloud_canvas. 
    SetCurrent();
    //logfile << "SETCURRENT  elapsed: " <<  timer.elapsed() << std::endl;
    // Initialize OpenGL
    if (!m_initialized)
    {
        initGL();
        reset_projection_mode();
        m_initialized = true;
    }
    
    //Generate Random funcitons

    boost::uniform_real<> uni_dist_color(0,1);
    boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > unic(generator, uni_dist_color);

    boost::uniform_real<> uni_dist_coords(-10.0,10.0);
    boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > unip(generator, uni_dist_coords);

    //logfile << "RANDOM GEN  elapsed: " <<  timer.elapsed() << std::endl;

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Transformations
    glLoadIdentity();

    glTranslatef( 0.0f, 0.0f, zcam);

    GLfloat m[4][4];
    build_rotmatrix( m, quat );
    glMultMatrixf( &m[0][0] );

    ///X
    glBegin(GL_LINES);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glEnd();

    ///Y
    glBegin(GL_LINES);
    glColor3f(1.0f,0.0f,0.1f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glEnd();

    ///Z
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

    glBegin(GL_POINTS);   
    for(int i = 0; i < 75000 ; ++i)
    {
      glColor3f(unic(), unic(), unic());
      glVertex3f(unip(), unip(), unip());
    }
    glEnd();
    //logfile << "GL_POINTS  elapsed: " <<  timer.elapsed() << std::endl;

    // Flush
    //glFlush();

    // Swap
    SwapBuffers();
    //logfile << "SWAP  elapsed: " <<  timer.elapsed() << std::endl;

    //logfile << "TOTAL: " <<  extimer.elapsed()<< std::endl << std::endl;
}

/*!
 * Should we show tooltips?
 */

bool point_cloud_canvas::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap point_cloud_canvas::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin point_cloud_canvas bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end point_cloud_canvas bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon point_cloud_canvas::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin point_cloud_canvas icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end point_cloud_canvas icon retrieval
}

/*!
 * InitGL
 */

void point_cloud_canvas::initGL()
{
    static const GLfloat light0_pos[4]   = { -50.0f, 50.0f, 0.0f, 0.0f };

    // white light
    static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
    /* light */
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);

    glEnable(GL_LIGHT0);
      /* remove back faces */
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* speedups */
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

  	    // Point Size
    float pointSize = 2.0;
    glPointSize(pointSize);
}

/*!
 * reset_projection_mode
 */
void point_cloud_canvas::reset_projection_mode()
{
    int w, h;
    GetClientSize(&w, &h);
    SetCurrent();
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat)w/h, 1.0, +100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void point_cloud_canvas::on_timer(wxTimerEvent&)
{
  Refresh(false);
  //wxPaintEvent dummyevent;
  //OnPaint(dummyevent);
}


/*!
 * All mouse events event handler for ID_POINTCLOUD_CANVAS
 */

void point_cloud_canvas::OnMouse( wxMouseEvent& event )
{
  int mwheel = event.GetWheelRotation();

  if(mwheel>0)
  {
    zcam -= 2.0;
    (zcam > 10.0)? (zcam) : (10.0);
    reset_projection_mode();
  }
  else if (mwheel < 0)
  {
    zcam += 2.0;
    reset_projection_mode();
  }

    if (event.Dragging())
    {
        wxSize sz(GetClientSize());

        /* drag in progress, simulate trackball */
        float spin_quat[4];
        trackball(spin_quat,
            (2.0*beginx - sz.x) / sz.x,
            (sz.y - 2.0*beginy) / sz.y,
            (2.0*event.GetX() - sz.x)    / sz.x,
            (sz.y - 2.0*event.GetY())    / sz.y);

        add_quats(spin_quat, quat, quat);

        /* orientation has changed, redraw mesh */
        Refresh(false);
    }

    beginx = event.GetX();
    beginy = event.GetY();

}




