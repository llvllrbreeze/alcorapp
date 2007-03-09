/////////////////////////////////////////////////////////////////////////////
// Name:        bumblebee_frame.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     09/03/2007 01:01:27
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "bumblebee_frame.h"
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
#include "gl_point_cloud_canvas.h"
////@end includes

#include "bumblebee_frame.h"

////@begin XPM images
////@end XPM images

/*!
 * bumblebee_frame type definition
 */

IMPLEMENT_CLASS( bumblebee_frame, wxFrame )

/*!
 * bumblebee_frame event table definition
 */

BEGIN_EVENT_TABLE( bumblebee_frame, wxFrame )

////@begin bumblebee_frame event table entries
////@end bumblebee_frame event table entries

END_EVENT_TABLE()

/*!
 * bumblebee_frame constructors
 */

bumblebee_frame::bumblebee_frame( )
{
    Init();
}

bumblebee_frame::bumblebee_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * bumblebee_frame creator
 */

bool bumblebee_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin bumblebee_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end bumblebee_frame creation
    return true;
}

/*!
 * Member initialisation 
 */

void bumblebee_frame::Init()
{
////@begin bumblebee_frame member initialisation
////@end bumblebee_frame member initialisation
  //bee.reset(new all::sense::bumblebee_driver_t);
  //bee->open("config/bumblebeeB.ini");
}
/*!
 * Control creation for bumblebee_frame
 */

void bumblebee_frame::CreateControls()
{    
////@begin bumblebee_frame content construction
    bumblebee_frame* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, 0, 0);

    point_cloud_view = new point_cloud_canvas( itemFrame1, ID_POINTCLOUD_CANVAS, wxDefaultPosition, wxSize(640, 480), wxSIMPLE_BORDER );
    itemBoxSizer3->Add(point_cloud_view, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemFrame1, ID_BUTTON1, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end bumblebee_frame content construction

    /////BIND!!!
    //point_cloud_view->grab = 
    //  boost::bind(&bumblebee_frame::grab, itemFrame1);

    //point_cloud_view->update_rgb_data = 
    //  boost::bind(&bumblebee_frame::update_rgb_data, itemFrame1);

    //point_cloud_view->update_3d_data = 
    //  boost::bind(&bumblebee_frame::update_3d_data, itemFrame1);
}

/*!
 * Should we show tooltips?
 */

bool bumblebee_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap bumblebee_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin bumblebee_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end bumblebee_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon bumblebee_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin bumblebee_frame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end bumblebee_frame icon retrieval
}

/////grabcallback
//void bumblebee_frame::grab()
//{
//  bee->grab();
//}
//
/////callback
//all::core::single_sarr bumblebee_frame::update_3d_data()
//{
//  //bee->grab();
//  return bee->get_depth_buffer();
//}
//
/////callback
//all::core::uint8_sarr bumblebee_frame::update_rgb_data()
//{
//  //bee->grab();
//  return bee->get_color_buffer(all::core::right_img);
//}
