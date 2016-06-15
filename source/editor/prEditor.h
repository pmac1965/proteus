// File: prEditor.h
//      Creates a bunch of menus for adjusting the games behaviour
//      at runtime
/**
 * Copyright 2016 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#pragma once


#include "../prConfig.h"
#include "../core/prTypes.h"


// Forward declarations
namespace Proteus { namespace Gui {
    class prGui;
}}

class prBitmapFont;
class prMessageManager;


// Function: EditorCreateGameMenus
//      Creates the default in game menus for the game player
//
// Parameters:
//      pTheGui         - A pointer to a message manager, so the game player knows whats been clicked
//      pMessageManager - A pointer to a GUI created by the game player
//      pBitmapFntDark  - A pointer to the dark font
//      pBitmapFntGrey  - A pointer to the grey font
void EditorCreateGameMenus(Proteus::Gui::prGui *pTheGui, prMessageManager *pMessageManager, prBitmapFont *pBitmapFntDark, prBitmapFont *pBitmapFntGrey);


enum
{
    // File menu
    EDITOR_FILE_EXIT        = 20000,

    // Tool menu
    EDITOR_FPS_ON,
    EDITOR_FPS_OFF,
    EDITOR_FPS_RESET,
    EDITOR_FPS_POSITION,
    EDITOR_COLLISION_DRAW,

    // Profile menu
    EDITOR_PROFILE_ON,
    EDITOR_PROFILE_OFF,

    // Step menu
    EDITOR_STEP_PAUSE,
    EDITOR_STEP_PLAY,
    EDITOR_STEP_STEP,

    // Help menu
    EDITOR_HELP_SHOW,
};
