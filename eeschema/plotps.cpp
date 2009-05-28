/////////////////////////////////////////////////////////////////////////////

// Name:        plotps.cpp
// Purpose:
// Author:      jean-pierre Charras
// Modified by:
// Created:     01/02/2006 08:37:24
// RCS-ID:
// Copyright:   GNU License
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 01/02/2006 08:37:24

#if defined (__GNUG__) && !defined (NO_GCC_PRAGMA)
#pragma implementation "plotps.h"
#endif

#include "fctsys.h"
#include "gr_basic.h"

#include "common.h"
#include "program.h"
#include "libcmp.h"
#include "general.h"
#include "worksheet.h"
#include "plot_common.h"
#include "protos.h"

// coeff de conversion dim en 1 mil -> dim en unite PS:
const double SCALE_PS = 0.001;

extern void Move_Plume( wxPoint pos, int plume );
extern void Plume( int plume );

enum PageFormatReq {
    PAGE_SIZE_AUTO,
    PAGE_SIZE_A4,
    PAGE_SIZE_A
};


/* Variables locales : */
static int   PS_SizeSelect  = PAGE_SIZE_AUTO;
extern FILE* PlotOutput;
static bool  Plot_Sheet_Ref = TRUE;


////@begin includes
////@end includes

#include "plotps.h"

////@begin XPM images
////@end XPM images


/***********************************************************/
void WinEDA_SchematicFrame::ToPlot_PS( wxCommandEvent& event )
/***********************************************************/

/* fonction relai de creation de la frame de dialogue pour trace Postscript
 */
{
    wxPoint pos;

    pos = GetPosition();

    pos.x += 10;
    pos.y += 20;

    WinEDA_PlotPSFrame* Ps_frame = new WinEDA_PlotPSFrame( this );

    Ps_frame->ShowModal();
    Ps_frame->Destroy();
}


/*!
 * WinEDA_PlotPSFrame type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WinEDA_PlotPSFrame, wxDialog )

/*!
 * WinEDA_PlotPSFrame event table definition
 */

BEGIN_EVENT_TABLE( WinEDA_PlotPSFrame, wxDialog )

////@begin WinEDA_PlotPSFrame event table entries
    EVT_BUTTON( ID_PLOT_PS_CURRENT_EXECUTE, WinEDA_PlotPSFrame::OnPlotPsCurrentExecuteClick )

    EVT_BUTTON( ID_PLOT_PS_ALL_EXECUTE, WinEDA_PlotPSFrame::OnPlotPsAllExecuteClick )

    EVT_BUTTON( wxID_CANCEL, WinEDA_PlotPSFrame::OnCancelClick )

////@end WinEDA_PlotPSFrame event table entries

END_EVENT_TABLE()

/*!
 * WinEDA_PlotPSFrame constructors
 */

WinEDA_PlotPSFrame::WinEDA_PlotPSFrame()
{
}


WinEDA_PlotPSFrame::WinEDA_PlotPSFrame( WinEDA_DrawFrame* parent,
                                        wxWindowID        id,
                                        const wxString&   caption,
                                        const wxPoint&    pos,
                                        const wxSize&     size,
                                        long              style )
{
    m_Parent = parent;
    Create( parent, id, caption, pos, size, style );
}


/*!
 * WinEDA_PlotPSFrame creator
 */

bool WinEDA_PlotPSFrame::Create( wxWindow*       parent,
                                 wxWindowID      id,
                                 const wxString& caption,
                                 const wxPoint&  pos,
                                 const wxSize&   size,
                                 long            style )
{
////@begin WinEDA_PlotPSFrame member initialisation
    m_SizeOption = NULL;
    m_PlotPSColorOption = NULL;
    m_Plot_Sheet_Ref = NULL;
    m_btClose = NULL;
    m_DefaultLineSizeCtrlSizer = NULL;
    m_MsgBox = NULL;
////@end WinEDA_PlotPSFrame member initialisation

////@begin WinEDA_PlotPSFrame creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WinEDA_PlotPSFrame creation
    return true;
}


/*!
 * Control creation for WinEDA_PlotPSFrame
 */

void WinEDA_PlotPSFrame::CreateControls()
{
////@begin WinEDA_PlotPSFrame content construction
    // Generated by DialogBlocks, 24/04/2009 14:25:24 (unregistered)

    WinEDA_PlotPSFrame* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString m_SizeOptionStrings;
    m_SizeOptionStrings.Add(_("Auto"));
    m_SizeOptionStrings.Add(_("Page Size A4"));
    m_SizeOptionStrings.Add(_("Page Size A"));
    m_SizeOption = new wxRadioBox( itemDialog1, ID_RADIOBOX1, _("Plot page size:"), wxDefaultPosition, wxDefaultSize, m_SizeOptionStrings, 1, wxRA_SPECIFY_COLS );
    m_SizeOption->SetSelection(0);
    itemBoxSizer3->Add(m_SizeOption, 0, wxGROW|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Plot Options:"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_PlotPSColorOptionStrings;
    m_PlotPSColorOptionStrings.Add(_("B/W"));
    m_PlotPSColorOptionStrings.Add(_("Color"));
    m_PlotPSColorOption = new wxRadioBox( itemDialog1, ID_RADIOBOX, _("Plot Color:"), wxDefaultPosition, wxDefaultSize, m_PlotPSColorOptionStrings, 1, wxRA_SPECIFY_COLS );
    m_PlotPSColorOption->SetSelection(0);
    itemStaticBoxSizer6->Add(m_PlotPSColorOption, 0, wxGROW|wxALL, 5);

    m_Plot_Sheet_Ref = new wxCheckBox( itemDialog1, ID_CHECKBOX, _("Print Sheet Ref"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
    m_Plot_Sheet_Ref->SetValue(false);
    itemStaticBoxSizer6->Add(m_Plot_Sheet_Ref, 0, wxGROW|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_PLOT_PS_CURRENT_EXECUTE, _("&Plot Page"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton11->SetDefault();
    itemBoxSizer10->Add(itemButton11, 0, wxGROW|wxALL, 5);

    wxButton* itemButton12 = new wxButton( itemDialog1, ID_PLOT_PS_ALL_EXECUTE, _("Plot A&LL"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemButton12, 0, wxGROW|wxALL, 5);

    m_btClose = new wxButton( itemDialog1, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(m_btClose, 0, wxGROW|wxALL, 5);

    m_DefaultLineSizeCtrlSizer = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(m_DefaultLineSizeCtrlSizer, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemDialog1, wxID_STATIC, _("Messages :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText15, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP|wxADJUST_MINSIZE, 5);

    m_MsgBox = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxSize(-1, 200), wxTE_MULTILINE );
    itemBoxSizer2->Add(m_MsgBox, 0, wxGROW|wxALL|wxFIXED_MINSIZE, 5);

    // Set validators
    m_SizeOption->SetValidator( wxGenericValidator(& PS_SizeSelect) );
    m_PlotPSColorOption->SetValidator( wxGenericValidator(& g_PlotPSColorOpt) );
    m_Plot_Sheet_Ref->SetValidator( wxGenericValidator(& Plot_Sheet_Ref) );
////@end WinEDA_PlotPSFrame content construction

    SetFocus(); // make the ESC work
    m_DefaultLineSizeCtrl = new WinEDA_ValueCtrl( this, _(
                                                      "Default Line Width" ),
                                                  g_PlotLine_Width,
                                                  g_UnitMetric, m_DefaultLineSizeCtrlSizer,
                                                  EESCHEMA_INTERNAL_UNIT );
}


/*!
 * Should we show tooltips?
 */

bool WinEDA_PlotPSFrame::ShowToolTips()
{
    return true;
}


/*!
 * Get bitmap resources
 */

wxBitmap WinEDA_PlotPSFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WinEDA_PlotPSFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WinEDA_PlotPSFrame bitmap retrieval
}


/*!
 * Get icon resources
 */

wxIcon WinEDA_PlotPSFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WinEDA_PlotPSFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WinEDA_PlotPSFrame icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void WinEDA_PlotPSFrame::OnPlotPsCurrentExecuteClick( wxCommandEvent& event )
{
    int Select_PlotAll = FALSE;

    InitOptVars();
    CreatePSFile( Select_PlotAll, PS_SizeSelect );
    m_MsgBox->AppendText( wxT( "*****\n" ) );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
 */

void WinEDA_PlotPSFrame::OnPlotPsAllExecuteClick( wxCommandEvent& event )
{
    int Select_PlotAll = TRUE;

    InitOptVars();
    CreatePSFile( Select_PlotAll, PS_SizeSelect );
    m_MsgBox->AppendText( wxT( "*****\n" ) );
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void WinEDA_PlotPSFrame::OnCancelClick( wxCommandEvent& event )
{
    InitOptVars();
    EndModal( 0 );
}


/*****************************************/
void WinEDA_PlotPSFrame::InitOptVars()
/*****************************************/
{
    Plot_Sheet_Ref   = m_Plot_Sheet_Ref->GetValue();
    g_PlotPSColorOpt = m_PlotPSColorOption->GetSelection();
    PS_SizeSelect    = m_SizeOption->GetSelection();
    g_PlotLine_Width = m_DefaultLineSizeCtrl->GetValue();
    if( g_PlotLine_Width < 1 )
        g_PlotLine_Width = 1;
}


/*************************************************************/
void WinEDA_PlotPSFrame::CreatePSFile( int AllPages, int pagesize )
/*************************************************************/
{
    WinEDA_SchematicFrame* schframe     = (WinEDA_SchematicFrame*) m_Parent;
    SCH_SCREEN*            screen       = schframe->GetScreen();
    SCH_SCREEN*            oldscreen    = screen;
    DrawSheetPath*         sheetpath, *oldsheetpath = schframe->GetSheet();
    wxString PlotFileName;
    Ki_PageDescr*          PlotSheet, * RealSheet;
    int BBox[4];
    wxPoint plot_offset;

    g_PlotFormat = PLOT_FORMAT_POST;

    /* When printing all pages, the printed page is not the current page.
     *  In complex hierarchies, we must setup references and others parameters in the printed SCH_SCREEN
     *  because in complex hierarchies a SCH_SCREEN (a schematic drawings)
     *  is shared between many sheets
     */
    EDA_SheetList SheetList( NULL );
    sheetpath = SheetList.GetFirst();
    DrawSheetPath list;

    for( ; ;  )
    {
        if( AllPages )
        {
            if( sheetpath == NULL )
                break;
            list.Clear();
            if( list.BuildSheetPathInfoFromSheetPathValue( sheetpath->Path() ) )
            {
                schframe->m_CurrentSheet = &list;
                schframe->m_CurrentSheet->UpdateAllScreenReferences();
                schframe->SetSheetNumberAndCount();
                screen = schframe->m_CurrentSheet->LastScreen();
                ActiveScreen = screen;
            }
            else  // Should not happen
                return;
            sheetpath = SheetList.GetNext();
        }
        PlotSheet = screen->m_CurrentSheetDesc;
        RealSheet = &g_Sheet_A4;

        if( pagesize == PAGE_SIZE_AUTO )
            RealSheet = PlotSheet;
        else if( pagesize == PAGE_SIZE_A )
            RealSheet = &g_Sheet_A;

        /* Calculate plot bouding box in 1/1000 inch */
        BBox[0] = BBox[1] = g_PlotMargin;   // Plot margin in 1/1000 inch
        BBox[2] = RealSheet->m_Size.x - g_PlotMargin;
        BBox[3] = RealSheet->m_Size.y - g_PlotMargin;

        /* Calculate pcbnew to PS conversion scale */
        g_PlotScaleX = SCALE_PS * (float) (BBox[2] - BBox[0]) / PlotSheet->m_Size.x;
        g_PlotScaleY = SCALE_PS * (float) (BBox[3] - BBox[1]) / PlotSheet->m_Size.y;

        plot_offset.x = 0;
        plot_offset.y = PlotSheet->m_Size.y;

        PlotFileName = schframe->GetUniqueFilenameForCurrentSheet( ) + wxT( ".ps" );

        PlotOneSheetPS( PlotFileName, screen, RealSheet, BBox, plot_offset );

        if( !AllPages )
            break;
    }

    ActiveScreen = oldscreen;
    schframe->m_CurrentSheet = oldsheetpath;
    schframe->m_CurrentSheet->UpdateAllScreenReferences();
    schframe->SetSheetNumberAndCount();
}


/*****************************************************************************************/
void WinEDA_PlotPSFrame::PlotOneSheetPS( const wxString& FileName,
                                         SCH_SCREEN*     screen,
                                         Ki_PageDescr*   sheet,
                                         int             BBox[4],
                                         wxPoint         plot_offset )
/*****************************************************************************************/

/* Trace en format PS. d'une feuille de dessin
 */
{
    wxString       Line;
    SCH_ITEM*      DrawList;
    SCH_COMPONENT* DrawLibItem;
    int            layer;
    wxPoint        StartPos, EndPos;

    PlotOutput = wxFopen( FileName, wxT( "wt" ) );
    if( PlotOutput == NULL )
    {
        Line  = wxT( "\n** " );
        Line += _( "Unable to create " ) + FileName + wxT( " **\n\n" );
        m_MsgBox->AppendText( Line );
        wxBell();
        return;
    }

    SetLocaleTo_C_standard();
    Line.Printf( _( "Plot: %s\n" ), FileName.GetData() );
    m_MsgBox->AppendText( Line );

    InitPlotParametresPS( plot_offset, sheet, g_PlotScaleX, g_PlotScaleY );
    SetDefaultLineWidthPS( g_PlotLine_Width );

    /* Init : */
    PrintHeaderPS( PlotOutput, wxT( "EESchema-PS" ), FileName, 1, BBox, wxLANDSCAPE );
    InitPlotParametresPS( plot_offset, sheet, 1.0, 1.0 );

    if( m_Plot_Sheet_Ref->GetValue() )
    {
        if( (g_PlotFormat == PLOT_FORMAT_POST) && g_PlotPSColorOpt )
            SetColorMapPS( BLACK );
        m_Parent->PlotWorkSheet( PLOT_FORMAT_POST, screen );
    }

    DrawList = screen->EEDrawList;
    while( DrawList )  /* tracage */
    {
        layer = LAYER_NOTES;

        switch( DrawList->Type() )
        {
        case DRAW_BUSENTRY_STRUCT_TYPE:             /* Struct Raccord et Segment sont identiques */
            #undef STRUCT
            #define STRUCT ( (DrawBusEntryStruct*) DrawList )
            StartPos = STRUCT->m_Pos;
            EndPos   = STRUCT->m_End();
            layer    = STRUCT->GetLayer();

        case DRAW_SEGMENT_STRUCT_TYPE:
            #undef STRUCT
            #define STRUCT ( (EDA_DrawLineStruct*) DrawList )
            if( DrawList->Type() == DRAW_SEGMENT_STRUCT_TYPE )
            {
                StartPos = STRUCT->m_Start;
                EndPos   = STRUCT->m_End;
                layer    = STRUCT->GetLayer();
            }
            if( g_PlotPSColorOpt )
                SetColorMapPS( ReturnLayerColor( layer ) );

            switch( layer )
            {
            case LAYER_NOTES:         /* Trace en pointilles */
                SetCurrentLineWidth( -1 );
                fprintf( PlotOutput, "[50 50] 0 setdash\n" );
                Move_Plume( StartPos, 'U' );
                Move_Plume( EndPos, 'D' );
                Plume( 'Z' );
                fprintf( PlotOutput, "[] 0 setdash\n" );
                break;

            case LAYER_BUS:         /* Trait large */
            {
                fprintf( PlotOutput, "%d setlinewidth\n", g_PlotLine_Width * 3 );
                Move_Plume( StartPos, 'U' );
                Move_Plume( EndPos, 'D' );
                Plume( 'Z' );
                fprintf( PlotOutput, "%d setlinewidth\n", g_PlotLine_Width );
            }
                break;

            default:
                SetCurrentLineWidth( -1 );
                Move_Plume( StartPos, 'U' );
                Move_Plume( EndPos, 'D' );
                Plume( 'Z' );
                break;
            }

            break;

        case DRAW_JUNCTION_STRUCT_TYPE:
                #undef STRUCT
                #define STRUCT ( (DrawJunctionStruct*) DrawList )
            if( g_PlotPSColorOpt )
                SetColorMapPS( ReturnLayerColor( STRUCT->GetLayer() ) );
            PlotCercle( STRUCT->m_Pos, DRAWJUNCTION_SIZE, 1 );
            break;

        case TYPE_SCH_TEXT:
        case TYPE_SCH_LABEL:
        case TYPE_SCH_GLOBALLABEL:
        case TYPE_SCH_HIERLABEL:
            PlotTextStruct( DrawList );
            break;

        case TYPE_SCH_COMPONENT:
            DrawLibItem = (SCH_COMPONENT*) DrawList;
            PlotLibPart( DrawLibItem );
            break;

        case DRAW_PICK_ITEM_STRUCT_TYPE:
            break;

        case DRAW_POLYLINE_STRUCT_TYPE:
            break;

        case DRAW_HIERARCHICAL_PIN_SHEET_STRUCT_TYPE:
            break;

        case DRAW_MARKER_STRUCT_TYPE:
            break;

        case DRAW_SHEET_STRUCT_TYPE:
                #undef STRUCT
                #define STRUCT ( (DrawSheetStruct*) DrawList )
            PlotSheetStruct( STRUCT );
            break;

        case DRAW_NOCONNECT_STRUCT_TYPE:
                #undef STRUCT
                #define STRUCT ( (DrawNoConnectStruct*) DrawList )
            if( g_PlotPSColorOpt )
                SetColorMapPS( ReturnLayerColor( LAYER_NOCONNECT ) );
            PlotNoConnectStruct( STRUCT );
            break;

        default:
            break;
        }

        DrawList = DrawList->Next();
    }

    /* fin */
    CloseFilePS( PlotOutput );
    SetLocaleTo_Default();

    m_MsgBox->AppendText( wxT( "Ok\n" ) );
}
