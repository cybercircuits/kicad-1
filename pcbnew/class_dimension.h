/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 1992-2011 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file class_dimension.h
 * @brief DIMENSION class definition.
 */

#ifndef DIMENSION_H_
#define DIMENSION_H_


#include <class_board_item.h>


class LINE_READER;
class EDA_DRAW_PANEL;
class TEXTE_PCB;


class DIMENSION : public BOARD_ITEM
{
public:
    int        m_Width;
    wxPoint    m_Pos;
    int        m_Shape;         /// Current always 0.
    int        m_Unit;          /// 0 = inches, 1 = mm
    int        m_Value;         /// value of PCB dimensions.

    TEXTE_PCB  m_Text;
    int        m_crossBarOx, m_crossBarOy, m_crossBarFx, m_crossBarFy;
    int        m_featureLineGOx, m_featureLineGOy, m_featureLineGFx, m_featureLineGFy;
    int        m_featureLineDOx, m_featureLineDOy, m_featureLineDFx, m_featureLineDFy;
    int        m_arrowD1Ox, m_arrowD1Oy, m_arrowD1Fx, m_arrowD1Fy;
    int        m_arrowD2Ox, m_arrowD2Oy, m_arrowD2Fx, m_arrowD2Fy;
    int        m_arrowG1Ox, m_arrowG1Oy, m_arrowG1Fx, m_arrowG1Fy;
    int        m_arrowG2Ox, m_arrowG2Oy, m_arrowG2Fx, m_arrowG2Fy;

public:
    DIMENSION( BOARD_ITEM* aParent );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    ~DIMENSION();

    const wxPoint& GetPosition() const      { return m_Pos; }

    void SetPosition( const wxPoint& aPos );    // override, sets m_Text's position too

    void SetTextSize( const wxSize& aTextSize )
    {
        m_Text.SetSize( aTextSize );
    }

    void SetLayer( int aLayer );

    void SetShape( int aShape )         { m_Shape = aShape; }
    int GetShape() const                { return m_Shape; }

    int GetWidth() const                { return m_Width; }
    void SetWidth( int aWidth )         { m_Width = aWidth; }

    /**
     * Function AdjustDimensionDetails
     * Calculate coordinates of segments used to draw the dimension.
     * @param aDoNotChangeText (bool) if false, the dimension text is initialized
     */
    void AdjustDimensionDetails( bool aDoNotChangeText = false );

    void SetText( const wxString& NewText );
    const wxString GetText() const;

    void Copy( DIMENSION* source );

    void Draw( EDA_DRAW_PANEL* panel, wxDC* DC,
               int aColorMode, const wxPoint& offset = ZeroOffset );

    /**
     * Function Move
     * @param offset : moving vector
     */
    void Move( const wxPoint& offset );

    void Rotate( const wxPoint& aRotCentre, double aAngle );

    void Flip( const wxPoint& aCentre );

    /**
     * Function Mirror
     * Mirror the Dimension , relative to a given horizontal axis
     * the text is not mirrored. only its position (and angle) is mirrored
     * the layer is not changed
     * @param axis_pos : vertical axis position
     */
    void Mirror( const wxPoint& axis_pos );

    void DisplayInfo( EDA_DRAW_FRAME* frame );

    bool HitTest( const wxPoint& aPosition );

    bool HitTest( const EDA_RECT& aRect ) const;

    wxString GetClass() const
    {
        return wxT( "DIMENSION" );
    }

    EDA_RECT GetBoundingBox() const;

    wxString GetSelectMenuText() const;

    BITMAP_DEF GetMenuImage() const { return  add_dimension_xpm; }

    EDA_ITEM* Clone() const;

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const { ShowDummy( os ); } // override
#endif
};

#endif  // DIMENSION_H_
