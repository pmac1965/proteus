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
 *
 */


#include "prParticleManager.h"
#include "prEmitter.h"
#include "../debug/prTrace.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../tinyxml/tinyxml.h"
#include "../core/prString.h"


#define PARTICLE_MANAGER_DEBUG


// This struct is used to hold an effect type for a defined emitter
struct prEffectType
{
    #if defined(PARTICLE_MANAGER_DEBUG)
    prString    mTypeName;
    #endif
    u32         mHash;
    s32         mCount;
    f32         mWaitTime;
    f32         mRunTime;
};


// Types
typedef std::list<prEffectType>             prEffectTypeList;
typedef std::list<prEffectType>::iterator   prEffectTypeListIt;


// This class is used to hold the definition data of a particle emitter
class prEmitterDefinition
{
public:
    // Ctor
    prEmitterDefinition(const char *name) : mHash(prStringHash(name))
    {
        #if defined(PARTICLE_MANAGER_DEBUG)
        mDefName.Set(name);
        prTrace("New 'prEmitterDefinition' %s - %08x\n", mDefName.Text(), mHash);
        #endif
    }

    
    // Debug assist
    void DisplayUsage()
    {
        #if (defined(PARTICLE_MANAGER_DEBUG) && (defined(DEBUG) || defined(_DEBUG)))
        prEffectTypeListIt it;

        for (it = mEffects.begin(); it != mEffects.end(); ++it)
        {
            prEffectType &et = (*it);
            prTrace("Effect name    : %s\n", et.mTypeName.Text());
            prTrace("Effect count   : %i\n", et.mCount);
            prTrace("Effect hash    : %x\n", et.mHash);
            prTrace("Effect waitTime: %f\n", et.mWaitTime);
            prTrace("Effect runTime : %f\n", et.mRunTime);
            prTrace("\n");
        }
        #endif
    }


    prEffectTypeList    mEffects;
    u32                 mHash;

    #if defined(PARTICLE_MANAGER_DEBUG)
    prString            mDefName;
    #endif
};



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
/// Updates the particle manager
/// ---------------------------------------------------------------------------
void prParticleManager::Update(f32 dt)
{
}


/// ---------------------------------------------------------------------------
/// Fires off a particle effect
/// ---------------------------------------------------------------------------
prEmitter *prParticleManager::Fire(const char *name, prVector3 &pos)
{
    prEmitter *pEmitter = NULL;

    if (name && *name && !mEmitters.empty())
    {
        u32 hash = prStringHash(name);

        // Find an emitters definition
        std::map<u32, prEmitter*>::iterator it;

        for (it = mEmitters.begin(); it != mEmitters.end(); ++it)
        {
            if (hash == (*it).first)
            {
                prTrace("Found definition\n");
                break;
            }
        }

        //// Make new emitter
        //if (pEmitter)
        //{
        //    pEmitter->mPos = pos;
        //}

        // Add to emitter list
    }

    return pEmitter;//fired;
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
                //ParseAttribs_Sequence(pParent->ToElement());
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

            #if defined(PARTICLE_MANAGER_DEBUG)
            et.mTypeName.Set(pName);
            #endif

            ed->mEffects.push_back(et);
        }

        ed->DisplayUsage();
        PRSAFE_DELETE(ed);
    }
}
