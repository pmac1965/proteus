// File: prParticleManager.h
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


// Forward declarations
class prEmitter;
namespace Proteus
{
    namespace Math
    {
        class prVector3;
    }
}

class TiXmlNode;
class TiXmlElement;
struct prEmitterDefinition;


#include <list>
#include <map>
#include <string>
#include "../core/prTypes.h"


// Class: prParticleManager
//      This class is responsible for creating and destroying particle emitters
class prParticleManager
{
public:
    // Method: prParticleManager
    //      Constructor.
    prParticleManager();

    // Method: ~prParticleManager
    //      Destructor.
    ~prParticleManager();

    // Method: Load
    //      Loads the particle emitter definitions
    //
    // Parameters:
    //      filename - An xml file describing the emitter types
    bool Load(const char *filename);

    // Method: Clear
    //      Clears the particle manager of all definitions
    void Clear();

    // Method: Update
    //      Updates the particle manager
    void Update(f32 dt);

    // Method: Fire
    //      Fires off a particle effect
    //
    // Return:
    //      Returns an ID for the emitter, or -1 on failure
    s32 Fire(const char *name, const Proteus::Math::prVector3 &pos);

    // Method: GetEmitterDefinitionCount
    //      Returns the number of emitter definitions
    //
    // Notes:
    //      If you have a defintion called 'smokey' and a definition called 'fireball', 
    //      the returned count would be two.
    //
    //      *PLEASE REMEMBER THIS IS THE COUNT OF EMITTER TYPES, NOT ACTIVE EMITTERS*
    s32 GetEmitterDefinitionCount() const;

    //
    //
    prEmitterDefinition *GetEmitterDefinitionByIndex(s32 index);


private:
    static s32 sEmitterID;                                              // Used to give emitters a unique ID


private:
    // Parses a particle file.
    void ParseParticleFile(TiXmlNode* pParent);

    // Parses a particle file.
    void ParseAttribs_ParticleFile(TiXmlElement* pElement);

    // Parses a particle file.
    void ParseAttribs_Emitter(TiXmlElement* pElement);

    // Parses a particle file.
    void ParseAttribs_Effect(TiXmlElement* pElement);


private:    
    std::map<s32, prEmitter*>                       mEmitters;          // The active emitters
    std::map<std::string, prEmitterDefinition*>     mDefinitions;       // The definitions of the emitters
    bool                                            mCorrectFileType;   // Used to ensure its the correct file type during loading.
};
