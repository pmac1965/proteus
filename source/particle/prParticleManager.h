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


#ifndef __PRPARTICLEMANAGER_H
#define __PRPARTICLEMANAGER_H


// Forward declarations
class prEmitter;
class prVector3;
class TiXmlNode;
class TiXmlElement;
class prEmitterDefinition;


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

    // Method: Update
    //      Updates the particle manager
    void Update(f32 dt);

    // Method: Fire
    //      Fires off a particle effect
    prEmitter *Fire(const char *name, prVector3 &pos);


private:
    void Add();

    // Parses a particle file.
    void ParseParticleFile(TiXmlNode* pParent);

    // Parses a particle file.
    void ParseAttribs_ParticleFile(TiXmlElement* pElement);

    // Parses a particle file.
    void ParseAttribs_Emitter(TiXmlElement* pElement);


private:
    // The active emitters
    std::map<u32, prEmitter*>                       mEmitters;

    // The definitions of the emitters
    std::map<std::string, prEmitterDefinition*>     mDefinitions;

    bool    mCorrectFileType;
};


#endif//__PRPARTICLEMANAGER_H
