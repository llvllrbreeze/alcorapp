/////////////////////////////////////////////////////////////////////////////
// Name:        urg_2d_plot.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     10/03/2007 12:51:34
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _URG_2D_PLOT_H_
#define _URG_2D_PLOT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "urg_2d_plot.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "urg_2d_plot_frame.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * urg_2d_plot_app class declaration
 */

class urg_2d_plot_app: public wxApp
{    
    DECLARE_CLASS( urg_2d_plot_app )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    urg_2d_plot_app();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin urg_2d_plot_app event handler declarations

////@end urg_2d_plot_app event handler declarations

////@begin urg_2d_plot_app member function declarations

////@end urg_2d_plot_app member function declarations

////@begin urg_2d_plot_app member variables
////@end urg_2d_plot_app member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(urg_2d_plot_app)
////@end declare app

#endif
    // _URG_2D_PLOT_H_
