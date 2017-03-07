// File: prBackgroundManager.h
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


#include "../core/prCoreSystem.h"


// Forward declarations.
class prBackground;


#define MAX_BACKGROUNDS     8


// Class: prBackgroundManager
//      Background management class
//
// Notes:
//      This class is used to create and destroy backgrounds.
//      You must use this class to create backgrounds, as they
//      cannot be created without it.
class prBackgroundManager : public prCoreSystem
{
public:
    // Method: prBackgroundManager
    //      Constructs the background manager.
    prBackgroundManager();

    // Method: ~prBackgroundManager
    //      Destructor.
    ~prBackgroundManager();

    // Method: Create
    //      Creates a background.
    //
    // Parameters:
    //      filename - Name of the background to load
    //
    // Returns:
    //      A constructed background or NULL
    prBackground *Create(const char *filename);

    // Method: Release
    //      Releases a background and any asssociated assets.
    //
    // Parameters:
    //      bg - The background to release
    void Release(prBackground *bg);
    
    // Method: ReleaseAll
    //      Releases all backgrounds and their asssociated assets. 
    void ReleaseAll();

    // Method: DisplayUsage
    //      Shows all the assets being used by the background manager.
    void DisplayUsage();

private:
    // Stops passing by value and assignment.
    prBackgroundManager(const prBackgroundManager&);
    const prBackgroundManager& operator = (const prBackgroundManager&);

private:
    prBackground   *backgrounds[MAX_BACKGROUNDS];
};
