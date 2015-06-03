// File: prOnScreenLogger.h
/**
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


#ifndef __PRONSCREENLOGGER_H
#define __PRONSCREENLOGGER_H


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"
#include "../display/prFixedWidthFont.h"
#include <list>


// Forward declarations
class prTexture;


// Defines
#define MAX_MESSAGE_COUNT   64


// Removable helper
#if !defined(REMOVE_OSL)
  #if defined(PLATFORM_PC)
    #define prLogOsl(text, ...) {                                                                   \
        prOnScreenLogger *pOSL = (prOnScreenLogger *)prCoreGetComponent(PRSYSTEM_ONSCREENLOGGER);   \
        if (pOSL) { pOSL->Add(text, __VA_ARGS__); }                                                 \
    }

  #else    
    #define prLogOsl(text, args...) {                                                               \
        prOnScreenLogger *pOSL = (prOnScreenLogger *)prCoreGetComponent(PRSYSTEM_ONSCREENLOGGER);   \
        if (pOSL) { pOSL->Add(text, ## args); }                                                     \
    }

  #endif

#else
    #define prLogOsl(text, ...)

#endif


// Class: prOnScreenLogger
//      Simple on screen logging class.
//      Useful when the debugger isn't very good, or for when you want to
//      log on test devices
class prOnScreenLogger : public prCoreSystem
{
public:
    // Method: prOnScreenLogger
    //      Ctor
    prOnScreenLogger();

    // Method: ~prOnScreenLogger
    //      Dtor
    ~prOnScreenLogger();

    // Method: Add
    //      Adds a message.
    void Add(const char *message, ...);

    // Method: Draw
    //      Draw all the messages.
    //
    // Parameters:
    //      xpos      - The initial x position
    //      ypos      - The initial y position
    //      alignment - Text alignment
    //
    // See Also:
    //      <prFixedWidthFont::prFixedWidthAlignment>
    void Draw(Proteus::Core::u32 xpos, Proteus::Core::u32 ypos, prFixedWidthFont::prFixedWidthAlignment alignment);

    // Method: Clear
    //      Remove all the messages.
    void Clear();

    // Method: Count
    //      Returns the number of messages.
    Proteus::Core::s32 Count() const;

    // Method: SetEnabled
    //      Determines if logger accepts and prints messages.
    void SetEnabled(bool state) { m_enabled = state; } 

    // Method: GetEnabled
    //      Returns the enabled state.
    bool GetEnabled() const { return m_enabled; }


private:
    prTexture          *m_pTexture;
    prFixedWidthFont   *m_pFixedWidthFont;
    bool                m_enabled;
    bool                m_exp0;
    bool                m_exp1;
    bool                m_exp2;
    std::list<char *>   m_messages;
};


#endif//__PRONSCREENLOGGER_H
