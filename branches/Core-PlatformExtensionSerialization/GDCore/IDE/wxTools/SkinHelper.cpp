/** \file
 *  Game Develop
 *  2008-2013 Florian Rival (Florian.Rival@gmail.com)
 */
#include "SkinHelper.h"
#include "AuiTabArt.h"
#include "FlatAuiTabArt.h"
#include "GDCore/IDE/CommonBitmapManager.h"
#include "GDCore/IDE/wxTools/RibbonMetroArtProvider.h"
#include <wx/ribbon/bar.h>
#include <wx/ribbon/art.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/propgrid/propgrid.h>
#include <wx/config.h>
#include <wx/dcbuffer.h>
#include <wx/settings.h>

namespace gd
{

/** \brief Internal art provider providing a nice help button instead of the default one.
 */
class gdRibbonMSWArtProvider : public wxRibbonMSWArtProvider
{
public:
    gdRibbonMSWArtProvider() : wxRibbonMSWArtProvider(), helpBitmap(gd::CommonBitmapManager::GetInstance()->help16) {};
    virtual ~gdRibbonMSWArtProvider() {};
    wxRibbonArtProvider* Clone() const { return new gdRibbonMSWArtProvider(*this); };

    /** A mere copy of the original implementation, with a custom bitmap used instead of the default one.
     */
    void DrawHelpButton(wxDC& dc, wxRibbonBar* wnd, const wxRect& rect)
    {
        DrawPartialPageBackground(dc, wnd, rect, false);

        dc.DestroyClippingRegion();
        dc.SetClippingRegion(rect);

        if ( wnd->IsHelpButtonHovered() )
        {
            dc.SetPen(m_ribbon_toggle_pen);
            dc.SetBrush(m_ribbon_toggle_brush);
            dc.DrawRoundedRectangle(rect.GetX(), rect.GetY(), 20, 20, 1.0);
            dc.DrawBitmap(helpBitmap, rect.GetX ()+2, rect.GetY()+2, true);
        }
        else
        {
            dc.DrawBitmap(helpBitmap, rect.GetX ()+2, rect.GetY()+2, true);
        }

    }

    wxBitmap helpBitmap;
};

/** \brief Internal art provider providing a nice help button instead of the default one.
 */
class gdRibbonAUIArtProvider : public wxRibbonAUIArtProvider
{
public:
    gdRibbonAUIArtProvider() : wxRibbonAUIArtProvider(), helpBitmap(gd::CommonBitmapManager::GetInstance()->help16) {};
    virtual ~gdRibbonAUIArtProvider() {};
    wxRibbonArtProvider* Clone() const { return new gdRibbonAUIArtProvider(*this); };

    /** A mere copy of the original implementation, with a custom bitmap used instead of the default one.
     */
    void DrawHelpButton(wxDC& dc, wxRibbonBar* wnd, const wxRect& rect)
    {
        DrawPartialPageBackground(dc, wnd, rect, false);

        dc.DestroyClippingRegion();
        dc.SetClippingRegion(rect);

        if ( wnd->IsHelpButtonHovered() )
        {
            dc.SetPen(m_ribbon_toggle_pen);
            dc.SetBrush(m_ribbon_toggle_brush);
            dc.DrawRoundedRectangle(rect.GetX(), rect.GetY(), 20, 20, 1.0);
            dc.DrawBitmap(helpBitmap, rect.GetX ()+2, rect.GetY()+2, true);
        }
        else
        {
            dc.DrawBitmap(helpBitmap, rect.GetX ()+2, rect.GetY()+2, true);
        }

    }

    wxBitmap helpBitmap;
};

void SkinHelper::ApplyCurrentSkin(wxRibbonBar & bar)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString result;
    pConfig->Read( _T( "/Skin/RDefined" ), &result );

    //Ribbon skin
    if ( result == "true" )
    {
        int r = 120, v = 120, b = 120;
        int r2 = 120, v2 = 120, b2 = 120;

        wxRibbonArtProvider * ribbonArtProvider = NULL;
        pConfig->Read( _T( "/Skin/RibbonStyle" ), &result );

        //Style
        if ( result == "AUI" )
            ribbonArtProvider = new gdRibbonAUIArtProvider;
        else if ( result == "Metro" )
            ribbonArtProvider = new RibbonMetroArtProvider;
        else
            ribbonArtProvider = new gdRibbonMSWArtProvider;

        bar.SetArtProvider(ribbonArtProvider);

        //Colors
        pConfig->Read( _T( "/Skin/Ribbon1R" ), &r );
        pConfig->Read( _T( "/Skin/Ribbon1G" ), &v );
        pConfig->Read( _T( "/Skin/Ribbon1B" ), &b );

        pConfig->Read( _T( "/Skin/Ribbon2R" ), &r2 );
        pConfig->Read( _T( "/Skin/Ribbon2G" ), &v2 );
        pConfig->Read( _T( "/Skin/Ribbon2B" ), &b2 );

        wxColour colour, secondary, tertiary;
        bar.GetArtProvider()->GetColourScheme(&colour, &secondary, &tertiary);
        bar.GetArtProvider()->SetColourScheme(wxColour(r, v, b), wxColour(r2, v2, b2), wxColour(0, 0, 0));

    }
    else
    {
        bar.SetArtProvider(new gdRibbonMSWArtProvider);
        bar.GetArtProvider()->SetColourScheme(wxColour(244, 245, 247), wxColour(231, 241, 254), wxColour(0, 0, 0));
    }
}

void SkinHelper::ApplyCurrentSkin(wxAuiManager & auiManager)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString result;

    //DockArt skin
    wxAuiDefaultDockArt *dockArt = new wxAuiDefaultDockArt();
    pConfig->Read( _T( "/Skin/Defined" ), &result );
    dockArt->SetFont(wxAUI_DOCKART_CAPTION_FONT, wxFont(10, wxDEFAULT, wxNORMAL, wxNORMAL, FALSE));
    if ( result == "true" )
    {
        int r = 120, v = 120, b = 120;

        pConfig->Read( _T( "/Skin/PaneA1R" ), &r );
        pConfig->Read( _T( "/Skin/PaneA1G" ), &v );
        pConfig->Read( _T( "/Skin/PaneA1B" ), &b );
        dockArt->SetColour( 7, wxColour( r, v, b ) );

        pConfig->Read( _T( "/Skin/PaneA2R" ), &r );
        pConfig->Read( _T( "/Skin/PaneA2G" ), &v );
        pConfig->Read( _T( "/Skin/PaneA2B" ), &b );
        dockArt->SetColour( 8, wxColour( r, v, b ) );

        pConfig->Read( _T( "/Skin/PaneI1R" ), &r );
        pConfig->Read( _T( "/Skin/PaneI1G" ), &v );
        pConfig->Read( _T( "/Skin/PaneI1B" ), &b );
        dockArt->SetColour( 9, wxColour( r, v, b ) );

        pConfig->Read( _T( "/Skin/PaneI2R" ), &r );
        pConfig->Read( _T( "/Skin/PaneI2G" ), &v );
        pConfig->Read( _T( "/Skin/PaneI2B" ), &b );
        dockArt->SetColour( 10, wxColour( r, v, b ) );

        pConfig->Read( _T( "/Skin/BorderR" ), &r );
        pConfig->Read( _T( "/Skin/BorderG" ), &v );
        pConfig->Read( _T( "/Skin/BorderB" ), &b );
        dockArt->SetColour( 13, wxColour( r, v, b ) );

        pConfig->Read( _T( "/Skin/BackR" ), &r );
        pConfig->Read( _T( "/Skin/BackG" ), &v );
        pConfig->Read( _T( "/Skin/BackB" ), &b );
        dockArt->SetColour( 6, wxColour( r, v, b ) );

        pConfig->Read( _T( "/Skin/ATextR" ), &r );
        pConfig->Read( _T( "/Skin/ATextG" ), &v );
        pConfig->Read( _T( "/Skin/ATextB" ), &b );
        dockArt->SetColour( 11, wxColour( r, v, b ) );

        pConfig->Read( _T( "/Skin/ITextR" ), &r );
        pConfig->Read( _T( "/Skin/ITextG" ), &v );
        pConfig->Read( _T( "/Skin/ITextB" ), &b );
        dockArt->SetColour( 12, wxColour( r, v, b ) );
    }
    else
    {
        dockArt->SetColour(6, wxColour(211,222,246));
        dockArt->SetColour(13, wxColour(172,183,208));
        dockArt->SetColour(9, wxColour(214,221,233));
        dockArt->SetColour(10, wxColour(214,221,233));
        dockArt->SetColour(7, wxColour(221,229,246));
        dockArt->SetColour(8, wxColour(221,229,246));
        dockArt->SetColour(11, wxColour(104,114,138));
        dockArt->SetColour(12, wxColour(104,114,138));
    }
    dockArt->SetColour(wxAUI_DOCKART_BACKGROUND_COLOUR, wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

    auiManager.SetArtProvider(dockArt);
}

void SkinHelper::ApplyCurrentSkin(wxAuiNotebook & notebook, bool subnotebook)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString result;

    wxAuiTabArt * tabArt = NULL;
    pConfig->Read( _T( "/Skin/TabStyle" ), &result );
    if ( result == "Flat" )
    {
        gd::FlatAuiTabArt * art = new gd::FlatAuiTabArt();
        art->DisableBackgroundGradient(subnotebook);        
        tabArt = art;
    }
    else
    {
        gd::AuiTabArt * art = new gd::AuiTabArt();
        art->DisableBackgroundGradient(subnotebook);        
        tabArt = art;
    }

    pConfig->Read( _T( "/Skin/Defined" ), &result );
    if ( result == "true" )
    {
        wxColor tabColor;
        pConfig->Read( _T( "/Skin/TabColor" ), &tabColor );
        tabArt->SetColour(tabColor);

        wxColor activeTabColor;
        pConfig->Read( _T( "/Skin/ActiveTabColor" ), &activeTabColor );
        tabArt->SetActiveColour(activeTabColor);
    }
    else
    {
        tabArt->SetColour(wxColour(220, 225, 232));
        tabArt->SetActiveColour(wxColour(220, 225, 232));
    }

    notebook.SetArtProvider(tabArt);
}

class AuiToolBarArt : public wxAuiDefaultToolBarArt
{
public:

    AuiToolBarArt() : wxAuiDefaultToolBarArt() {};
    virtual ~AuiToolBarArt() {};

    virtual wxAuiToolBarArt* Clone() { return static_cast<wxAuiToolBarArt*>(new AuiToolBarArt); };

    void SetBaseColour(wxColour baseColour)
    {
        m_baseColour = baseColour;
    }

    virtual void DrawBackground( wxDC& dc, wxWindow* wnd, const wxRect& rect_)
    {
        //Defines the two rectangles for gradient
        wxRect rect1 = rect_;
        rect1.height /= 3;
        wxRect rect2 = rect_;
        rect2.y += rect1.height;
        rect2.height = rect2.height*2.0/3.0+1;

        wxColour startColour = m_baseColour.ChangeLightness(165);
        wxColour intermediateColour = m_baseColour.ChangeLightness(140);
        wxColour endColour = m_baseColour.ChangeLightness(125);
        intermediateColour.Set(intermediateColour.Red()-10,intermediateColour.Green()-10, intermediateColour.Blue()+10);

        dc.GradientFillLinear(rect1, startColour, intermediateColour, wxSOUTH);
        dc.GradientFillLinear(rect2, intermediateColour, endColour, wxSOUTH);
    }
};

class FlatAuiToolBarArt : public wxAuiDefaultToolBarArt
{
public:

    FlatAuiToolBarArt() : wxAuiDefaultToolBarArt() {};
    virtual ~FlatAuiToolBarArt() {};

    virtual wxAuiToolBarArt* Clone() { return static_cast<wxAuiToolBarArt*>(new FlatAuiToolBarArt); };

    void SetBaseColour(wxColour baseColour)
    {
        m_baseColour = baseColour;
    }

    virtual void DrawBackground( wxDC& dc, wxWindow* wnd, const wxRect& rect_)
    {
        wxColour startColour = m_baseColour.ChangeLightness(165);
        dc.GradientFillLinear(rect_, startColour, startColour, wxSOUTH);
    }
};

void SkinHelper::ApplyCurrentSkin(wxAuiToolBar & toolbar)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString result = "Classic";

    //DockArt skin
    pConfig->Read( _T( "/Skin/ToolbarStyle" ), &result );
    wxColor baseColor;
    pConfig->Read( _T( "/Skin/ToolbarCustomColor" ), &baseColor );

    bool toolbarCustomColor = false;
    pConfig->Read( _T( "/Skin/ToolbarHasCustomColor" ), &toolbarCustomColor );

    if ( result == "Flat" ) 
    {
        FlatAuiToolBarArt * art = new FlatAuiToolBarArt;
        if ( toolbarCustomColor ) art->SetBaseColour(baseColor);
        toolbar.SetArtProvider(art);   
    }
    else
    {
        AuiToolBarArt * art = new AuiToolBarArt;
        if ( toolbarCustomColor ) art->SetBaseColour(baseColor);
        toolbar.SetArtProvider(art);   
    }
}

void SkinHelper::ApplyCurrentSkin(wxPropertyGrid & propertyGrid)
{
    propertyGrid.SetMarginColour( wxSystemSettings::GetColour(wxSYS_COLOUR_MENU) );
    propertyGrid.SetCaptionBackgroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_MENU) );
    propertyGrid.SetEmptySpaceColour( wxSystemSettings::GetColour(wxSYS_COLOUR_MENU) );
    propertyGrid.SetCellBackgroundColour( *wxWHITE );
    propertyGrid.SetCellTextColour( *wxBLACK );
    propertyGrid.SetLineColour( wxColour(212,208,200) );
}

}
