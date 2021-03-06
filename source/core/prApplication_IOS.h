// File: prApplication_Ios.h
//      The interface class for all iOS applications.
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


#include "../prConfig.h"


#if defined(PLATFORM_IOS)


#include "prTypes.h"
#include "prApplication.h"


// Class: prApplication_Ios
//      Application class for ios
//
// Notes:
//      Overrides DisplayCreate and Run
//
// See Also:
//      <DisplayCreate>
//      <Run>
class prApplication_Ios : public prApplication
{
public:
    // Method: prApplication_Ios
    //      Ctor    
    prApplication_Ios();

    // Method: ~prApplication_Ios
    //      Dtor    
    virtual ~prApplication_Ios() {}

    // Method: DisplayCreate
    //      Creates the application display.
    //
    // Parameters:
    //      width       - Display width.
    //      height      - Display height.
    //      pWindowName - The name of the window, which will only appear in windowed apps
    //      fullscreen  - Create a full screen application if possible
    //
    // Notes:
    //      This call will destroy the previous display if it exists.
    //
    // Returns:
    //      PRTRUE if the display was created successfully.
    //      PRFALSE if the display was NOT created successfully.
    PRBOOL DisplayCreate(u32 width, u32 height, const char *pWindowName, bool fullscreen) override;

    // Method: Run
    //      Runs the game.
    //
    // Notes:
    //      This function will the run the game continuously, and
    //      will not exit until the game is complete.
    //
    // Returns:
    //      Will return PRFALSE when the game is complete.
    PRBOOL Run() override;


private:
    const char *BuildType();
};


#endif//PLATFORM_IOS
