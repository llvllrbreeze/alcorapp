/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_gui.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:49:28
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _EXPLORATION_GUI_H_
#define _EXPLORATION_GUI_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "exploration_gui.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wx_img_stream_panel_t;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_EXPLORATION_GUI_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxNO_BORDER
#define SYMBOL_EXPLORATION_GUI_TITLE _("attentive eploration")
#define SYMBOL_EXPLORATION_GUI_IDNAME ID_EXPLORATION_GUI
#define SYMBOL_EXPLORATION_GUI_SIZE wxSize(400, 300)
#define SYMBOL_EXPLORATION_GUI_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * exploration_gui class declaration
 */

class exploration_gui: public wxFrame
{    
    DECLARE_CLASS( exploration_gui )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    exploration_gui();
    exploration_gui( wxWindow* parent, wxWindowID id = SYMBOL_EXPLORATION_GUI_IDNAME, const wxString& caption = SYMBOL_EXPLORATION_GUI_TITLE, const wxPoint& pos = SYMBOL_EXPLORATION_GUI_POSITION, const wxSize& size = SYMBOL_EXPLORATION_GUI_SIZE, long style = SYMBOL_EXPLORATION_GUI_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EXPLORATION_GUI_IDNAME, const wxString& caption = SYMBOL_EXPLORATION_GUI_TITLE, const wxPoint& pos = SYMBOL_EXPLORATION_GUI_POSITION, const wxSize& size = SYMBOL_EXPLORATION_GUI_SIZE, long style = SYMBOL_EXPLORATION_GUI_STYLE );

    /// Destructor
    ~exploration_gui();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin exploration_gui event handler declarations

////@end exploration_gui event handler declarations

////@begin exploration_gui member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end exploration_gui member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin exploration_gui member variables
    wx_img_stream_panel_t* m_rgb_panel;
    wx_img_stream_panel_t* m_3d_panel;
    /// Control identifiers
    enum {
        ID_EXPLORATION_GUI = 10000,
        ID_RGB_PANEL = 10001,
        ID_3D_PANEL = 10002,
        ID_BUTTON1 = 10003,
        ID_BUTTON2 = 10004,
        ID_BUTTON3 = 10005
    };
////@end exploration_gui member variables
};

#endif
    // _EXPLORATION_GUI_H_
