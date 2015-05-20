/**
 * prParticleManager.cpp
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


#include "prParticleManager.h"
#include "prEmitter.h"
#include "prParticleShared.h"
#include "../debug/prTrace.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../tinyxml/tinyxml.h"
#include "../core/prString.h"


using namespace Proteus::Core;


// Types
typedef std::map<s32, prEmitter*>::iterator                     prEmitterIt;
typedef std::map<std::string, prEmitterDefinition*>::iterator   prEmitterDefinitionIt;


// Used to give an emitter an ID.
s32 prParticleManager::sEmitterID = 0;


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prParticleManager::prParticleManager()
{
    mCorrectFileType = false;
}


/// ---------------------------------------------------------------------------
/// Destructor
/// ---------------------------------------------------------------------------
prParticleManager::~prParticleManager()
{
    Clear();
}


/// ---------------------------------------------------------------------------
/// Loads the particle emitter definitions
/// ---------------------------------------------------------------------------
bool prParticleManager::Load(const char *filename)
{
    PRASSERT(filename && *filename);

    // Init
    mCorrectFileType = false;

    TiXmlDocument* doc = new TiXmlDocument(filename);    
    bool result = false;
    if (doc)
    {
        bool loaded = doc->LoadFile();
      
        if (loaded)
        {
            ParseParticleFile(doc);
            result = true;
        }
        else
        {
            PRWARN("Failed to Load %s\n", filename);
        }

        delete doc;
    }

    return result;

}


/// ---------------------------------------------------------------------------
/// Clears the particle manager
/// ---------------------------------------------------------------------------
void prParticleManager::Clear()
{
    // Clean the emitters
    if (!mEmitters.empty())
    {
        prEmitterIt it;
        for (it = mEmitters.begin(); it != mEmitters.end(); ++it)
        {
            PRSAFE_DELETE((*it).second);
        }
        //prTrace("Clean emitters\n");
    }

    // Clean the definitions
    if (!mDefinitions.empty())
    {
        prEmitterDefinitionIt it;
        for (it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
        {
            PRSAFE_DELETE((*it).second);
        }
        //prTrace("Clean definitions\n");
    }

    // And clear
    mEmitters.clear();
    mDefinitions.clear();
}


/// ---------------------------------------------------------------------------
/// Updates the particle manager
/// ---------------------------------------------------------------------------
void prParticleManager::Update(f32 dt)
{
    if (!mEmitters.empty())
    {
        prEmitterIt it;
        for (it = mEmitters.begin(); it != mEmitters.end(); ++it)
        {
            prEmitter *pEmitter = (*it).second;
            if (pEmitter)
            {
                if (pEmitter->Update(dt))
                {
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Fires off a particle effect
/// ---------------------------------------------------------------------------
s32 prParticleManager::Fire(const char *name, const Proteus::Math::prVector3 &pos)
{
    s32 handle = -1;

    PRUNUSED(pos);

    if (name && *name && !mDefinitions.empty())
    {
        // Okay, we hashed the name for speed!
        u32 hash = prStringHash(name);


        // Find an emitters definition
        prEmitterDefinitionIt it;
        for (it = mDefinitions.begin(); it != mDefinitions.end(); ++it)
        {
            const prEmitterDefinition *ed = (*it).second;
            if (hash == ed->mHash)
            {
                //prTrace("Found definition\n");
                
                // Ensure emitter is always positive
                sEmitterID++;
                sEmitterID &= 0x0FFFFFFF;

                // Make new emitter
                prEmitter *pEmitter = new prEmitter(sEmitterID, *ed);
                if (pEmitter)
                {
                    handle = pEmitter->GetID();
                    mEmitters.insert(std::pair<s32, prEmitter*>(handle, pEmitter));
                }
                break;
            }
        }
    }


    return handle;
}


/// ---------------------------------------------------------------------------
/// Parses a particle file
/// ---------------------------------------------------------------------------
void prParticleManager::ParseParticleFile(TiXmlNode* pParent)
{
    switch (pParent->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        {
            // File data
            if (strcmp(pParent->Value(), "particle_file") == 0)
            {
                ParseAttribs_ParticleFile(pParent->ToElement());
            }

            // Parse each emitter
            else if (strcmp(pParent->Value(), "emitter") == 0)
            {
                ParseAttribs_Emitter(pParent->ToElement());
            }

            // Parse each particle effect
            else if (strcmp(pParent->Value(), "effect") == 0)
            {
                ParseAttribs_Effect(pParent->ToElement());
            }

    
            // The first element should set the file type.
            // So lets validate it
            if (mCorrectFileType == false)
            {
                PRPANIC("Incorrect file type!");
            }
        }
        break;

    default:
        break;
    } 


    for (TiXmlNode *pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        ParseParticleFile(pChild);
    }
}


/// ---------------------------------------------------------------------------
/// Parses a particle file
/// ---------------------------------------------------------------------------
void prParticleManager::ParseAttribs_ParticleFile(TiXmlElement* pElement)
{
    PRASSERT(pElement);

    if (pElement)
    {
        PRASSERT(pElement->Attribute("version"));
    }

    // Indicate correct file type.
    mCorrectFileType = true;
}


/// ---------------------------------------------------------------------------
/// Parses a particle file
/// ---------------------------------------------------------------------------
void prParticleManager::ParseAttribs_Emitter(TiXmlElement* pElement)
{
    PRASSERT(pElement);

    if (pElement)
    {
        // Sanity checks
        const char *pEmitterName = pElement->Attribute("name");
        PRASSERT(pEmitterName);
        PRASSERT(prStringLength(pEmitterName) > 0);

        
        // Create the emitters definition
        prEmitterDefinition *ed = new prEmitterDefinition(pEmitterName);        


        // Acquire the effects data.
        TiXmlHandle   root(pElement);
        TiXmlElement *pElem  = root.FirstChild("effectType").Element();
        for (; pElem; pElem = pElem->NextSiblingElement())
        {
            const char *pName     = pElem->Attribute("name");
            const char *pCount    = pElem->Attribute("count");
            const char *pWaitTime = pElem->Attribute("waitTime");
            const char *pRuntime  = pElem->Attribute("runTime");

            PRASSERT(pName);
            PRASSERT(pCount);
            PRASSERT(pWaitTime);
            PRASSERT(pRuntime);

            prEffectType et;
            et.mHash     = prStringHash(pName);
            et.mCount    = atoi(pCount);
            et.mRunTime  = (f32)atof(pWaitTime);
            et.mWaitTime = (f32)atof(pRuntime);

            ed->mEffects.push_back(et);
        }

        // Store the definition
        std::string key(pEmitterName);

        if (mDefinitions.find(key) == mDefinitions.end())
        {
            mDefinitions.insert
            (
                std::pair<std::string, prEmitterDefinition*>(key, ed)
            );
        }
        else
        {
            PRPANIC("Multiple emitter defintions with the same name is not allowed");
        }
    }
}


/// ---------------------------------------------------------------------------
/// Parses a particle file
/// ---------------------------------------------------------------------------
void prParticleManager::ParseAttribs_Effect(TiXmlElement* pElement)
{
    PRASSERT(pElement);
    if (pElement)
    {
    }
}
