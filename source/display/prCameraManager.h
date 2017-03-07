// File: prCameraManager.h
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


#include "../core/prTypes.h"
#include "../core/prList.h"


// Forward declarations
class prCamera;


// Class: prCameraManager
//      Class designed to manage all the cameras.
class prCameraManager
{
public:
    // Method: prCameraManager
    //      Constructor.
    prCameraManager();

    // Method: ~prCameraManager
    //      Constructor.
    ~prCameraManager();

    // Method: Update
    //      Updates the active camera.
    void Update();    
    
    // Method: Add
    //      Adds a camera.
    //
    // Parameters:
    //      cam - The camera to add
    void Add(prCamera* cam);
    
    // Method: Release
    //      Releases a specific camera.
    //
    // Parameters:
    //      cam - The camera to remove
    void Release(prCamera* cam);
    
    // Method: ReleaseAll
    //      Releases all the cameras.
    void ReleaseAll();
    
    // Method: SetActiveCamera
    //      Activate a specific camera.
    //
    // Parameters:
    //      cam - The camera to activate
    void SetActiveCamera(prCamera* cam);
        
    // Method: SetActiveCamera
    //      Activate a specific camera.
    //
    // Parameters:
    //      name - The camera to activate
    void SetActiveCamera(const char *name);
    
    // Method: GetActiveCamera
    //      Returns the active camera.
    prCamera* GetActiveCamera() const { return m_activeCamera; }
        
    // Method: GetCamera
    //      Gets a specific camera.
    prCamera* GetCamera(const char* name);

    // Method: Count
    //      Gets the number of contained cameras.
    Proteus::Core::s32 Count() const { return m_cameras.Size(); }
        
    // Method: DisplayUsage
    //      Displays all the cameras.
    void DisplayUsage();


private:

    prList<prCamera*> m_cameras;    
    prCamera*         m_activeCamera;
};
