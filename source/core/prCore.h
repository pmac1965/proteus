// File: prCore.h
/**
 * Copyright 2014 Paul Michael McNab
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


#include "prTypes.h"


// Forward declarations
class prCoreSystem;


// Enum: prRendererType
//      The types of renderer supported
//
//  PRRENDERER_OPENGL   - Use OpenGL
//  PRRENDERER_VULKAN   - Use Vulkan
typedef enum
{
    PRRENDERER_OPENGL,
	PRRENDERER_VULKAN

} prRendererType;


// Enum: prVerType
//      The version of the renderer required
//
//  PRGLVER_11  - OpenGL Version 1.1
//  PRGLVER_20  - OpenGL Version 2.0
//  PRGLVER_30  - OpenGL Version 3.0 or above
typedef enum
{
    PRGLVER_11,
    PRGLVER_20,
    PRGLVER_30,

} prVerType;


// Enum: prSystems
//      Contains core systems as well the optional systems
//
//  PRSYSTEM_RESOURCEMANAGER       - The resource handling system. (Core system)
//  PRSYSTEM_RENDERER              - The rendering system. (Core system)
//  PRSYSTEM_TOUCH                 - The touch system. (Core system)
//  PRSYSTEM_MESSAGEMANAGER        - The message passing system. (Core system)
//  PRSYSTEM_REGISTRY              - The registry system. (Core system)
//  PRSYSTEM_FILEMANAGER           - The file system. (Core system)
//  PRSYSTEM_MOUSE                 - Mouse system. (Core system for Desktops)
//  PRSYSTEM_KEYBOARD              - Keyboard system. (Core system for Desktops)
//  PRSYSTEM_FPS                   - FPS counter.
//  PRSYSTEM_AUDIO                 - The sound system.
//  PRSYSTEM_ONSCREENLOGGER        - The on screen logger.
//  PRSYSTEM_FADEMANAGER           - The fade manager.
//  PRSYSTEM_GUIMANAGER            - The GUI manager.
//  PRSYSTEM_BACKGROUNDMANAGER     - The background manager.
//  PRSYSTEM_SPRITEMANAGER         - The sprite manager.
//  PRSYSTEM_ACCELEROMETER         - The accelerometer.
//  PRSYSTEM_FONTMANAGER           - The TTF font manager    
//  PRSYSTEM_MAX                   - -- KEEP THIS LAST --
enum prSystems
{
    // Core systems
    PRSYSTEM_RESOURCEMANAGER,  
    PRSYSTEM_TOUCH,            
    PRSYSTEM_MESSAGEMANAGER,   
    PRSYSTEM_REGISTRY,         
    PRSYSTEM_FILEMANAGER,      
    PRSYSTEM_RENDERER,         

    // Desktop only
#if defined(PLATFORM_PC)
    PRSYSTEM_MOUSE,            
    PRSYSTEM_KEYBOARD,         

    // Desktop only
#elif defined(PLATFORM_LINUX)
    PRSYSTEM_MOUSE,
    PRSYSTEM_KEYBOARD,
#endif

    // Optional systems
    PRSYSTEM_FPS,              
    PRSYSTEM_AUDIO,            
    PRSYSTEM_ONSCREENLOGGER,   
    PRSYSTEM_FADEMANAGER,      
    PRSYSTEM_GUIMANAGER,       
    PRSYSTEM_BACKGROUNDMANAGER,
    PRSYSTEM_SPRITEMANAGER,    
    PRSYSTEM_ACCELEROMETER,    
    PRSYSTEM_FONTMANAGER,    
    PRSYSTEM_LEADERBOARDS,
    PRSYSTEM_LAYERMANAGER,
    PRSYSTEM_MAX,                   // -- KEEP THIS LAST --      
};


// Function: prCoreInit
//      Initialises the engine core components
//
// Parameters:
//      saveDataPath - The name of a folder
//
// Notes:
//      These are;
//      - The resource manager
//      - Touch support
//      - The message manager
//      - The registry
//      - The file manager
//      - The mouse (If available on platform)
//      - The keyboard (If available on platform)
//
// Notes:
//      *THIS FUNCTION MUST BE CALLED FIRST*
//
// Notes:
//      If the system requires a folder name, then it should be supplied.
//      Normally this would be the games name. Please remember to change for
//      different game versions. e.g; "game_lite" or "game_free"
//
// Notes:
//      After this function you will still need to specify a renderer, as
//      strictly speaking a renderer isn't a requirement for some tools
//
// Notes:
//      A save folder name for saveDataPath must be supplied, as
//      it is expected that all games will save data.
//
// See Also:
//      <prCoreSetRenderer>
void prCoreInit(const char *saveDataPath);


// Function: prCoreSetRenderer
//      Sets the engine renderer
//
// Parameters:
//      rendererType - The type of renderer. DirectX or OpenGL
//      version      - Which version of the renderer
//
// Notes:
//      This function must be called before you create a game window
//
// See Also:
//      <prRendererType>
//      <prVerType>
// 
// Returns:
//      PRTRUE if the core was constructed
//      PRFALSE if the core already exists 
PRBOOL prCoreSetRenderer(prRendererType rendererType, prVerType version);


// Function: prCoreCreateOptional
//      Creates optional engine core components
//
// Parameters:
//      optionalSystems - A pointer to an array of the systems to create
//      count           - The number of elements
//
// Notes:
//      This function should called just after prCoreInit
//
// See Also:
//      <prCoreInit>
void prCoreCreateOptional(s32 *optionalSystems, u32 count);


// Function: prCoreDestroy
//      Destroys the engine core.
//
// Notes:
//      This is handled by the engine. Do not call
void prCoreDestroy();


// Function: prCoreExist
//      Does the core exist.
//
// Returns:
//      PRTRUE or PRFALSE
PRBOOL prCoreExist();


// Function: prCoreComponentExist
//      Does a core component exist
//
// Parameters:
//      systemID - The unique ID of the system
//
// Notes:
//      The core must exist as well for this function to work
//
// Returns:
//      PRTRUE or PRFALSE
PRBOOL prCoreComponentExist(s32 systemID);


// Function: prCoreGetComponent
//      Fetches a core system component pointer, Includes optional components
//
// Parameters:
//      systemID - The unique ID of the system
//
// Notes:
//      The core must exist for this function to work correctly
// 
// Returns:
//      A prCoreSystem pointer.
//      May return nullptr
prCoreSystem *prCoreGetComponent(u32 systemID);
