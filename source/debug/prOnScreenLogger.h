// File: prOnScreenLogger.h
/*
    Copyright 2014 Paul Michael McNab

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef __PRONSCREENLOGGER_H
#define __PRONSCREENLOGGER_H


//#include <string>
#include <list>
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"
#include "../display/prFixedWidthFont.h"


// Forward declarations
class prTexture;


// Defines
#define MAX_MESSAGE_COUNT   64


// Class: prOnScreenLogger
//      Simple on screen logging class.
//      Useful when the debugger isn't very good, or for when you want to
//      log on test devices
class prOnScreenLogger  : public prCoreSystem
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
    void Draw(u32 xpos, u32 ypos, prFixedWidthFont::prFixedWidthAlignment alignment);

    // Method: Clear
    //      Remove all the messages.
    void Clear();

    // Method: Count
    //      Returns the number of messages.
    s32 Count();


private:

    prTexture          *m_pTexture;
    prFixedWidthFont   *m_pFixedWidthFont;
    std::list<char *>   m_messages;
};


#endif//__PRONSCREENLOGGER_H
