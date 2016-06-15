/**
 * prEditor.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "../prConfig.h"
#include "prEditor.h"


#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)


#include "../core/prMacros.h"
#include "../gui/prGui.h"
#include "../gui/prMenuStrip.h"
#include "../gui/prMenuItem.h"
#include "../gui/prMenu.h"
#include "../display/prBitmapFont.h"
#include "../font/prFontBase.h"
#include "../input/prKeys.h"


using namespace Proteus::Core;
using namespace Proteus::Gui;


namespace
{
    prGui       *pGui           = nullptr;
    prMenuStrip *pMenuStrip     = nullptr;
}


/// ---------------------------------------------------------------------------
/// Creates the editors menus
/// ---------------------------------------------------------------------------
void EditorCreateGameMenus(prGui *pTheGui, prMessageManager *pMessageManager, prBitmapFont *pBitmapFntDark, prBitmapFont *pBitmapFntGrey)
{
    PRASSERT(pTheGui);
    PRASSERT(pMessageManager);
    PRASSERT(pBitmapFntDark);
    PRASSERT(pBitmapFntGrey);

    // Create the GUI
    pGui = pTheGui;    
    pGui->SetBMPFont(pBitmapFntDark);

    pMenuStrip = static_cast<prMenuStrip *>(pGui->Create(WT_MenuStrip, "GameMenuStrip"));
    if (pMenuStrip)
    {
        // Create the file menu
        {
            prMenu *pFileMenu = static_cast<prMenu *>(pGui->Create(WT_Menu, "EditorFileMenu"));
            pFileMenu->SetText("File");
            pFileMenu->SetGrayFontAsBitmapFont(pBitmapFntGrey);
            pFileMenu->SetMessageManager(pMessageManager);

			// Exit
            prMenuItem *pExit = new prMenuItem("Exit", EDITOR_FILE_EXIT);
            pFileMenu->AddMenuItem(pExit);

            // Add menu
            pMenuStrip->AddMenu(pFileMenu);
        }

        // Create the tool menu
        {
            prMenu *pToolMenu = static_cast<prMenu *>(pGui->Create(WT_Menu, "EditorToolMenu"));
            pToolMenu->SetText("Tool");
            pToolMenu->SetGrayFontAsBitmapFont(pBitmapFntGrey);
            pToolMenu->SetMessageManager(pMessageManager);

            // Create the menu items
            prMenuItem *pFpsOn    = new prMenuItem("FPS On",  EDITOR_FPS_ON);
            pFpsOn->SetCommandKeys(prMenuItemControlKey::Control, PRKEY_F);

            prMenuItem *pFpsOff   = new prMenuItem("FPS Off",  EDITOR_FPS_OFF);
            prMenuItem *pFpsReset = new prMenuItem("FPS Reset",  EDITOR_FPS_RESET);
            prMenuItem *pFpsPos   = new prMenuItem("FPS Position",  EDITOR_FPS_POSITION);

            // Add the menu items
            pToolMenu->AddMenuItem(pFpsOn);
            pToolMenu->AddMenuItem(pFpsOff);
            pToolMenu->AddMenuItem(pFpsReset);
            pToolMenu->AddMenuItem(pFpsPos);

            // Add menu
            pMenuStrip->AddMenu(pToolMenu);
        }

        // Create the profile menu
        {
            prMenu *pProfileMenu = static_cast<prMenu *>(pGui->Create(WT_Menu, "EditorProfileMenu"));
            pProfileMenu->SetText("Profile");
            pProfileMenu->SetGrayFontAsBitmapFont(pBitmapFntGrey);
            pProfileMenu->SetMessageManager(pMessageManager);

            // Create the menu items
            prMenuItem *pFpsOn    = new prMenuItem("Profile On",  EDITOR_PROFILE_ON);
            prMenuItem *pFpsOff   = new prMenuItem("Profile Off",  EDITOR_PROFILE_OFF);

            // Add the menu items
            pProfileMenu->AddMenuItem(pFpsOn);
            pProfileMenu->AddMenuItem(pFpsOff);

            pMenuStrip->AddMenu(pProfileMenu);
        }

        // Create the step menu
        {
            prMenu *pStepMenu = static_cast<prMenu *>(pGui->Create(WT_Menu, "EditorStepMenu"));
            pStepMenu->SetText("Step");
            pStepMenu->SetGrayFontAsBitmapFont(pBitmapFntGrey);
            pStepMenu->SetMessageManager(pMessageManager);

            // Create the menu items
            prMenuItem *pStep      = new prMenuItem("Step",  EDITOR_STEP_STEP);
            prMenuItem *pStepPause = new prMenuItem("Pause",  EDITOR_STEP_PAUSE);

            // Add the menu items
            pStepMenu->AddMenuItem(pStep);
            pStepMenu->AddMenuItem(pStepPause);

            pMenuStrip->AddMenu(pStepMenu);
        }

        // Create the help menu
        {
            prMenu *pHelpMenu = static_cast<prMenu *>(pGui->Create(WT_Menu, "EditorHelpMenu"));
            pHelpMenu->SetText("Help");
            pHelpMenu->SetGrayFontAsBitmapFont(pBitmapFntGrey);
            pHelpMenu->SetMessageManager(pMessageManager);

            // Create the menu items
            prMenuItem *pHelp   = new prMenuItem("Show",  EDITOR_HELP_SHOW);

            // Add the menu items
            pHelpMenu->AddMenuItem(pHelp);

            pMenuStrip->AddMenu(pHelpMenu);
        }
    }
}

#endif
