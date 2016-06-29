/**
 * prAchievementManager.cpp
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


#include "../prConfig.h"
#include <string>
#include "prAchievementManager.h"
#include "prAchievement_base.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../core/prResourceManager.h"
#include "../core/prRegistry.h"
#include "../display/prSprite.h"
#include "../display/prTexture.h"
#include "../display/prRenderer.h"
#include "../display/prBitmapFont.h"
#include "../display/prSpriteManager.h"
#include "../locale/prLanguage.h"
#include "../persistence/prSave.h"
#include "../tinyxml/tinyxml.h"


// Platform specifics
#if defined(PLATFORM_PC)        
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>
  #include "prAchievement_pc.h"

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>
  #include "prAchievement_ios.h"

#elif defined(PLATFORM_MAC)
  //#include <OpenGLES/ES1/gl.h>
  #include "prAchievement_mac.h"

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include "prAchievement_android.h"

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include "prAchievement_linux.h"

#else
  #error No platform defined.

#endif


using namespace Proteus::Core;
using namespace Proteus::Achievement;


// Defines
//#define ACHIEVEMENT_DEBUG
#define SHOW_ACHIEVEMENT_TIME       3000.0f                 // In milliseconds
#define TEXT_DISPLAY_WIDTH          260.0f
#define AWARDS_TEST_DELAY           60.f                    // One minute delay.


// Enums
enum
{
    MODE_NONE,                                              // Do nothing
    MODE_PRE,                                               // Pre-initialisation step
    MODE_ON,                                                // Bring on achievement
    MODE_WAIT,                                              // Show achievement
    MODE_OFF,                                               // Remove achievement
    MODE_POST,                                              // Okay we're done
};


// Debug assist
#if defined(ACHIEVEMENT_DEBUG)
const char *GetAchievementState(s32 state)
{
    switch(state)
    {
    case Proteus::Achievement::NotAwarded: return "Not Awarded";
    case Proteus::Achievement::Awarding:   return "Awarding";
    case Proteus::Achievement::Awarded:    return "Awarded";
    case Proteus::Achievement::Received:   return "Received";
    }

    return "Unknown";
}
#endif


// Achievement definition.
typedef struct prAchievementDefinition
{
    u32				hash;
    std::string     name;
    std::string     howTo;
    std::string     description;
    std::string     achieved;
    std::string     identifier;
    std::string     image;

} prAchievementDefinition;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prAchievementManager::prAchievementManager(prAchievementProvider provider)
{
    TODO("CHECK FILE NAME/PATH SIZES WITH LINUX")

    mpAchievements                  = nullptr;
    mpLanguage                      = nullptr;
    mpNotificationBar               = nullptr;
    mpNotificationIcon              = nullptr;
    mprAchievementDisplayCallback   = nullptr;
    mIoSuccess                      = false;
    mCorrectFile                    = false;
    mRender                         = false;
    mEnabled                        = true;
    mAchievementsCount              = 0;
    mCheckIndex                     = 0;
    mDisplayMode                    = MODE_NONE;
    mTestTimer                      = AWARDS_TEST_DELAY;
    mDefaultPanelXPos               = 0.0f;
    mDefaultPanelYPos               = 0.0f;
    mRunTime                        = 0.0f;
    
    mpSave          = new prSave();                 // A save class to load/save status data
    mpSpriteManager = new prSpriteManager();        // Our own sprite manager, so they can't be unloaded

#if defined(PLATFORM_PC)
        PRASSERT(provider == Engine || provider == Steam);
        mpAchievementProvider = new prAchievement_PC(provider);
        mpAchievementProvider->Initialise();

#elif defined(PLATFORM_IOS)
        PRASSERT(provider == Engine || provider == GooglePlay);
        mpAchievementProvider = new prAchievement_Ios(provider);
        mpAchievementProvider->Initialise();

#elif defined(PLATFORM_ANDROID)
        PRASSERT(provider == Engine || provider == GooglePlay);
        mpAchievementProvider = new prAchievement_Android(provider);
        mpAchievementProvider->Initialise();

#elif defined(PLATFORM_LINUX)
        PRASSERT(provider == Engine);
        mpAchievementProvider = new prAchievement_Linux(provider);
        mpAchievementProvider->Initialise();

#elif defined(PLATFORM_MAC)
        PRASSERT(provider == Engine);
        mpAchievementProvider = new prAchievement_Mac(provider);
        mpAchievementProvider->Initialise();

#else
        #error No platform defined.

#endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAchievementManager::~prAchievementManager()
{
    PRSAFE_DELETE(mpSave);
    PRSAFE_DELETE(mpAchievementProvider);
    PRSAFE_DELETE(mpAchievements);
    PRSAFE_DELETE(mpSpriteManager);
}


/// ---------------------------------------------------------------------------
/// Update the achievement manager
/// ---------------------------------------------------------------------------
void prAchievementManager::Update(f32 dt)
{
    if (mEnabled)
    {
         PRASSERT(mpAchievementProvider);

         // Update
         mpAchievementProvider->Update(dt);

        // Check if any awards given have been set as awarding, but not awarded.
        if (mpAchievementProvider->IsReady())
        {
            mTestTimer -= dt;

            if (mTestTimer < 0.0f)// && mode == MODE_NONE)
            {
                mTestTimer = AWARDS_TEST_DELAY;

                if (mpAchievements && mAchievementsCount > 0)
                {
                    // If an achievement state is awarding, try to verify awarded state
                    if (mpAchievements[mCheckIndex].state == Status::Awarding)
                    {
                        //mpAchievementProvider->Award(GetIdentifierByIndex(mCheckIndex), 0);
                    }

                    if (++mCheckIndex >= mAchievementsCount)
                    {
                        mCheckIndex = 0;                
                    }
                }
            }
        }

        // Update the render state
        if (mRender)
        {
            PRASSERT(mpSpriteManager);
            PRASSERT(mpNotificationBar);

            switch(mDisplayMode)
            {
            // Do nothing
            case MODE_NONE:
                break;

            // Pre-initialisation step
            case MODE_PRE:
                mRunTime = 0.0f;
                mDisplayMode++;

                // Set panel default position
                mpNotificationBar->pos.x = mDefaultPanelXPos;
                mpNotificationBar->pos.y = mDefaultPanelYPos;

                // Set icons position
                if (mpNotificationIcon)
                {
                    mpNotificationIcon->pos.x = mDefaultPanelXPos;
                    mpNotificationIcon->pos.y = mDefaultPanelYPos;
                    mpNotificationIcon->SetVisible(true);
                }

                // Let the user know if case they want to do something
                if (mprAchievementDisplayCallback)
                    mprAchievementDisplayCallback->AwardDisplayNotification(prAchievementDisplayCallback::BeginDisplay, mpNotificationBar, mRunTime);

                mpNotificationBar->SetVisible(true);
                break;

            // Bring on achievement
            case MODE_ON:
                mRunTime += (dt / 1000.f);
                mRunTime  = PRMIN(mRunTime, 1.0f);
                
                // Let the user know if case they want to do something
                if (mprAchievementDisplayCallback)
                    mprAchievementDisplayCallback->AwardDisplayNotification(prAchievementDisplayCallback::PanelOn, mpNotificationBar, mRunTime);

                if (mRunTime > 0.999f)
                {
                    mDisplayMode++;
                    mRunTime = 0.0f;
                }
                break;

            // Show achievement
            case MODE_WAIT:
                mRunTime += (dt / 1000.f);
                mRunTime  = PRMIN(mRunTime, 3.0f);

                // Let the user know if case they want to do something
                if (mprAchievementDisplayCallback)
                    mprAchievementDisplayCallback->AwardDisplayNotification(prAchievementDisplayCallback::PanelShow, mpNotificationBar, mRunTime);
                
                if (mRunTime > 2.999f)
                {
                    mDisplayMode++;
                    mRunTime = 0.0f;
                }
                break;

            // Remove achievement
            case MODE_OFF:
                mRunTime += (dt / 1000.f);
                mRunTime  = PRMIN(mRunTime, 1.0f);

                // Let the user know if case they want to do something
                if (mprAchievementDisplayCallback)
                    mprAchievementDisplayCallback->AwardDisplayNotification(prAchievementDisplayCallback::PanelOff, mpNotificationBar, mRunTime);
                
                if (mRunTime > 0.999f)
                {
                    mDisplayMode++;
                    mRunTime = 0.0f;
                }
                break;

            // We're done
            case MODE_POST:
                mpNotificationBar->SetVisible(false);

                if (mprAchievementDisplayCallback)
                    mprAchievementDisplayCallback->AwardDisplayNotification(prAchievementDisplayCallback::EndDisplay, nullptr, 0.0f);

                mDisplayMode = MODE_NONE;

                // Release the icon
                mpSpriteManager->Release(mpNotificationIcon);
                mpNotificationIcon = nullptr;
                break;
            }


            // Got anything to render?
            if (mDisplayMode == MODE_NONE && !mRenderList.empty())
            {
                // Get the achievements index.
                auto idx = mRenderList.begin();
                u32 index = (*idx);

                // Remove the first index
                mRenderList.pop_front();

                // Search for and load the icon for this achievement
                auto itr = mAchievementsList.begin();
                auto end = mAchievementsList.end();
                for (u32 currIndex = 0; itr != end; ++itr, currIndex++)
                {
                    if (currIndex == index)
                    {
                        // Create the icon
                        mpNotificationIcon = mpSpriteManager->Create((*itr).image.c_str());
                        if (mpNotificationIcon == nullptr)
                        {
                            prTrace(LogError, "Failed to find achievement icon\n");
                            mDisplayMode = MODE_PRE;
                        }
                        else
                        {
                            mDisplayMode = MODE_PRE;
                            mpNotificationIcon->SetVisible(false);
                        }
                    }
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Render achievement dropdowns if required.
/// ---------------------------------------------------------------------------
void prAchievementManager::Render()
{
    if (mEnabled && mRender)
    {
        if (mpNotificationBar)
        {
            mpNotificationBar->Draw();
        }
        if (mpNotificationIcon)
        {
            mpNotificationIcon->Draw();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Award an achievement
/// ---------------------------------------------------------------------------
void prAchievementManager::Award(const char *key, s32 awardValue)
{
    PRASSERT(mpAchievementProvider);

    if (mEnabled)
    {
        PRASSERT(key && *key);
        PRASSERT(mAchievementsCount > 0);

        u32 hash   = prStringHash(key);
        bool found = false;

        for (s32 i=0; i<mAchievementsCount; i++)
        {
            if (mpAchievements[i].hash == hash)
            {
                found = true;

                if (mpAchievements[i].state == Proteus::Achievement::NotAwarded)
                {
                    mpAchievements[i].count++;

                    if (Achieved(mpAchievements[i].count, awardValue, -1))
                    {
                        // Start the award process.
                        mpAchievementProvider->Award(GetIdentifier(key), 0);
                                
                        // Set award is being awarded.
                        mpAchievements[i].state = Proteus::Achievement::Awarding;
                                
                        // Add to display handler
                        mRenderList.push_back(i);
                    }
                                
                    // Save state change
                    Save();
                }

                break;
            }
        }

        if (!found)
        {
            prTrace(LogError, "prAchievementManager::Award - Failed to find achievement: %s\n", key);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Is an achievement awarded?
/// ---------------------------------------------------------------------------
bool prAchievementManager::IsAwarded(const char *key) const
{
    PRASSERT(key && *key);
    PRASSERT(mAchievementsCount > 0);
    PRASSERT(mpAchievements);

    bool result = false;
    u32 hash    = prStringHash(key);

    for (s32 i=0; i<mAchievementsCount; i++)
    {
        if (mpAchievements[i].hash == hash)
        {
            if (mpAchievements[i].state == Proteus::Achievement::Awarding)
            {
                result = true;
            }
            break;
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Load the achievements.
/// ---------------------------------------------------------------------------
void prAchievementManager::Load(const char *filename)
{
    PRASSERT(filename && *filename);
    PRASSERT(mpSave);

    // Load achievement descriptions.
    LoadAchievementDescriptions(filename);

    // Load the status data.
    s32 size = 0;
    mpSave->StartLoad((void **)&mpAchievements, &size, this, "achievements.dat");    
    do
    {
        mpSave->Update();
    }
    while(mpSave->IsWorking());
    
    // No achievements, then write the default file.
    if (!mIoSuccess)
    {
        if (!mAchievementsList.empty())
        {
            // Create the achievements status list and save it.
            u32 count = (u32)mAchievementsList.size();

            // Set initial states
            prAchievementStatus *alist = new prAchievementStatus[count];
            PRASSERT(alist);
            auto itr = mAchievementsList.begin();
            auto end = mAchievementsList.end();
            s32 index = 0;
            for (; itr != end; ++itr, ++index)
            {
                alist[index].state = Status::NotAwarded;                    // Not awarded
                alist[index].hash  = prStringHash((*itr).name.c_str());     // Name hash
                alist[index].count = 0;                                     // Count times called. Award is given when target count is reached
            }

            // Store.
            mpAchievements = alist;

            // Set number of achievements.
            mAchievementsCount = (u32)mAchievementsList.size();

            // Save
            Save();

            if (!mIoSuccess)
            {
                prTrace(LogError, "Failed to save initial achievements status file.\n");
            }
        }
        else
        {
            prTrace(LogError, "Achievements list is empty\n");
        }
    }
    else
    {
        // Set number of achievements.
        mAchievementsCount = size / sizeof(prAchievementStatus);
        PRASSERT(mAchievementsCount == (s32)mAchievementsList.size());
#if defined(ACHIEVEMENT_DEBUG)
        prTrace(LogError, "Loaded %i achievements\n", mAchievementsCount);
#endif
    }


#if defined(ACHIEVEMENT_DEBUG)
    if (mAchievementsCount > 0)
    {
        for (s32 i=0; i<mAchievementsCount; i++)
        {
            prTrace(LogError, "Name: %*s, Hash %08x, State: %s, Count %i\n", 24, GetNameByIndex(i), mpAchievements[i].hash, GetAchievementState(mpAchievements[i].state), mpAchievements[i].count);
        }
    }
#endif
}


/// ---------------------------------------------------------------------------
/// Saves the achievements states.
/// ---------------------------------------------------------------------------
void prAchievementManager::Save()
{
    PRASSERT(mpSave);

    // No achievements?
    if (mAchievementsCount == 0)
        return;

    // No data?
    if (mpAchievements == nullptr)
        return;
        
    // Save the file.
    if (mpSave)
    {
        mpSave->StartSave(mpAchievements, mAchievementsCount * sizeof(prAchievementStatus), this, "achievements.dat");
        do
        {
            mpSave->Update();
        }
        while(mpSave->IsWorking());
        
        if (!mIoSuccess)
        {
            prTrace(LogError, "Failed to save achievements status file.\n");
        }
    }
}


/// ---------------------------------------------------------------------------
/// This method receives state messages.
/// ---------------------------------------------------------------------------
void prAchievementManager::SaveResult(s32 result)
{
    if (result == IO_RESULT_SUCCESS)
    {
        mIoSuccess = true;
    }
    else
    {
        mIoSuccess = false;
    }
}


/// ---------------------------------------------------------------------------
/// This method receives state messages.
/// ---------------------------------------------------------------------------
void prAchievementManager::LoadResult(s32 result)
{
    if (result == IO_RESULT_SUCCESS)
    {
        mIoSuccess = true;
    }
    else
    {
        mIoSuccess = false;
    }
}


/// ---------------------------------------------------------------------------
/// Gets the description text for the specified entry by type.
/// ---------------------------------------------------------------------------
const char *prAchievementManager::GetDescriptionText(const char *name, DescriptionType type)
{
    PRASSERT(name && *name);

    auto itr = mAchievementsList.begin();
    auto end = mAchievementsList.end();

    u32 hash = prStringHash(name);

    for (; itr != end; ++itr)
    {
        if (hash == (*itr).hash)
        {
            switch(type)
            {
            case DescriptionType::Brief:
                return (*itr).description.c_str();

            case DescriptionType::Achieved:
                return (*itr).achieved.c_str();

            case DescriptionType::HowTo:
                return (*itr).howTo.c_str();
            }
        }
    }

    prTrace(LogError, "prAchievementManager::GetDescriptionText - Failed to find achievement: %s\n", name);
    return nullptr;
}


/// -----------------------------------------------------------------------
/// Gets the description text for the specified entry by type.
/// -----------------------------------------------------------------------
const char *prAchievementManager::GetDescriptionTextByIndex(u32 index, DescriptionType type)
{
    auto itr = mAchievementsList.begin();
    auto end = mAchievementsList.end();

    for (u32 currIndex = 0; itr != end; ++itr, currIndex++)
    {
        if (currIndex == index)
        {
            switch(type)
            {
            case DescriptionType::Brief:
                return (*itr).description.c_str();

            case DescriptionType::Achieved:
                return (*itr).achieved.c_str();

            case DescriptionType::HowTo:
                return (*itr).howTo.c_str();
            }
        }
    }

    prTrace(LogError, "prAchievementManager::GetDescriptionTextByIndex - Failed to find achievement: %i\n", index);
    return nullptr;
}


/// -----------------------------------------------------------------------
/// Get the name of the achievement by index.
/// -----------------------------------------------------------------------
const char *prAchievementManager::GetNameByIndex(u32 index)
{
    auto itr = mAchievementsList.begin();
    auto end = mAchievementsList.end();

    for (u32 currIndex = 0; itr != end; ++itr, currIndex++)
    {
        if (currIndex == index)
        {
            return (*itr).name.c_str();
        }
    }

    return nullptr;
}


/// -----------------------------------------------------------------------
/// Set the image to use as the bar
/// -----------------------------------------------------------------------
void prAchievementManager::SetNotificationBar(const char *filename, NotificationPos position)
{
    PRASSERT(filename && *filename);
    PRASSERT(mpSpriteManager);

    // Create bar
    mpNotificationBar = mpSpriteManager->Create(filename);
    s32 width   = mpNotificationBar->GetFrameWidth();
    s32 height  = mpNotificationBar->GetFrameHeight();

    // X, Y defaults to top/left
    f32 xpos = 0.0f;
    f32 ypos = 0.0f;

    // Get the screens details
    prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
    PRASSERT(pReg);
    f32 scrnWidth  = (f32)atof(pReg->GetValue("ScreenWidth"));
    f32 scrnHeight = (f32)atof(pReg->GetValue("ScreenHeight"));

    // Set Y axis
    if ((position & NotificationPos::Top) == NotificationPos::Top)
    {
        ypos = 0.0f;
    }    
    
    if ((position & NotificationPos::Centre) == NotificationPos::Centre)
    {
        ypos = (scrnHeight / 2) - (height / 2);
    }    
    
    if ((position & NotificationPos::Bottom) == NotificationPos::Bottom)
    {
        ypos = scrnHeight - height;
    }

    // Set X axis
    if ((position & NotificationPos::Left) == NotificationPos::Left)
    {
        xpos = 0.0f;
    }    
    
    if ((position & NotificationPos::Middle) == NotificationPos::Middle)
    {
        xpos = (scrnWidth / 2) - (width / 2);
    }    
    
    if ((position & NotificationPos::Right) == NotificationPos::Right)
    {
        xpos = scrnWidth - width;
    }

    // Set on position
    mpNotificationBar->pos.x = xpos;
    mpNotificationBar->pos.y = ypos;

    // Store default panel position
    mDefaultPanelXPos = xpos;
    mDefaultPanelYPos = ypos;

    // Hide the panel
    mpNotificationBar->SetVisible(false);
}


/// ---------------------------------------------------------------------------
/// Sets the font used.
/// ---------------------------------------------------------------------------
void prAchievementManager::SetNotificationBarFont(prBitmapFont *font, float width, float scale)
{
    PRUNUSED(font);
    PRUNUSED(width);
    PRUNUSED(scale);
    //PRASSERT(pImpl);
    //PRASSERT(font);
    //PRUNUSED(width);
    //imp.pFont = font;
    //imp.fontScale = scale;
}


/// ---------------------------------------------------------------------------
/// Test to see if an achievement has been achieved.
/// ---------------------------------------------------------------------------
bool prAchievementManager::Achieved(s32 value, s32 testValue, s32 type)
{
    PRUNUSED(type);               // In case we add compare types other than '>='

    bool result = false;

    if (value >= testValue)
    {
        result = true;
    }

    return result;
}


/// -----------------------------------------------------------------------
/// Load the XML descriptions of the achievements.
/// -----------------------------------------------------------------------
void prAchievementManager::LoadAchievementDescriptions(const char *filename)
{
    PRASSERT(filename && *filename);

    // Parse the document
    TiXmlDocument *doc = new TiXmlDocument(filename);
    if (doc)
    {
        if (doc->LoadFile())
        {
            ParseFile(doc);
            PRASSERT(mCorrectFile);
#if defined(ACHIEVEMENT_DEBUG)
            prTrace(LogError, "Loaded %i achievement definitions\n", mAchievementsList.size());
#endif
        }
        else
        {
            PRWARN("Failed to Load %s\n", filename);
        }

        delete doc;
    }
}


/// -----------------------------------------------------------------------
/// Parses the xml achievements descriptions file.
/// -----------------------------------------------------------------------
void prAchievementManager::ParseFile(TiXmlNode* pParent)
{
    switch (pParent->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        {
            // File data
            if (strcmp(pParent->Value(), "achievements_file") == 0)
            {
                mCorrectFile = true;
            }
            // Entries
            else if (strcmp(pParent->Value(), "achievement") == 0)
            {
                ParseAttribs_Achievement(pParent->ToElement());
            }
        }
        break;

    default:
        break;
    } 

    for (TiXmlNode *pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        ParseFile(pChild);
    }
}


/// -----------------------------------------------------------------------
/// Attribute parser.
/// -----------------------------------------------------------------------
void prAchievementManager::ParseAttribs_Achievement(TiXmlElement* pElement)
{
    PRASSERT(pElement);
    PRASSERT(pElement->Attribute("name"));
    PRASSERT(pElement->Attribute("desc_brief"));
    PRASSERT(pElement->Attribute("desc_howto"));
    PRASSERT(pElement->Attribute("desc_achieved"));
    PRASSERT(pElement->Attribute("badge"));

    // Get platform specific identifier.
    const char *identifier = nullptr;

    #if defined(PLATFORM_PC)        
        PRASSERT(pElement->Attribute("id_pc"));
        identifier = pElement->Attribute("id_pc");

    #elif defined(PLATFORM_IOS)
        PRASSERT(pElement->Attribute("id_ios"));
        identifier = pElement->Attribute("id_ios");

    #elif defined(PLATFORM_MAC)
        PRASSERT(pElement->Attribute("id_mac"));
        identifier = pElement->Attribute("id_mac");

    #elif defined(PLATFORM_ANDROID)
        PRASSERT(pElement->Attribute("id_android"));
        identifier = pElement->Attribute("id_android");

    #elif defined(PLATFORM_LINUX)
        PRASSERT(pElement->Attribute("id_linux"));
        identifier = pElement->Attribute("id_linux");

    #else
        #error No platform defined.

    #endif


    prAchievementDefinition d;
    d.name          = pElement->Attribute("name");
    d.description   = pElement->Attribute("desc_brief");
    d.howTo         = pElement->Attribute("desc_howto");
    d.achieved      = pElement->Attribute("desc_achieved");
    d.identifier    = identifier;
    d.image			= pElement->Attribute("badge");
    d.hash          = prStringHash(pElement->Attribute("name"));

    mAchievementsList.push_back(d);
}


/// -----------------------------------------------------------------------
/// Gets the identifer for the achievement.
/// -----------------------------------------------------------------------
const char *prAchievementManager::GetIdentifierByIndex(u32 index)
{
    auto itr = mAchievementsList.begin();
    auto end = mAchievementsList.end();

    u32 idx = 0;
    for (; itr != end; ++itr)
    {
        if (idx == index)
        {
            return (*itr).identifier.c_str();
        }

        idx++;
    }

    prTrace(LogError, "prAchievementManager::GetIdentifierByIndex - Failed to find identifier\n");
    return nullptr;
}


/// -----------------------------------------------------------------------
/// Gets the identifer for the achievement.
/// -----------------------------------------------------------------------
const char *prAchievementManager::GetIdentifier(const char *name)
{
    PRASSERT(name && *name);

    if (name && *name)
    {
        auto itr = mAchievementsList.begin();
        auto end = mAchievementsList.end();
        u32 hash = prStringHash(name);

        for (; itr != end; ++itr)
        {
            if (hash == (*itr).hash)
            {
                return (*itr).identifier.c_str();
            }
        }
    }

    prTrace(LogError, "prAchievementManager::GetIdentifier - Failed to find identifier: %s\n", name);
    return nullptr;
}
