/*
 * KiRouter - a push-and-(sometimes-)shove PCB router
 *
 * Copyright (C) 2014  CERN
 * Author: Maciej Suminski <maciej.suminski@cern.ch>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.or/licenses/>.
 */

/**
 * Push and Shove router track width and via size dialog.
 */

#include "dialog_track_via_size.h"
#include <base_units.h>
#include <confirm.h>
#include <boost/optional.hpp>

#include "class_board_design_settings.h"

DIALOG_TRACK_VIA_SIZE::DIALOG_TRACK_VIA_SIZE( wxWindow* aParent, BOARD_DESIGN_SETTINGS& aSettings ) :
    DIALOG_TRACK_VIA_SIZE_BASE( aParent ),
    m_settings( aSettings )
{
    // Load router settings to dialog fields
    m_trackWidth->SetValue( To_User_Unit( m_trackWidth->GetUnits(), m_settings.GetCustomTrackWidth() ) );
    m_viaDiameter->SetValue( To_User_Unit( m_viaDiameter->GetUnits(), m_settings.GetCustomViaSize() ) );
    m_viaDrill->SetValue( To_User_Unit( m_viaDrill->GetUnits(), m_settings.GetCustomViaDrill() ) );

    m_trackWidth->SetFocus();

    // Pressing ENTER when any of the text input fields is active applies changes
    #if wxCHECK_VERSION( 3, 0, 0 )
        Connect( wxEVT_TEXT_ENTER, wxCommandEventHandler( DIALOG_TRACK_VIA_SIZE::onOkClick ), NULL, this );
    #else
        Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DIALOG_TRACK_VIA_SIZE::onOkClick ), NULL, this );
    #endif
    
    Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DIALOG_TRACK_VIA_SIZE::onClose ) );
}


bool DIALOG_TRACK_VIA_SIZE::check()
{
    // Wrong input
    if( !m_trackWidth->GetValue() || !m_viaDiameter->GetValue() || !m_viaDrill->GetValue() )
        return false;

    // Via drill should be smaller than via diameter
    if( m_viaDrill->GetValue() >= m_viaDiameter->GetValue() )
        return false;

    return true;
}


void DIALOG_TRACK_VIA_SIZE::onClose( wxCloseEvent& aEvent )
{
    EndModal( 0 );
}


void DIALOG_TRACK_VIA_SIZE::onOkClick( wxCommandEvent& aEvent )
{
    if( check() )
    {
        // Store dialog values to the router settings
        m_settings.SetCustomTrackWidth( From_User_Unit( m_trackWidth->GetUnits(), *m_trackWidth->GetValue() ) );
        m_settings.SetCustomViaSize( From_User_Unit( m_viaDiameter->GetUnits(), *m_viaDiameter->GetValue() ) );
        m_settings.SetCustomViaDrill( From_User_Unit( m_viaDrill->GetUnits(), *m_viaDrill->GetValue() ) );
        EndModal( 1 );
    }
    else
    {
        DisplayError( GetParent(), _( "Settings are incorrect" ) );
        m_trackWidth->SetFocus();
    }
}


void DIALOG_TRACK_VIA_SIZE::onCancelClick( wxCommandEvent& aEvent )
{
    EndModal( 0 );
}
