// File: prCoreSystem.h
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


#include "prTypes.h"
#include "prString.h"


// Class: prCoreSystem
//      The base class used by the engines systems.
//
// Notes:
//        This class must be used to add any core system to the engine core.
class prCoreSystem
{
public:

    // Method: prCoreSystem
    //      Constructor
    //
    // Parameters:
    //      id      - A unique ID which identifies the system.
    //      pName   - The name of the system.
    prCoreSystem(Proteus::Core::u32 id, const char *pName);

    // Method: ~prCoreSystem
    //      Destructor
    virtual ~prCoreSystem();

    // Method: ID
    //      Fetches the core systems unique ID
    Proteus::Core::u32 ID() const { return m_id; }

    // Method: Name
    //      Fetches the core systems name.
    const char *Name() const { return m_name.Text(); }


private:

    Proteus::Core::u32  m_id;           // The systems unique ID.
    prString            m_name;         // The systems name.
};
