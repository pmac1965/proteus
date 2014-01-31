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


#ifndef __PRCORE_H
#define __PRCORE_H


#include "prTypes.h"


// Forward declarations
class prCoreSystem;


/// @enum   prRendererType
/// @brief  The types of renderer supported
typedef enum prRendererType
{
    PRRENDERER_OPENGL,              ///< Use OpenGL
    PRRENDERER_DIRECTX,             ///< Use DirectX

} prRendererType;


/// @enum   prVerType
/// @brief  The version of renderer required
typedef enum prVerType
{
    PRGLVER_11,                     ///< OpenGL Version 1.1
    PRGLVER_20,                     ///< OpenGL Version 2.0
    PRGLVER_30,                     ///< OpenGL Version 3.0
    PRDXVER_9,                      ///< DirectX Version 9
    PRDXVER_10,                     ///< DirectX Version 10
    PRDXVER_11,                     ///< DirectX Version 11

} prVerType;


/// @enum   prSystems
/// @brief  Also contains all the optional systems
enum prSystems
{
    // Core systems
    PRSYSTEM_RENDERER,                              ///< The rendering system. (Core system)
    PRSYSTEM_TOUCH,                                 ///< The touch system. (Core system)
    PRSYSTEM_MESSAGEMANAGER,                        ///< The message passing system. (Core system)
    PRSYSTEM_REGISTRY,                              ///< The registry system. (Core system)
    PRSYSTEM_RESOURCEMANAGER,                       ///< The resource handling system. (Core system)
    PRSYSTEM_FILEMANAGER,                           ///< The file system. (Core system)

#if defined(PLATFORM_PC)
    PRSYSTEM_MOUSE,                                 ///< Mouse system. (Core system for Desktops)
    PRSYSTEM_KEYBOARD,                              ///< Keyboard system. (Core system for Desktops)
#endif

    // Optional systems
    PRSYSTEM_FPS,                                   ///< FPS counter.
    PRSYSTEM_AUDIO,                                 ///< The sound system.
    PRSYSTEM_ONSCREENLOGGER,                        ///< The on screen logger.
    PRSYSTEM_FADEMANAGER,                           ///< The fade manager.
    PRSYSTEM_GUIMANAGER,                            ///< The GUI manager.
    PRSYSTEM_BACKGROUNDMANAGER,                     ///< The background manager.
    PRSYSTEM_SPRITEMANAGER,                         ///< The sprite manager.
    PRSYSTEM_MAX,                                   ///< -- KEEP THIS LAST --

    // Helpers
    PRSYSTEM_OPTIONAL_MIN   = PRSYSTEM_FPS,         ///< Optional min
    PRSYSTEM_OPTIONAL_MAX   = PRSYSTEM_MAX - 1,     ///< Optional max
    PRSYSTEM_LISTEND        = -1,                   ///< List end marker
};


/// @brief      Creates the engine core components
/// @param      rendererType - The type of renderer. DirectX or OpenGL
/// @param      version - Which version of the renderer
/// @note       This function must be the first call made during game construction
/// @see        prRendererType
/// @see        prVerType
/// @return     PRTRUE if the core was constructed, PRFALSE if the core already exists 
PRBOOL prCoreCreate(prRendererType rendererType, prVerType version);


/// @brief      Creates optional engine core components
/// @param      optionalSystems - A pointer to an array of the systems to create
/// @note       This function should called just after prCoreCreate
/// @see        prCoreCreate
void prCoreCreateOptional(s32 *optionalSystems);


/// @brief      Destroys the engine core.
/// @note       This is handled by the engine. Do not call
void prCoreDestroy();


/// @brief      Does the core exist.
/// @return     PRTRUE or PRFALSE
PRBOOL prCoreExist();


/// @brief      Does a core component exist
/// @param      systemID - The unique ID of the system
/// @note       The core must exist as well for this function to work
/// @return     PRTRUE or PRFALSE
PRBOOL prCoreComponentExist(s32 systemID);


/// @brief      Fetches a core system component pointer, Includes optional 
/// @n          components.
/// @param      systemID - The unique ID of the system
/// @note       The core must exist for this function to work correctly
/// @return     A prCoreSystem pointer.
/// @return     May return NULL.
prCoreSystem *prCoreGetComponent(u32 systemID);


#endif//__PRCORE_H
