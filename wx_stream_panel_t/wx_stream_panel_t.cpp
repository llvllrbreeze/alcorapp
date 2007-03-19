/////////////////////////////////////////////////////////////////////////////
// Name:        wx_stream_panel_t.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     16/03/2007 15:13:39
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wx_stream_panel_t.hpp"
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

#include "wx_stream_panel_t.hpp"


wx_stream_dest_t::wx_stream_dest_t(wxWindow* dc) {
	m_dc = dc;
	m_decoder.set_output_ordering(all::core::interleaved_tag);
}

void wx_stream_dest_t::process_data() {
	boost::crc_32_type::value_type enc_crc;
	int enc_data_size = m_data_size - sizeof(enc_crc);
	all::core::uint8_sarr enc_data(new all::core::uint8_t[enc_data_size]);

	memcpy(&enc_crc, m_data.get(), sizeof(enc_crc));

	memcpy(enc_data.get(), m_data.get() + sizeof(enc_crc), enc_data_size);

	if (m_decoder.decode(m_image, enc_data, enc_data_size)) {
		draw_panel();
	}
}

void wx_stream_dest_t::draw_panel() {

	wxClientDC dc_client(m_dc);
	
	wxImage cam_image(m_image.width, m_image.height, reinterpret_cast <unsigned char*> (m_image.data.get()), true);

	wxBitmap cam_bmp(cam_image, 24);

	m_dc->PrepareDC(dc_client);
	dc_client.DrawBitmap(cam_bmp, 0, 0, false);
    
}

/*!
 * wx_stream_panel_t type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wx_stream_panel_t, wxPanel )

/*!
 * wx_stream_panel_t event table definition
 */

BEGIN_EVENT_TABLE( wx_stream_panel_t, wxPanel )

////@begin wx_stream_panel_t event table entries

////@end wx_stream_panel_t event table entries

END_EVENT_TABLE()

/*!
 * wx_stream_panel_t constructors
 */

wx_stream_panel_t::wx_stream_panel_t(char* ini_file)
{
	m_stream_ini = ini_file;
    Init();
}

wx_stream_panel_t::wx_stream_panel_t( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*!
 * wx_stream_panel_t creator
 */

bool wx_stream_panel_t::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wx_stream_panel_t creation
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
////@end wx_stream_panel_t creation
    return true;
}

/*!
 * wx_stream_panel_t destructor
 */

wx_stream_panel_t::~wx_stream_panel_t()
{
////@begin wx_stream_panel_t destruction
////@end wx_stream_panel_t destruction
	//delete m_stream_client;
}

/*!
 * Member initialisation 
 */

void wx_stream_panel_t::Init()
{
////@begin wx_stream_panel_t member initialisation
////@end wx_stream_panel_t member initialisation
}
/*!
 * Control creation for wx_stream_panel_t
 */

void wx_stream_panel_t::CreateControls()
{    

	m_stream_dest.reset(new wx_stream_dest_t(this));

	m_stream_client.reset (new all::core::stream_client_t(m_stream_dest, m_stream_ini));
	m_stream_client->run_async();

}

/*!
 * Should we show tooltips?
 */

bool wx_stream_panel_t::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wx_stream_panel_t::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wx_stream_panel_t bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wx_stream_panel_t bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wx_stream_panel_t::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wx_stream_panel_t icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wx_stream_panel_t icon retrieval
}



