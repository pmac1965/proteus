/**
 * prCameraManager.cpp
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


#include "prCameraManager.h"
#include "prCamera.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prStringUtil.h"
#include "../core/prDefines.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prCameraManager::prCameraManager()
{
    m_activeCamera = 0;
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prCameraManager::~prCameraManager()
{
    ReleaseAll();
}


/// ---------------------------------------------------------------------------
/// Updates the active camera.
/// ---------------------------------------------------------------------------
void prCameraManager::Update()
{
    if (m_activeCamera)
    {
        m_activeCamera->Update();
    }
}


/// ---------------------------------------------------------------------------
/// Adds a camera.
/// ---------------------------------------------------------------------------
void prCameraManager::Add(prCamera* cam)
{
    PRASSERT(cam);
    m_cameras.AddTail(cam);
}

   
/// ---------------------------------------------------------------------------
/// Releases a specific camera.
/// ---------------------------------------------------------------------------
void prCameraManager::Release(prCamera* cam)
{
    PRASSERT(cam);
    
    prList<prCamera*>::prNode *node = m_cameras.Find(cam);
    
    if (node)
    {
        delete node->item;
        
        m_cameras.Remove(node);
                        
        // Do we need to reset the active camera?
        if (m_cameras.Size() == 0 || m_activeCamera == cam)
        {
            m_activeCamera = 0;
        }
    }
    else
    {
        PRWARN("Attempted to release a camera thats not in the camera manager");
    }
}


/// ---------------------------------------------------------------------------
/// Releases all the cameras.
/// ---------------------------------------------------------------------------
void prCameraManager::ReleaseAll()
{
    prList<prCamera*>::prIterator it = m_cameras.Begin();
    
    while(it.Okay())
    {
        delete *it;        
        ++it;
    }

    m_activeCamera = 0;
}
    

/// ---------------------------------------------------------------------------
/// Activate a specific camera
/// ---------------------------------------------------------------------------
void prCameraManager::SetActiveCamera(prCamera* cam)
{
    PRASSERT(cam);
        
    // Early out if camera has already been set.
    if (m_activeCamera == cam)
    {
        return;
    }


    // Lets look through the camera list.
    prList<prCamera*>::prIterator it = m_cameras.Begin();    
    while(it.Okay())
    {
        if (cam == *it)
        {
            // We need to set all the other cameras as inactive.
            prList<prCamera*>::prIterator iterator = m_cameras.Begin();            
            while(iterator.Okay())
            {
                (*iterator)->SetActive(false);
                ++iterator;
            }
                        
            // Now set this camera as active.
            cam->SetActive(true);
            m_activeCamera = cam;                                                
            return;
        }
        
        ++it;
    }
    
    PRWARN("Failed to set active camera");
}
    
 
/// ---------------------------------------------------------------------------
/// Activate a specific camera
/// ---------------------------------------------------------------------------
void prCameraManager::SetActiveCamera(const char *name)
{
    PRASSERT(name && *name);

    u32 hash = prStringHash(name);

    prList<prCamera*>::prIterator it = m_cameras.Begin();    
    while(it.Okay())
    {
        if (hash == (*it)->GetHash())
        {
            SetActiveCamera(*it);
            return;
        }
        
        ++it;
    }
    
    PRWARN("Failed to set active camera");
}


/// ---------------------------------------------------------------------------
/// Gets a specific camera.
/// ---------------------------------------------------------------------------
prCamera* prCameraManager::GetCamera(const char* name)
{
    PRASSERT(name && *name);
    
    u32 hash = prStringHash(name);
    
    prList<prCamera*>::prIterator it = m_cameras.Begin();    
    while(it.Okay())
    {
        if (hash == (*it)->GetHash())
        {
            return *it;
        }
    }
    
    return 0;
}


/// ---------------------------------------------------------------------------
/// Displays all the cameras.
/// ---------------------------------------------------------------------------
void prCameraManager::DisplayUsage()
{
#if defined(_DEBUG) || defined(DEBUG)

    prTrace(prLogLevel::LogError, "\n");
    prTrace(prLogLevel::LogError, "Camera manager: =========================================================================\n");

    prList<prCamera*>::prIterator it = m_cameras.Begin();    
    while(it.Okay())
    {
        prCamera *cam = *it;
        
        prTrace
        (
            prLogLevel::LogError,
            "Camera ID %*i, Active: %s, Hash 0x%08x, Name: %s\n", 
            4, cam->GetID(), 
            cam->GetActive() ? "true " : "false",
            cam->GetHash(),
            cam->GetName()          
        );
        
        ++it;
    }
    
    prTrace(prLogLevel::LogError, "Cameras: %i\n", m_cameras.Size());
    prTrace(prLogLevel::LogError, "=========================================================================================\n");

#endif
}
