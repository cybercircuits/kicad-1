/**********************************************************************/
/* fonctions membres des classes utilisees dans pcbnew (voir pcbstruct.h */
/*	  sauf routines relatives aux pistes (voir class_track.cpp)		  */
/**********************************************************************/

#include "fctsys.h"

#include "common.h"
#include "pcbnew.h"

#include "trigo.h"
#include "id.h"


/* Default PCB zoom coefficients. */
static const int PcbZoomList[] = { 5, 10, 15, 20, 40, 80, 160, 320, 640, 1280,
                                   2560, 5120, 10240, 20480 };

#define PCB_ZOOM_LIST_CNT ( sizeof( PcbZoomList ) / sizeof( int ) )


/* Default grid sizes for PCB editor screens. */
static GRID_TYPE PcbGridList[] = {
    { ID_POPUP_GRID_LEVEL_1000, wxSize( 1000, 1000 ) },
    { ID_POPUP_GRID_LEVEL_500, wxSize( 500, 500 ) },
    { ID_POPUP_GRID_LEVEL_250, wxSize( 250, 250 ) },
    { ID_POPUP_GRID_LEVEL_200, wxSize( 200, 200 ) },
    { ID_POPUP_GRID_LEVEL_100, wxSize( 100, 100 ) },
    { ID_POPUP_GRID_LEVEL_50, wxSize( 50, 50 ) },
    { ID_POPUP_GRID_LEVEL_25, wxSize( 25, 25 ) },
    { ID_POPUP_GRID_LEVEL_20, wxSize( 20, 20 ) },
    { ID_POPUP_GRID_LEVEL_10, wxSize( 10, 10 ) },
    { ID_POPUP_GRID_LEVEL_5, wxSize( 5, 5 ) },
    { ID_POPUP_GRID_LEVEL_2, wxSize( 2, 2 ) },
    { ID_POPUP_GRID_LEVEL_1, wxSize( 1, 1 ) }
};

#define PCB_GRID_LIST_CNT ( sizeof( PcbGridList ) / sizeof( GRID_TYPE ) )


/**************************************************/
/* Class SCREEN: classe de gestion d'un affichage */
/***************************************************/
/* Constructeur de SCREEN */
PCB_SCREEN::PCB_SCREEN( ) : BASE_SCREEN( TYPE_SCREEN )
{
    size_t i;

    for( i = 0; i < PCB_ZOOM_LIST_CNT; i++ )
        m_ZoomList.Add( PcbZoomList[i] );

    for( i = 0; i < PCB_GRID_LIST_CNT; i++ )
        AddGrid( PcbGridList[i] );

    SetGrid( wxSize( 500, 500 ) );        /* pas de la grille en 1/10000 "*/
    Init();
}


/***************************/
PCB_SCREEN::~PCB_SCREEN()
/***************************/
{
}

/*************************/
void PCB_SCREEN::Init()
/*************************/
{
    InitDatas();
    m_Active_Layer       = COPPER_LAYER_N;      /* ref couche active 0.. 31 */
    m_Route_Layer_TOP    = CMP_N;               /* ref couches par defaut pour vias (Cu.. Cmp) */
    m_Route_Layer_BOTTOM = COPPER_LAYER_N;
    m_Zoom               = 128;                               /* valeur */
}

int PCB_SCREEN::GetInternalUnits( void )
{
    return PCB_INTERNAL_UNIT;
}

/* Return true if a microvia can be put on board
 * A microvia ia a small via restricted to 2 near neighbour layers
 * because its is hole is made by laser which can penetrate only one layer
 * It is mainly used to connect BGA to the first inner layer
 * And it is allowed from an external layer to the first inner layer
 */
bool PCB_SCREEN::IsMicroViaAcceptable( void )
{
    int copperlayercnt = g_DesignSettings.m_CopperLayerCount;

    if( !g_DesignSettings.m_MicroViasAllowed )
        return false;   // Obvious..
    if( copperlayercnt < 4 )
        return false;   // Only on multilayer boards..
    if( ( m_Active_Layer == COPPER_LAYER_N )
       || ( m_Active_Layer == LAYER_CMP_N )
       || ( m_Active_Layer == g_DesignSettings.m_CopperLayerCount - 2 )
       || ( m_Active_Layer == LAYER_N_2 ) )
        return true;

    return false;
}


/*************************/
/* class DISPLAY_OPTIONS */
/*************************/

/*
 *  Handle display options like enable/disable some optional drawings:
 */

DISPLAY_OPTIONS::DISPLAY_OPTIONS()
{
    DisplayPadFill   = TRUE;
    DisplayPadNum    = TRUE;
    DisplayPadNoConn = TRUE;
    DisplayPadIsol   = TRUE;

    DisplayModEdge      = TRUE;
    DisplayModText      = TRUE;
    DisplayPcbTrackFill = TRUE; /* FALSE = sketch , TRUE = rempli */
    DisplayTrackIsol    = FALSE;
    m_DisplayViaMode    = VIA_HOLE_NOT_SHOW;

    DisplayPolarCood = TRUE;
    DisplayZonesMode = 0;       // 0 = Show filled areas outlines in zones,
                                // 1 = do not show filled areas outlines
                                // 2 = show outlines of filled areas
    Show_Modules_Cmp = TRUE;
    Show_Modules_Cu  = TRUE;

    DisplayDrawItems    = TRUE;
    ContrastModeDisplay = FALSE;
}


/*****************************************************/
EDA_BoardDesignSettings::EDA_BoardDesignSettings()
/*****************************************************/

// Default values for designing boards
{
    int ii;

    static const int default_layer_color[32] = {
        GREEN,     LIGHTGRAY, LIGHTGRAY, LIGHTGRAY,
        LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY,
        LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY,
        LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, RED,
        LIGHTGRAY, LIGHTGRAY,
        LIGHTGRAY, LIGHTGRAY,
        MAGENTA,   CYAN,
        LIGHTGRAY, LIGHTGRAY,
        LIGHTGRAY,
        LIGHTGRAY,
        LIGHTGRAY, LIGHTGRAY,
        LIGHTGRAY,
        LIGHTGRAY,
        LIGHTGRAY,
        LIGHTGRAY
    };

    m_CopperLayerCount = 2;                         // Default design is a double sided board
    m_ViaDrill = 250;                               // defualt via drill (for the entire board)
    m_ViaDrillCustomValue = 250;                    // via drill for vias which must have a defined drill value
    m_CurrentViaSize    = 450;                      // Current via size
    m_CurrentViaType    = VIA_THROUGH;              // via type (VIA_BLIND_BURIED, VIA_THROUGH VIA_MICROVIA)
    m_CurrentTrackWidth = 170;                      // current track width
    m_UseConnectedTrackWidth = false;				// if true, when creating a new track starting on an existing track, use this track width
    m_MicroViaDrill = 50;                           // micro via drill (for the entire board)
    m_CurrentMicroViaSize = 150;                    // Current micro via size
    m_MicroViasAllowed    = false;                  // true to allow micro vias

    for( ii = 0; ii < HISTORY_NUMBER; ii++ )
    {
        m_TrackWidthHistory[ii] = 0;    // Last HISTORY_NUMBER used track widths
        m_ViaSizeHistory[ii]    = 0;    // Last HISTORY_NUMBER used via sizes
    }

    m_DrawSegmentWidth = 100;               // current graphic line width (not EDGE layer)
    m_EdgeSegmentWidth = 100;               // current graphic line width (EDGE layer only)
    m_PcbTextWidth   = 100;                 // current Pcb (not module) Text width
    m_PcbTextSize    = wxSize( 500, 500 );  // current Pcb (not module) Text size
    m_TrackClearence = 100;                 // track to track and track to pads clearance
    m_MaskMargin = 150;                     // Solder mask margin
    /* Color options for screen display of the Printed Board: */
    m_PcbGridColor = DARKGRAY;              // Grid color

    for( ii = 0; ii < 32; ii++ )
        m_LayerColor[ii] = default_layer_color[ii];

    // Layer colors (tracks and graphic items)
    m_ViaColor[VIA_NOT_DEFINED]  = DARKGRAY;
    m_ViaColor[VIA_MICROVIA]     = CYAN;
    m_ViaColor[VIA_BLIND_BURIED] = BROWN;
    m_ViaColor[VIA_THROUGH] = WHITE;

    m_ModuleTextCMPColor    = LIGHTGRAY;    // Text module color for modules on the COMPONENT layer
    m_ModuleTextCUColor  = MAGENTA;         // Text module color for modules on the COPPER layer
    m_ModuleTextNOVColor = DARKGRAY;        // Text module color for "invisible" texts (must be BLACK if really not displayed)
    m_AnchorColor   = BLUE;                 // Anchor color for modules and texts
    m_PadCUColor    = GREEN;                // Pad color for the COMPONENT side of the pad
    m_PadCMPColor   = RED;                  // Pad color for the COPPER side of the pad
    m_RatsnestColor = WHITE;                // Ratsnest color
}


// see pcbstruct.h
int EDA_BoardDesignSettings::GetVisibleLayers() const
{
    int layerMask = 0;

    for( int i = 0, mask = 1;  i< 32;   ++i, mask <<= 1 )
    {
        if( !( m_LayerColor[i] & ITEM_NOT_SHOW ) )
            layerMask |= mask;
    }

    return layerMask;
}
