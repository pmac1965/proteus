// File: prCamera.h
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


#ifndef __PRCAMERA_H
#define __PRCAMERA_H


#include "../core/prTypes.h"
#include "../math/prVector3.h"
#include "../core/prString.h"


// Class: prCamera
//      Base camera class
class prCamera
{
public:
    // Method: prCamera
    //      Ctor
    prCamera();

    // Method: prCamera
    //      Ctor
    //
    // Parameters:
    //      eye     - Camera vector
    //      up      - Camera vector
    //      at      - Camera vector
    prCamera(prVector3 &eye, prVector3 &up, prVector3 &at);
    
    // Method: ~prCamera
    //      Dtor
    virtual ~prCamera();

    // Method: Update
    //      Updates the camera.
    virtual void Update() {}

    // Method: LookAt
    //      Uploads the matrices.
    void LookAt();
    
    // Method: SetTarget
    //      Sets the camera look at target position
    void SetTarget(prVector3 &target);
    
    // Method: SetTarget
    //      Sets the camera look at target position
    void SetTarget(const prVector3 &target);
    
    // Method: GetTarget
    //      Gets the camera look at target position
    prVector3 GetTarget();

    // Method: SetPosition
    //      Sets the cameras position.
    void SetPosition(prVector3 &pos);

    // Method: SetPosition
    //      Sets the cameras position.
    void SetPosition(const prVector3 &pos);

    // Method: GetPosition
    //      Gets the cameras position.
    prVector3 GetPosition();

    // Method: GetID
    //      Gets the cameras unique ID.
    s32 GetID() const { return m_id; }

    // Method: GetHash
    //      Gets the cameras hash.
    u32 GetHash() const { return m_hash; }

    // Method: GetActive
    //      Gets the cameras active status
    PRBOOL GetActive() const { return m_active; }

    // Method: GetName
    //      Gets the cameras name.
    const char *GetName() const { return m_name.Text(); }

    // Method: SetActive
    //      Sets the cameras active status.
    void SetActive(PRBOOL state) { m_active = state; }


protected:

    prVector3   m_eye;
    prVector3   m_at;
    prVector3   m_up;


private:

    static u32  baseID;

    PRBOOL      m_active;
    u32         m_hash;
    s32         m_id;

    prString    m_name;
};


#endif// __PRCAMERA_H
