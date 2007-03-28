/////////////////////////////////////////////////////////////////////////////
// Name:        exploration_machine_gui.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     23/03/2007 02:33:51
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _EXPLORATION_MACHINE_GUI_H_
#define _EXPLORATION_MACHINE_GUI_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "exploration_machine_gui.h"
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
class tri_scene_canvas;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_EXPLORATION_MACHINE_GUI_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_EXPLORATION_MACHINE_GUI_TITLE _("Dialog")
#define SYMBOL_EXPLORATION_MACHINE_GUI_IDNAME ID_EXPLORATION_MACHINE_GUI
#define SYMBOL_EXPLORATION_MACHINE_GUI_SIZE wxSize(600, 400)
#define SYMBOL_EXPLORATION_MACHINE_GUI_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * exploration_machine_gui class declaration
 */

class exploration_machine_gui: public wxFrame
{    
    DECLARE_CLASS( exploration_machine_gui )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    exploration_machine_gui();
    exploration_machine_gui( wxWindow* parent, wxWindowID id = SYMBOL_EXPLORATION_MACHINE_GUI_IDNAME, const wxString& caption = SYMBOL_EXPLORATION_MACHINE_GUI_TITLE, const wxPoint& pos = SYMBOL_EXPLORATION_MACHINE_GUI_POSITION, const wxSize& size = SYMBOL_EXPLORATION_MACHINE_GUI_SIZE, long style = SYMBOL_EXPLORATION_MACHINE_GUI_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_EXPLORATION_MACHINE_GUI_IDNAME, const wxString& caption = SYMBOL_EXPLORATION_MACHINE_GUI_TITLE, const wxPoint& pos = SYMBOL_EXPLORATION_MACHINE_GUI_POSITION, const wxSize& size = SYMBOL_EXPLORATION_MACHINE_GUI_SIZE, long style = SYMBOL_EXPLORATION_MACHINE_GUI_STYLE );

    /// Destructor
    ~exploration_machine_gui();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin exploration_machine_gui event handler declarations

////@end exploration_machine_gui event handler declarations

////@begin exploration_machine_gui member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end exploration_machine_gui member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin exploration_machine_gui member variables
    /// Control identifiers
    enum {
        ID_EXPLORATION_MACHINE_GUI = 10000
    };
////@end exploration_machine_gui member variables
};

#endif
    // _EXPLORATION_MACHINE_GUI_H_
