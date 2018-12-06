/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of BrewBlox.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "startup_screen.h"
#include "BrewPiTouch.h"
#include "screen.h"
#include "spark_wiring_timer.h"
#include "widget_color_scheme.h"
#include <algorithm>

extern BrewPiTouch touch;

#define str(s) #s
#define xstr(s) str(s)

D4D_DECLARE_STD_LABEL(scrStartup_version, "BrewBlox " xstr(GIT_VERSION), 0, 140, 320, 15, FONT_SMALL);
D4D_DECLARE_STD_LABEL(scrStartup_text, "Tap screen to re-calibrate touch", 160 - 16 * 7, 200, 32 * 7, 15, FONT_SMALL)
D4D_DECLARE_STD_PROGRESS_BAR(scrStartup_progress, 80, 180, 160, 20, 0)

D4D_DECLARE_SCREEN_BEGIN(screen_startup, ScrStartup_, 0, 0, (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), nullptr, 0, nullptr, (D4D_SCR_F_DEFAULT | D4D_SCR_F_TOUCHENABLE), nullptr)
D4D_DECLARE_SCREEN_OBJECT(scrStartup_version)
D4D_DECLARE_SCREEN_OBJECT(scrStartup_text)
D4D_DECLARE_SCREEN_OBJECT(scrStartup_progress)
D4D_DECLARE_SCREEN_END()

void
StartupScreen::activate()
{
    D4D_ActivateScreen(&screen_startup, D4D_TRUE);
}

void
StartupScreen::calibrateTouchIfNeeded()
{
    static D4D_TOUCHSCREEN_CALIB calib; // TODO load calibration

    if (calib.ScreenCalibrated != 1) {
        calibrateTouch();
        // store calibration
        D4D_TCH_SetCalibration(calib);
    }
}

void
StartupScreen::calibrateTouch()
{
#if PLATFORM_ID != 3
    touch.setStabilityThreshold(5); // require extra stable reading
    auto timeoutTimer = Timer(20000, D4D_InterruptCalibrationScreen, true);
#endif
    D4D_CalibrateTouchScreen();
#if PLATFORM_ID != 3
    touch.setStabilityThreshold(); // reset to default
#endif
}

void
ScrStartup_OnInit()
{
    D4D_PrgrsBarSetValue(&scrStartup_progress, 0);
}

void
ScrStartup_OnMain()
{
    static D4D_PROGRESS_BAR_VALUE v = 0;
    v = (v + 1) % 100;
    D4D_PrgrsBarSetValue(&scrStartup_progress, v);
}

void
ScrStartup_OnActivate()
{
}

void
ScrStartup_OnDeactivate()
{
}

uint8_t
ScrStartup_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    if (pMsg->nMsgId == D4D_MSG_TOUCHED) {
        StartupScreen::calibrateTouch();
    }
    return D4D_FALSE; // don't block further processing
}