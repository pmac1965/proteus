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
#include <list>
#include <string>
#include "prAchievementManager.h"
#include "prAchievement_base.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../core/prResourceManager.h"
#include "../display/prSprite.h"
#include "../display/prTexture.h"
#include "../display/prRenderer.h"
#include "../display/prBitmapFont.h"
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

//#elif defined(PLATFORM_BADA)
//  #include <FGraphicsOpengl.h>
//  #include "prAchievement_bada.h"
//  using namespace Osp::Graphics::Opengl;

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


// Defines
#define ACHIEVEMENT_DEBUG
#define SHOW_ACHIEVEMENT_TIME       3000.0f                 // In milliseconds
#define TEXT_DISPLAY_WIDTH          260.0f
#define AWARDS_TEST_DELAY           (1000.0f * 60.f)        // One minute delay.


// Enums
enum
{
    MODE_NONE,
    MODE_ON,
    MODE_WAIT,
    MODE_OFF,
};


// Debug assist
#if defined(ACHIEVEMENT_DEBUG) && (defined(_DEBUG) || defined(DEBUG))
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


// Implementation data.
typedef struct AchievementManagerImplementation
{
    /// -----------------------------------------------------------------------
    /// Ctor
    /// -----------------------------------------------------------------------
    AchievementManagerImplementation(const char *folder) : colour(prColour::White)
    {
        PRASSERT(folder && *folder);

        TODO("Finish converting achievement manager")

        save                    = new prSave(folder);
        achievementsCount       = 0;
        achievements            = NULL;
        pNotificationBar        = NULL;
        pFont                   = NULL;
        pTexture                = NULL;
        success                 = false;
        render                  = false;
//        hiRes                   = false;
        enabled                 = true;
        exp0                    = false;
        mode                    = MODE_NONE;
        pAchievementProvider    = NULL;
        pLanguage               = NULL;
        delay                   = 0.0f;
        timer                   = AWARDS_TEST_DELAY;
        on_pos                  = 0;
        off_pos                 = 0;
        step                    = 0;
        checkIndex              = 0;
        fontScale               = 1.0f;

        memset(text_title, 0, sizeof(text_title));
        memset(text_description, 0, sizeof(text_description));

#if defined(PLATFORM_PC)        
        pAchievementProvider = new prAchievement_PC();

#elif defined(PLATFORM_IOS)
        pAchievementProvider = new prAchievement_Ios();

//#elif defined(PLATFORM_BADA)
//        pAchievementProvider = new prAchievement_Bada();

#elif defined(PLATFORM_ANDROID)
        pAchievementProvider = new prAchievement_Android();

#elif defined(PLATFORM_LINUX)
        pAchievementProvider = new prAchievement_Linux();

#elif defined(PLATFORM_MAC)
        pAchievementProvider = new prAchievement_Mac();

#else
        #error No platform defined.

#endif
    }


    /// -----------------------------------------------------------------------
    /// Dtor
    /// -----------------------------------------------------------------------
    ~AchievementManagerImplementation()
    {
        PRSAFE_DELETE(save);
        PRSAFE_DELETE(pAchievementProvider);
        PRSAFE_DELETE_ARRAY(achievements);
    }
    
    
    AchievementManagerImplementation(AchievementManagerImplementation& other);// : colour(other.colour)
    
    
    // Unused copy constructor.
    /*AchievementManagerImplementation(AchievementManagerImplementation& other) : colour(other.colour)
    {
        save                    = NULL;
        achievementsCount       = 0;
        achievements            = NULL;
        pNotificationBar        = NULL;
        pFont                   = NULL;
        pTexture                = NULL;
//        success                 = false;
        render                  = false;
        hiRes                   = false;
        enabled                 = true;
        exp0                    = false;
        mode                    = MODE_NONE;
        pAchievementProvider    = NULL;
        pLanguage               = NULL;
        delay                   = 0.0f;
        timer                   = AWARDS_TEST_DELAY;
        on_pos                  = 0;
        off_pos                 = 0;
        step                    = 0;
        fontScale               = 1.0f;

        memset(text_title, 0, sizeof(text_title));
        memset(text_description, 0, sizeof(text_description));
        
        pAchievementProvider    = NULL;
    }*/


    // Update
    void Update(f32 dt)
    {
        if (!enabled)
            return;

        // Check if awards given have been registered with the OS!
        timer -= dt;
        if (timer < 0.0f && mode == MODE_NONE)
        {
            timer = AWARDS_TEST_DELAY;

            if (achievements && achievementsCount > 0 && checkIndex < achievementsCount)
            {
                // If an achievement state is awarding, try to verify awarded state
                if (achievements[checkIndex].state == Proteus::Achievement::Awarding)
                {
                    //if (!lite_build)
                    {
                        if (pAchievementProvider)
                        {
                            if (pAchievementProvider->IsReady())
                            {
                                pAchievementProvider->Award(GetIdentifierByIndex(checkIndex), 0);
                                //SaveState();
                                //prTrace(LogError, "Resub %i of %i\n", num, achievementsCount);
                            }
                        }
                    }                    
                }
                //else
                //{
                //    //prTrace(LogError, "Skipping %i\n", num);
                //    timer = 64.0f;
                //}
                
                if (++checkIndex >= achievementsCount)
                {
                    checkIndex = 0;                
                }
            }
        }


        /*if (render)
        {
            if (!renderList.empty())
            {
                if (pTexture == NULL)
                {
                    // Get the achievement index.
                    std::list<u32>::iterator i = renderList.begin();
                    u32 index = (*i);

                    // Search for and load its icon
                    std::list<AchievementDefinition>::iterator itr = achievementsList.begin();
                    std::list<AchievementDefinition>::iterator end = achievementsList.end();
                    for (u32 currIndex = 0; itr != end; ++itr, currIndex++)
                    {
                        if (currIndex == index)
                        {
                            pTexture = prSystemResourceManager::Get()->Load<Texture>((*itr).image.c_str());
                            mode     = MODE_ON;
                            delay    = SHOW_ACHIEVEMENT_TIME;

                            // Set the title and description text.
                            memset(text_title, 0, sizeof(text_title));
                            memset(text_description, 0, sizeof(text_description));

                            strcpy(text_title, (*itr).description.c_str());

                            if (pLanguage)
                            {
                                char temp[512];
                                PRUNUSED(temp);

                                strcpy(temp, (*itr).achieved.c_str());
                                strcpy(temp, pLanguage->GetString(temp));

                                StringParseControls(temp, text_description);
                            }
                            else
                            {
                                strcpy(text_description, (*itr).achieved.c_str());
                            }
                            return;
                        }
                    }

                    prTrace(LogError, "Failed to find icon\n");
                    return;
                }
                
                switch(mode)
                {
                // Slide the bar on.
                case MODE_ON:
                    if (pNotificationBar->pos.y > on_pos)
                    {
                        pNotificationBar->pos.y -= step;
                    }
                    else
                    {
                        pNotificationBar->pos.y = (f32)on_pos;
                        mode = MODE_WAIT;
                    }
                    break;

                // Pause to allow viewing.
                case MODE_WAIT:
                    delay -= dt;
                    if (delay < 0)
                    {
                        mode = MODE_OFF;
                    }
                    break;

                // Slide the bar off.
                case MODE_OFF:
                    if (pNotificationBar->pos.y < off_pos)
                    {
                        pNotificationBar->pos.y += step;
                    }
                    else
                    {
                        pNotificationBar->pos.y = (f32)off_pos;
                        prSystemResourceManager::Get()->Unload(pTexture);
                        renderList.pop_front();
                        pTexture = NULL;
                        mode     = MODE_NONE;
                        //m_bloody_hack = 0;
                    }
                    break;
                }
            }
        }//*/
    }


    // ------------------------------------------------------------------------
    // Render
    // ------------------------------------------------------------------------
    void Render()
    {
        /*if (render)
        {
            if (!renderList.empty())
            {
                if (pNotificationBar)
                {
                    pNotificationBar->Draw();

                    if (pFont)
                    {
                        char temp [256] = {0};

                        strcpy(temp, text_description);
                        int cnt = StringWordWrap(temp, 30);

                        if (hiRes && cnt > -1)
                        {
                            // Draw achievement name and description.
                            pFont->Draw(pNotificationBar->pos.x + 260, pNotificationBar->pos.y + 22, fontScale, colour, BitmapFont::ALIGN_CENTER, text_title);

                            if (cnt == 1)
                            {
                                pFont->Draw(pNotificationBar->pos.x + 110, pNotificationBar->pos.y + 48, fontScale, colour, BitmapFont::ALIGN_LEFT, text_description);
                            }
                            else
                            {
                                int i = StringFindFirstIndex(temp, '\n');
                                temp[i] = '\0';

                                pFont->Draw(pNotificationBar->pos.x + 110, pNotificationBar->pos.y + 48, fontScale, colour, BitmapFont::ALIGN_LEFT, temp);
                                pFont->Draw(pNotificationBar->pos.x + 110, pNotificationBar->pos.y + 64, fontScale, colour, BitmapFont::ALIGN_LEFT, &temp[i+1]);
                            }
                        }
                        else
                        {
                            if (cnt > -1)
                            {
                            // Draw achievement name and description.
                            pFont->Draw(pNotificationBar->pos.x + 270, pNotificationBar->pos.y + 11, fontScale, colour, BitmapFont::ALIGN_CENTER, text_title);
                            if (cnt == 1)
                            {
                                pFont->Draw(pNotificationBar->pos.x + 172, pNotificationBar->pos.y + 24, fontScale, colour, BitmapFont::ALIGN_LEFT, text_description);
                            }
                            else
                            {
                                int i = StringFindFirstIndex(temp, '\n');
                                temp[i] = '\0';

                                pFont->Draw(pNotificationBar->pos.x + 172, pNotificationBar->pos.y + 24, fontScale, colour, BitmapFont::ALIGN_LEFT, temp);
                                pFont->Draw(pNotificationBar->pos.x + 172, pNotificationBar->pos.y + 36, fontScale, colour, BitmapFont::ALIGN_LEFT, &temp[i+1]);
                            }
                        }
                    }
                    }

                    if (pTexture)
                    {
                        glEnable(GL_BLEND);
                        ERR_CHECK();
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        ERR_CHECK();

                            glPushMatrix();
                            ERR_CHECK();
            
                            //glEnable(GL_TEXTURE_2D);
                            //ERR_CHECK();

                            glColor4f(1, 1, 1, 1);
                            ERR_CHECK();

                            if (hiRes)
                            {
                                glTranslatef(pNotificationBar->pos.x + 72, pNotificationBar->pos.y + 64, 0);
                                ERR_CHECK();
                                glScalef(64, 64, 0);
                                ERR_CHECK();
                            }
                            else
                            {
                                glTranslatef(pNotificationBar->pos.x + 150, pNotificationBar->pos.y + 32, 0);
                            ERR_CHECK();
                                glScalef(40, 40, 0);
                            ERR_CHECK();
                            }

                            pTexture->Bind();

                            Renderer::GetInstance()->DrawQuad(0.0f, 0.0f, 1.0f, 1.0f);
            
                            glPopMatrix();
                            ERR_CHECK();

                        glDisable(GL_BLEND);
                        ERR_CHECK();
                    }
                }


                //if (m_bloody_hack == 666 || m_bloody_hack == 999)
                //{
                //    glPopMatrix();
                //    ERR_CHECK();
                //}
            }
        }//*/
    }
    
    
    /// -----------------------------------------------------------------------
    /// Saves the achievements states.
    /// -----------------------------------------------------------------------
    /*void SaveState()
    {
        // Save the file.
        if (achievementsCount == 0)
            return;
        
        if (save)
        {
            save->StartSave(achievements, achievementsCount * sizeof(prAchievementStatus), this, "achievements.dat");
            success = false;
            do
            {
                save->Update();
            }
            while(save->IsWorking());
        
            if (!success)
            {
                prTrace(LogError, "Failed to save achievements status file.\n");
            }
        }
    }*/


    /// -----------------------------------------------------------------------
    /// Load the XML descriptions of the achievements.
    /// -----------------------------------------------------------------------
    void Load(const char *filename)
    {
        PRASSERT(filename && *filename);

        // Parse the document
        TiXmlDocument *doc = new TiXmlDocument(filename);
        if (doc)
        {
            if (doc->LoadFile())
            {
                ParseFile(doc);
                prTrace(LogError, "Loaded %i achievement definitions\n", achievementsList.size());
            }
            else
            {
                PRWARN("Failed to Load %s\n", filename);
            }

            delete doc;
        }
    }


    /// -----------------------------------------------------------------------
    /// Parses the xml file.
    /// -----------------------------------------------------------------------
    void ParseFile(TiXmlNode* pParent)
    {
        switch (pParent->Type())
        {
        case TiXmlNode::TINYXML_ELEMENT:
            {
                // File data
                if (strcmp(pParent->Value(), "achievements_file") == 0)
                {
                    // Unused
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
    void ParseAttribs_Achievement(TiXmlElement* pElement)
    {
        PRASSERT(pElement);
        if (pElement)
        {
            PRASSERT(pElement->Attribute("name"));
            PRASSERT(pElement->Attribute("desc_brief"));
            PRASSERT(pElement->Attribute("desc_howto"));
            PRASSERT(pElement->Attribute("desc_achieved"));
            PRASSERT(pElement->Attribute("identifier"));
            PRASSERT(pElement->Attribute("image"));

            prAchievementDefinition d;
            d.name          = pElement->Attribute("name");
            d.description   = pElement->Attribute("desc_brief");
            d.howTo         = pElement->Attribute("desc_howto");
            d.achieved      = pElement->Attribute("desc_achieved");
            d.identifier    = pElement->Attribute("identifier");
            d.image			= pElement->Attribute("image");
            d.hash          = prStringHash(pElement->Attribute("name"));

            achievementsList.push_back(d);
        }
    }


    // ------------------------------------------------------------------------
    // Callback
    // ------------------------------------------------------------------------
    //static void SaveCallback(int result)
    //{
        //success = (result == Save::SAVE_RESULT_SUCCESS);
    //}


    // ------------------------------------------------------------------------
    // Callback
    // ------------------------------------------------------------------------
    //static void LoadCallback(int result)
    //{
        //success = (result == Save::SAVE_RESULT_SUCCESS);
    //}


    // ------------------------------------------------------------------------
    // Add achievement to render list.
    // ------------------------------------------------------------------------
    void AddRenderList(u32 index)
    {
        if (render)
        {            
            renderList.push_back(index);
        }
    }


    // ----------------------------------------------------------------------------
    // Gets the identifer for the achievement.
    // ----------------------------------------------------------------------------
    const char *GetIdentifier(const char *name)
    {
        PRASSERT(name && *name);

        if (name && *name)
        {
            std::list<prAchievementDefinition>::iterator itr = achievementsList.begin();
            std::list<prAchievementDefinition>::iterator end = achievementsList.end();

            u32 hash = prStringHash(name);

            for (; itr != end; ++itr)
            {
                if (hash == (*itr).hash)
                {
//#if defined(PLATFORM_ANDROID)
//                    return (*itr).of.c_str();
//#else
                    return (*itr).identifier.c_str();
//#endif
                }
            }
        }

        prTrace(LogError, "prAchievementManager::GetIdentifier - Failed to find identifier: %s\n", name);
        return NULL;
    }


    // ----------------------------------------------------------------------------
    // Gets the identifer for the achievement.
    // ----------------------------------------------------------------------------
    const char *GetIdentifierByIndex(u32 index)
    {
        std::list<prAchievementDefinition>::iterator itr = achievementsList.begin();
        std::list<prAchievementDefinition>::iterator end = achievementsList.end();

        u32 idx = 0;

        for (; itr != end; ++itr)
        {
            if (idx == index)
            {
//#if defined(PLATFORM_ANDROID)
//                return (*itr).of.c_str();
//#else
                return (*itr).identifier.c_str();
//#endif
            }

            idx++;
        }

        prTrace(LogError, "prAchievementManager::GetIdentifierByIndex - Failed to find identifier\n");
        return NULL;
    }


    // --
    std::list<prAchievementDefinition>    achievementsList;
    std::list<u32>                        updateList;
    std::list<u32>                        renderList;

    // --
    u32                 achievementsCount;
    prSave              *save;
    prAchievementStatus *achievements;
    prAchievementBase   *pAchievementProvider;
    prSprite            *pNotificationBar;
    prTexture           *pTexture;
    prBitmapFont        *pFont;
    prLanguage          *pLanguage;
    u32                 mode;
    f32                 delay;
    f32                 timer;
    bool                render;
    bool                enabled;
    bool                success;
    bool                exp0;
    s32                 on_pos;
    s32                 off_pos;
    s32                 step;
    f32                 fontScale;
    prColour            colour;

    //u32                 m_bloody_hack;
    u32                 checkIndex;

    // --
    char                text_title[512];
    char                text_description[512];

    // --
    //static bool         success;

} AchievementManagerImplementation;


// ----------------------------------------------------------------------------
// statics
// ----------------------------------------------------------------------------
//bool AchievementManagerImplementation::success;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prAchievementManager::prAchievementManager(const char *folder) : pImpl (new AchievementManagerImplementation(folder))
                                                               , imp   (*pImpl)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAchievementManager::~prAchievementManager()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Update the achievement manager
/// ---------------------------------------------------------------------------
void prAchievementManager::Update(f32 dt)
{
    PRASSERT(pImpl);

#if defined(PLATFORM_BADA)
    return;

#else
    if (imp.enabled == false)
    {
        imp.Update(dt);
    }
#endif
}


/// ---------------------------------------------------------------------------
/// Render achievement dropdowns if required.
/// ---------------------------------------------------------------------------
void prAchievementManager::Render()
{
    PRASSERT(pImpl);

#if defined(PLATFORM_BADA)
    return;

#else
    if (imp.enabled == false)
        return;

    imp.Render();

#endif
}


/// ---------------------------------------------------------------------------
/// Award an achievement
/// ---------------------------------------------------------------------------
void prAchievementManager::Award(const char *key, s32 awardValue)
{
#if defined(PLATFORM_BADA)
    return;

#else

    if (imp.enabled == false)
        return;

    PRASSERT(pImpl);
    PRASSERT(key && *key);
    PRASSERT(imp.achievementsCount > 0);

    if (key && *key)
    {
        u32 hash   = prStringHash(key);
        bool found = false;

        for (u32 i=0; i<imp.achievementsCount; i++)
        {
            if (imp.achievements[i].hash == hash)
            {
                found = true;

                //
                if (imp.achievements[i].state == Proteus::Achievement::NotAwarded)
                {
                    imp.achievements[i].count++;

                    if (Achieved(imp.achievements[i].count, awardValue, -1))
                    {
                        // Award
                        if (imp.pAchievementProvider)
                        {
/*                            if (!lite_build)
                            {
                                // Start the award process.
                                imp.pAchievementProvider->Award(imp.GetIdentifier(key), 0);

                                // Set award is being awarded.
                                imp.achievements[i].state = Proteus::Achievement::Awarding;

                                // Add to display handler
                                imp.AddRenderList(i);

                                // Save state change
                                Save();
                            }
                            else
                            {
                                // Start the award process.
//                                imp.pAchievementProvider->Award(imp.GetIdentifier(key), 0);
                                
                                // Set award is being awarded.
                                imp.achievements[i].state = Proteus::Achievement::Awarding;
                                
                                // Add to display handler
                                imp.AddRenderList(i);
                                
                                // Save state change
                                Save();
                            }//*/
                        }
                    }
                }
                break;
            }
        }

        if (!found)
        {
            prTrace(LogError, "prAchievementManager::Award - Failed to find achievement: %s\n", key);
        }
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Is an achievement awarded
/// ---------------------------------------------------------------------------
bool prAchievementManager::IsAwarded(const char *key) const
{
    PRASSERT(pImpl);

#if defined(PLATFORM_BADA)
    PRUNUSED(key);
    return false;

#else

    bool result = false;

    PRASSERT(key && *key);
    PRASSERT(imp.achievementsCount > 0);

    if (key && *key)
    {
        u32 hash   = prStringHash(key);

        for (u32 i=0; i<imp.achievementsCount; i++)
        {
            if (imp.achievements[i].hash == hash)
            {
                if (imp.achievements[i].state == Proteus::Achievement::Awarding)
                {
                    result = true;
                }
                break;
            }
        }
    }

    return result;

#endif
}


/// ---------------------------------------------------------------------------
/// Load the achievements.
/// ---------------------------------------------------------------------------
void prAchievementManager::Load(const char *filename)
{
    PRASSERT(filename && *filename);

    // Load achievement definitions.
    imp.Load(filename);

    // Load the status data.
    s32 size = 0;
    if (imp.save)
    {
        imp.save->StartLoad((void **)&imp.achievements, &size, this, "achievements.dat");    
        imp.success = false;
        do
        {
            imp.save->Update();
        }
        while(imp.save->IsWorking());
    }
    else
    {
        PRPANIC("No save class");
        return;
    }
    
    // No achievements, then write the default file.
    if (!imp.success)
    {
        if (!imp.achievementsList.empty())
        {
            // Create the achievements status list and save it.
            u32 count = (u32)imp.achievementsList.size();

            prAchievementStatus *alist = new prAchievementStatus[count];
            if (alist)
            {
                // Set initial states
                std::list<prAchievementDefinition>::iterator itr = imp.achievementsList.begin();
                std::list<prAchievementDefinition>::iterator end = imp.achievementsList.end();
                s32 index = 0;
                for (; itr != end; ++itr, ++index)
                {
                    alist[index].state = Proteus::Achievement::NotAwarded;      // Not awarded
                    alist[index].hash  = prStringHash((*itr).name.c_str());     // Name hash
                    alist[index].count = 0;                                     // Count times called. Award has when target reached
                }

                // Store.
                imp.achievements = alist;

                // Set number of achievements.
                imp.achievementsCount = (u32)imp.achievementsList.size();

                // Save
                Save();

                if (imp.success)
                {
                    prTrace(LogError, "Saved initial achievements status file.\n");
                }
                else
                {
                    prTrace(LogError, "Failed to save initial achievements status file.\n");
                }
            }
            else
            {
                prTrace(LogError, "Failed to allocate 'AchievementStatus' buffer\n");
            }
        }
        else
        {
            prTrace(LogError, "Failed to load achievements\n");
        }
    }
    else
    {
        // Set number of achievements.
        imp.achievementsCount = size / sizeof(prAchievementStatus);
        PRASSERT(imp.achievementsCount == imp.achievementsList.size());
        prTrace(LogError, "Loaded %i achievements\n", imp.achievementsCount);
    }


#if defined(ACHIEVEMENT_DEBUG) && (defined(_DEBUG) || defined(DEBUG))
    if (imp.achievementsCount > 0)
    {
        for (u32 i=0; i<imp.achievementsCount; i++)
        {
            prTrace(LogError, "Name: %*s, Hash %08x, State: %s, Count %i\n", 24, GetNameByIndex(i), imp.achievements[i].hash, GetAchievementState(imp.achievements[i].state), imp.achievements[i].count);
        }
    }
#endif
}


/// ---------------------------------------------------------------------------
/// Saves the achievements states.
/// ---------------------------------------------------------------------------
void prAchievementManager::Save()
{
    PRASSERT(pImpl);

    // No achievements?
    if (imp.achievementsCount == 0)
        return;

    // No data?
    if (imp.achievements == NULL)
        return;
        
    // Save the file.
    if (imp.save)
    {
        imp.save->StartSave(imp.achievements, imp.achievementsCount * sizeof(prAchievementStatus), this, "achievements.dat");
        imp.success = false;
        do
        {
            imp.save->Update();
        }
        while(imp.save->IsWorking());
        
        if (!imp.success)
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
        imp.success = true;
    }
    else
    {
        imp.success = false;
    }

    prTrace(LogError, "Achievements loaded %s\n", PRBOOL_TO_STRING(imp.success));
}


/// ---------------------------------------------------------------------------
/// This method receives state messages.
/// ---------------------------------------------------------------------------
void prAchievementManager::LoadResult(s32 result)
{
    if (result == IO_RESULT_SUCCESS)
    {
        imp.success = true;
    }
    else
    {
        imp.success = false;
    }

    prTrace(LogError, "Achievements loaded %s\n", PRBOOL_TO_STRING(imp.success));
}


/// ---------------------------------------------------------------------------
/// Sets whether the engine or the provider renders the achievement popup.
/// The engine is much faster and it allows for platforms that
/// don't have achievements.
/// ---------------------------------------------------------------------------
void prAchievementManager::SetRender(bool state)
{
    PRASSERT(pImpl);
    imp.render = state;
}


// ----------------------------------------------------------------------------
// Gets the description text for the specified entry by type.
// ----------------------------------------------------------------------------
const char *prAchievementManager::GetDescriptionText(const char *name, s32 type)
{
    PRASSERT(pImpl);
    PRASSERT(name && *name);

    if (name && *name)
    {
        std::list<prAchievementDefinition>::iterator itr = imp.achievementsList.begin();
        std::list<prAchievementDefinition>::iterator end = imp.achievementsList.end();

        u32 hash = prStringHash(name);

        for (; itr != end; ++itr)
        {
            if (hash == (*itr).hash)
            {
                switch(type)
                {
                case Proteus::Description::Brief:
                    return (*itr).description.c_str();

                case Proteus::Description::Achieved:
                    return (*itr).achieved.c_str();

                case Proteus::Description::HowTo:
                    return (*itr).howTo.c_str();
                }
            }
        }
    }

    prTrace(LogError, "prAchievementManager::GetDescriptionText - Failed to find achievement: %s\n", name);
    return NULL;
}


// ----------------------------------------------------------------------------
// Gets the description text for the specified entry by type.
// ----------------------------------------------------------------------------
const char *prAchievementManager::GetDescriptionTextByIndex(u32 index, s32 type)
{
    PRASSERT(pImpl);

    std::list<prAchievementDefinition>::iterator itr = imp.achievementsList.begin();
    std::list<prAchievementDefinition>::iterator end = imp.achievementsList.end();

    for (u32 currIndex = 0; itr != end; ++itr, currIndex++)
    {
        if (currIndex == index)
        {
            switch(type)
            {
            case Proteus::Description::Brief:
                return (*itr).description.c_str();

            case Proteus::Description::Achieved:
                return (*itr).achieved.c_str();

            case Proteus::Description::HowTo:
                return (*itr).howTo.c_str();
            }
        }
    }

    prTrace(LogError, "prAchievementManager::GetDescriptionTextByIndex - Failed to find achievement: %i\n", index);
    return NULL;
}


// ----------------------------------------------------------------------------
// Set the position of the notification bar.
// ----------------------------------------------------------------------------
void prAchievementManager::SetNotificationPosition(u32 pos)
{
    PRASSERT(pImpl);
    PRUNUSED(pos);

    /*switch(pos)
    {
    // An evil hack
    case 666:
        imp.m_bloody_hack = 666;
        break;

    case 999:
        imp.m_bloody_hack = 999;
        break;
    }*/
}


// ----------------------------------------------------------------------------
// Get number of achievements
// ----------------------------------------------------------------------------
u32 prAchievementManager::GetCount() const
{
    PRASSERT(pImpl);
    return imp.achievementsCount;
}


// ----------------------------------------------------------------------------
// Get the name of the achievement by index.
// ----------------------------------------------------------------------------
const char *prAchievementManager::GetNameByIndex(u32 index)
{
    PRASSERT(pImpl);

    std::list<prAchievementDefinition>::iterator itr = imp.achievementsList.begin();
    std::list<prAchievementDefinition>::iterator end = imp.achievementsList.end();

    for (u32 currIndex = 0; itr != end; ++itr, currIndex++)
    {
        if (currIndex == index)
        {
            return (*itr).name.c_str();
        }
    }

    return NULL;
}


// ----------------------------------------------------------------------------
// Set the image to use as the bar
// ----------------------------------------------------------------------------
void prAchievementManager::SetNotificationBar(prSprite *bar)
{
    PRASSERT(pImpl);
    PRASSERT(bar);
/*    imp.pNotificationBar = bar;

    if (bar)
    {
        imp.off_pos = atoi(System::GetValue("ScreenHeight"));
        imp.on_pos  = imp.off_pos - bar->FrameHeight();
        imp.step    = bar->FrameHeight() / 32;
    }*/
}


// ----------------------------------------------------------------------------
// Set the notification bars position.
// ----------------------------------------------------------------------------
void prAchievementManager::SetNotificationBarPos(s32 x, s32 y)
{
    if (imp.pNotificationBar)
    {
        imp.pNotificationBar->pos.x  = (float)x;
        imp.pNotificationBar->pos.y  = (float)y;
    }
}


/// ---------------------------------------------------------------------------
/// Sets the font used.
/// ---------------------------------------------------------------------------
void prAchievementManager::SetNotificationBarFont(prBitmapFont *font, float width, float scale)
{
    PRASSERT(pImpl);
    PRASSERT(font);
    PRUNUSED(width);
    imp.pFont = font;
    imp.fontScale = scale;
}


/// ---------------------------------------------------------------------------
/// Set a pointer to the language code.
/// ---------------------------------------------------------------------------
void prAchievementManager::SetLanguage(prLanguage *language)
{
    PRASSERT(pImpl);
    imp.pLanguage = language;
}


/// ---------------------------------------------------------------------------
/// Set the text colour
/// ---------------------------------------------------------------------------
void prAchievementManager::SetTextColour(const prColour &c)
{
    imp.colour = c;
}


/// ---------------------------------------------------------------------------
/// Enable and disable achievements.
/// ---------------------------------------------------------------------------
void prAchievementManager::Enable(bool state)
{
    imp.enabled = state;
}


/// -- PRIVATE ----------------------------------------------------------------


/// ---------------------------------------------------------------------------
/// Test to see if an achievement has been achieved.
/// ---------------------------------------------------------------------------
bool prAchievementManager::Achieved(s32 value, s32 testValue, s32 type)
{
    PRASSERT(pImpl);
    PRUNUSED(type);               // In case we add compare types other than '>='

    bool result = false;

    if (value >= testValue)
    {
        result = true;
    }

    return result;
}
