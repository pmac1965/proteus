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


#if defined(PLATFORM_PC)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void CreateBasicMenu(HWND hWnd)
{
//    HMENU hMenu = CreateMenu();
//    HMENU hSubMenu = CreatePopupMenu();
//
//    AppendMenu(hSubMenu, MF_STRING, ID_EXIT_GAME, L"&Quit");
//    AppendMenu(hSubMenu, MF_STRING, ID_SAVE_GAME, L"&Save Game");
////    AppendMenu(hSubMenu, MF_STRING, ID_QUIT_ITEM, L"&Quit");
//    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"&File");
//
//    //hSubMenu = CreatePopupMenu();
//    //AppendMenu(hSubMenu, MF_STRING, ID_SHOW_ALL_ITEM, L"Show &All Data");
//    //AppendMenu(hSubMenu, MF_STRING, ID_SELECT_REPORT_ITEM, L"S&eelect report");
//    //AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"&Reports");
//
//    SetMenu(hWnd, hMenu);
}
#else

#endif


/// ---------------------------------------------------------------------------
/// Creates the editors menus
/// ---------------------------------------------------------------------------
void EditorCreateMenus()
{
}
