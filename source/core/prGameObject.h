// File: prGameObject.h
/**
 * Copyright 2016 Paul Michael McNab
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
#include "prString.h"
#include "../editor/prEditorObject.h"
//#include "../math/prVector3.h"

//
//
class prGameObject PROT_EDITOR_INCL
{
public:
    prGameObject() PROT_EDITOR_CTOR
    {}

    virtual ~prGameObject() 
    {}

    //
    void Update() {}

    //
    void Draw() {}

    //
    inline Proteus::Core::PRBOOL IsActive() const { return mActive; }
    inline Proteus::Core::PRBOOL IsEnabled() const { return mEnabled; }
    inline Proteus::Core::PRBOOL IsVisible() const { return mVisible; }

    //
    inline void SetActive(Proteus::Core::PRBOOL active) { mActive = active; }
    inline void SetEnabled(Proteus::Core::PRBOOL active) { mEnabled = active; }
    inline void SetVisible(Proteus::Core::PRBOOL active) { mVisible = active; }

    //
    void AddComponent() {}

    //
    void FindComponentByName() {}

    //
    void FindComponentByType() {}

    //
    void FindComponentByTag() {}

    //
    void DestroyComponent() {}

    // serialize out
    void Save();

    // serialize in
    void Load();

    
private:
    prString                mName;
    prString                mTag;
    Proteus::Core::PRBOOL   mActive;
    Proteus::Core::PRBOOL   mEnabled;
    Proteus::Core::PRBOOL   mVisible;
};
