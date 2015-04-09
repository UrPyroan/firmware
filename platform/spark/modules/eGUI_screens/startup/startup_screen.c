/**
 ******************************************************************************
 * @file    startup_screen.c
 * @authors Matthew McGowan
 ******************************************************************************
  Copyright (c) 2015 Brewpi/Matthew McGowan.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#include "d4d.h"
#include "../brewpi_colors.h"
#include "../brewpi-logo.h"
#include "../widget_color_scheme.h"
#include "../fonts.h"
#include "startup_screen.h"
#include "Version.h"

D4D_DECLARE_STD_PICTURE(scrStartup_logo, 100, 40, 120, 80, &bmp_brewpi_logo_black_120_80);
D4D_DECLARE_COLOR_LABEL(scrStartup_version, "BrewPi " VERSION_STRING, 0, 140, 320, 15, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, D4D_COLOR_BLACK);

char startup_text[] = "Tap screen to re-calibrate touch"; // 32 characters

D4D_DECLARE_COLOR_LABEL(scrStartup_text, startup_text, 160-16*7, 200, 32*7, 15, FONT_SMALL, D4D_CONST, D4D_COLOR_BLACK, LOW_TEXT_COLOR);

D4D_DECLARE_SCREEN_BEGIN(screen_startup, ScrStartup_, 0 ,0, (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), NULL, 0, NULL, (D4D_SCR_F_DEFAULT|D4D_SCR_F_TOUCHENABLE), NULL)
    D4D_DECLARE_SCREEN_OBJECT(scrStartup_logo)
    D4D_DECLARE_SCREEN_OBJECT(scrStartup_version)
    D4D_DECLARE_SCREEN_OBJECT(scrStartup_text)    
D4D_DECLARE_SCREEN_END()        

        
static void ScrStartup_OnInit()
{
    
}

static void ScrStartup_OnMain()
{       
}


static void ScrStartup_OnActivate()
{    
}

static void ScrStartup_OnDeactivate()
{
}

static Byte ScrStartup_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg);
    if (pMsg->nMsgId==D4D_MSG_TOUCHED) 
        startup_screen_touched();
    return 0;
}
