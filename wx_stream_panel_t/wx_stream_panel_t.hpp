/////////////////////////////////////////////////////////////////////////////
// Name:        wx_stream_panel_t.hpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     16/03/2007 15:13:39
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STREAM_PANEL_T_H_
#define _WX_STREAM_PANEL_T_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "wx_stream_panel_t.hpp"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

#include <alcor/core/stream_client_t.hpp>
#include <alcor/core/stream_dest_t.hpp>
#include <alcor/core/jpeg_decoder_t.h>
#include <boost/enable_shared_from_this.hpp>


class wx_stream_dest_t : public all::core::stream_dest_t {

public:
	wx_stream_dest_t(wxWindow*);

	//jpeg decoder functions
	void process_data();
	void draw_panel();

private:
	//jpeg decoder members
    all::core::jpeg_decoder_t m_decoder;
	all::core::jpeg_data_t m_image;

	wxWindow* m_dc;
};

typedef boost::shared_ptr<wx_stream_dest_t> wx_stream_dest_ptr;

/*!
 * Forward declarations
 */

////@begin forward declarations
class wx_stream_panel_t;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PANEL1 10001
#define SYMBOL_WX_STREAM_PANEL_T_STYLE wxSUNKEN_BORDER|wxTAB_TRAVERSAL
#define SYMBOL_WX_STREAM_PANEL_T_IDNAME ID_PANEL1
#define SYMBOL_WX_STREAM_PANEL_T_SIZE wxDefaultSize
#define SYMBOL_WX_STREAM_PANEL_T_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wx_stream_panel_t class declaration
 */

class wx_stream_panel_t: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( wx_stream_panel_t )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wx_stream_panel_t(char* ini_file = "config/stream_client.ini");
    wx_stream_panel_t( wxWindow* parent, wxWindowID id = SYMBOL_WX_STREAM_PANEL_T_IDNAME, const wxPoint& pos = SYMBOL_WX_STREAM_PANEL_T_POSITION, const wxSize& size = SYMBOL_WX_STREAM_PANEL_T_SIZE, long style = SYMBOL_WX_STREAM_PANEL_T_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WX_STREAM_PANEL_T_IDNAME, const wxPoint& pos = SYMBOL_WX_STREAM_PANEL_T_POSITION, const wxSize& size = SYMBOL_WX_STREAM_PANEL_T_SIZE, long style = SYMBOL_WX_STREAM_PANEL_T_STYLE );

    /// Destructor
    ~wx_stream_panel_t();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wx_stream_panel_t event handler declarations


////@end wx_stream_panel_t event handler declarations

////@begin wx_stream_panel_t member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wx_stream_panel_t member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wx_stream_panel_t member variables
////@end wx_stream_panel_t member variables

	char* m_stream_ini;
	all::core::stream_client_ptr m_stream_client;
	wx_stream_dest_ptr m_stream_dest;
};

typedef boost::shared_ptr<wx_stream_panel_t> wx_stream_panel_ptr;

#endif
    // _WX_STREAM_PANEL_T_H_
