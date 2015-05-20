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
    prCamera(const Proteus::Math::prVector3 &eye, const Proteus::Math::prVector3 &up, const Proteus::Math::prVector3 &at);
    
    // Method: ~prCamera
    //      Dtor
    virtual ~prCamera();

    // Method: Update
    //      Updates the camera.
    virtual void Update() {}

    // Method: LookAt
    //      Uploads the matrices.
    void LookAt();

    // Method: Rotate
    //      Rotates the camera.
    //
    // Parameters:
    //      x - Amount to rotate the camera in the X direction
    //      y - Amount to rotate the camera in the Y direction
    //      z - Amount to rotate the camera in the Z direction
    void Rotate(Proteus::Core::f32 x, Proteus::Core::f32 y, Proteus::Core::f32 z);
    
    // Method: SetTarget
    //      Sets the camera look at target position
    void SetTarget(Proteus::Math::prVector3 &target);
    
    // Method: SetTarget
    //      Sets the camera look at target position
    void SetTarget(const Proteus::Math::prVector3 &target);
    
    // Method: SetPosition
    //      Sets the cameras position.
    void SetPosition(Proteus::Math::prVector3 &pos);

    // Method: SetPosition
    //      Sets the cameras position.
    void SetPosition(const Proteus::Math::prVector3 &pos);

    // Method: GetTarget
    //      Gets the camera look at target position
    Proteus::Math::prVector3 GetTarget() { return Proteus::Math::prVector3(m_at); }

    // Method: GetPosition
    //      Gets the cameras position.
    Proteus::Math::prVector3 GetPosition() { return Proteus::Math::prVector3(m_eye); }

    // Method: GetID
    //      Gets the cameras unique ID.
    Proteus::Core::s32 GetID() const { return m_id; }

    // Method: GetHash
    //      Gets the cameras hash.
    Proteus::Core::u32 GetHash() const { return m_hash; }

    // Method: GetActive
    //      Gets the cameras active status
    Proteus::Core::PRBOOL GetActive() const { return m_active; }

    // Method: GetName
    //      Gets the cameras name.
    const char *GetName() const { return m_name.Text(); }

    // Method: SetActive
    //      Sets the cameras active status.
    void SetActive(Proteus::Core::PRBOOL state) { m_active = state; }


protected:
    Proteus::Math::prVector3   m_eye;
    Proteus::Math::prVector3   m_up;
    Proteus::Math::prVector3   m_at;


private:
    static Proteus::Core::u32  baseID;


private:
    Proteus::Core::PRBOOL   m_active;
    Proteus::Core::u32      m_hash;
    Proteus::Core::s32      m_id;
    prString                m_name;
};


#endif// __PRCAMERA_H
