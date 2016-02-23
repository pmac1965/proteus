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


#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)


#include "../core/prMacros.h"
#include "../gui/prGui.h"
#include "../gui/prMenuStrip.h"


using namespace Proteus::Gui;


namespace
{
    prGui       *pGui           = nullptr;
    prMenuStrip *pMenuStrip     = nullptr;
}


/// ---------------------------------------------------------------------------
/// Creates the editors menus
/// ---------------------------------------------------------------------------
void EditorCreateGameMenus()
{
    if (pGui != nullptr)
        return;

    // Create the GUI
    pGui = new prGui();
    PRASSERT(pGui);
    
    //mApp.mpGui->SetBMPFont(mApp.mpTextFontDark);

    pMenuStrip = static_cast<prMenuStrip *>(pGui->Create(WT_MenuStrip, "GameMenuStrip"));
    if (pMenuStrip)
    {
        /*
        // Create the file menu
        {
            prMenu *pFileMenu = static_cast<prMenu *>(mApp.mpGui->Create(WT_Menu, "FileMenu"));
            pFileMenu->SetText("File");

            // Create the menu items
            prMenuItem *pNew    = new prMenuItem("New",     MENU_ITEM_FILE_NEW);
            prMenuItem *pOpen   = new prMenuItem("Open",    MENU_ITEM_FILE_OPEN);
            //prMenuItem *_pSep_  = new prMenuItem("-",       MENU_ITEM_SEPARATOR);
            prMenuItem *pSave   = new prMenuItem("Save",    MENU_ITEM_FILE_SAVE);
            prMenuItem *pSaveAs = new prMenuItem("Save As", MENU_ITEM_FILE_SAVEAS);
            prMenuItem *pExit   = new prMenuItem("Exit",    MENU_ITEM_FILE_OPEN);

            // Add the menu items
            pFileMenu->AddMenuItem(pNew);
            pFileMenu->AddMenuItem(pOpen);
            //pFileMenu->AddMenuItem(_pSep_);
            pFileMenu->AddMenuItem(pSave);
            pFileMenu->AddMenuItem(pSaveAs);
            //pFileMenu->AddMenuItem(_pSep_);
            pFileMenu->AddMenuItem(pExit);

            // Add menu
            pMenuStrip->AddMenu(pFileMenu);
        }

        // Create the edit menu
        {
            prMenu *pEditMenu = static_cast<prMenu *>(mApp.mpGui->Create(WT_Menu, "EditMenu"));
            pEditMenu->SetText("Edit");

            // Create the menu items
            prMenuItem *pPrefs = new prMenuItem("Prefs",  MENU_ITEM_EDIT_PREFS);

            // Add the menu items
            pEditMenu->AddMenuItem(pPrefs);

            // Add menu
            pMenuStrip->AddMenu(pEditMenu);
        }

        // Create the view menu
        {
            prMenu *pViewMenu = static_cast<prMenu *>(mApp.mpGui->Create(WT_Menu, "ViewMenu"));
            pViewMenu->SetText("View");

            pMenuStrip->AddMenu(pViewMenu);
        }

        // Create the help menu
        {
            prMenu *pHelpMenu = static_cast<prMenu *>(mApp.mpGui->Create(WT_Menu, "HelpMenu"));
            pHelpMenu->SetText("Help");

            // Create the menu items
            prMenuItem *pManual = new prMenuItem("Manual",  MENU_ITEM_HELP_MANUAL);
            prMenuItem *pHelp   = new prMenuItem("About",  MENU_ITEM_HELP_ABOUT);

            // Add the menu items
            pHelpMenu->AddMenuItem(pManual);
            pHelpMenu->AddMenuItem(pHelp);

            pMenuStrip->AddMenu(pHelpMenu);
        }//*/
    }
}


/// ---------------------------------------------------------------------------
/// Destroys the editors menus
/// ---------------------------------------------------------------------------
void EditorDestroyGameMenus()
{
    PRSAFE_DELETE(pGui);
}


#endif


//#if defined(PLATFORM_PC)
//
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//
//void CreateBasicMenu(HWND hWnd)
//{
////    HMENU hMenu = CreateMenu();
////    HMENU hSubMenu = CreatePopupMenu();
////
////    AppendMenu(hSubMenu, MF_STRING, ID_EXIT_GAME, L"&Quit");
////    AppendMenu(hSubMenu, MF_STRING, ID_SAVE_GAME, L"&Save Game");
//////    AppendMenu(hSubMenu, MF_STRING, ID_QUIT_ITEM, L"&Quit");
////    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"&File");
////
////    //hSubMenu = CreatePopupMenu();
////    //AppendMenu(hSubMenu, MF_STRING, ID_SHOW_ALL_ITEM, L"Show &All Data");
////    //AppendMenu(hSubMenu, MF_STRING, ID_SELECT_REPORT_ITEM, L"S&eelect report");
////    //AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"&Reports");
////
////    SetMenu(hWnd, hMenu);
//}
//#else
//
//#endif
//
//
///// ---------------------------------------------------------------------------
///// Creates the editors menus
///// ---------------------------------------------------------------------------
//void EditorCreateMenus()
//{
//}
