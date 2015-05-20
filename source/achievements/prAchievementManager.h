// File: prAchievementManager.h
/**
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


#ifndef __PRACHIEVEMENTMANAGER_H
#define __PRACHIEVEMENTMANAGER_H


#include "../core/prTypes.h"
#include "../persistence/prSave.h"


// Enums
namespace Proteus
{
    // Achievement status. Independently tracked and verified with the OS.
    namespace Achievement
    {
        enum
        {
            NotAwarded,                         // Award not given
            Awarding,                           // The award process has started
            Awarded,                            // Award confimation has been received.
            Received,                           // For games where an award can be received,
                                                // but not awarded until they purchase
                                                // the full version.
        };
    }

    // Notification position. If available
    namespace Notification
    {
        enum
        {
            Top,                                // Top of the screen.
            Bottom,                             // Bottom of the screen.
            Middle                              // Middle of the screen.
        };
    }

    // Description type.
    namespace Description
    {
        enum
        {
            Brief,                              // Brief description.
            HowTo,                              // The how to achieve description.
            Achieved                            // The achieved description.
        };
    }
}


// The achievement status.
typedef struct prAchievementStatus
{
    Proteus::Core::s32     state;               // State
    Proteus::Core::u32     hash;                // Hash
    Proteus::Core::u32     count;               // Count

} prAchievementStatus;


// Allows for platform specific implementations.
struct AchievementManagerImplementation;


// Forward declarations.
class prSprite;
class prBitmapFont;
class prLanguage;
class prColour;


// Class: prAchievementManager
//      Achievements manager
class prAchievementManager : public prIoResultCallback
{
public:
    // Method: prAchievementManager
    //      Ctor
    prAchievementManager(const char *folder);

    // Method: ~prAchievementManager
    //      Dtor
    ~prAchievementManager();

    // Method: Update
    //      Updates the achievement manager
    void Update(Proteus::Core::f32 dt);

    // Method: Render
    //      Renders the achievement dropdowns if required.
    void Render();

    // Method: Award
    //      Award an achievement
    //
    // Parameters:
    //      key         - key desc
    //      awardValue  - desc
    void Award(const char *key, Proteus::Core::s32 awardValue);

    // Is an achievement awarded
    bool IsAwarded(const char *key) const;

    // Loads the achievement definition file and the achievement status file.
    void Load(const char *filename);

    // Saves the achievements states.
    void Save();

    // Method: SaveResult
    //      This method receives state messages.
    //
    // Parameters:
    //      save - The save result
    void SaveResult(Proteus::Core::s32 result);

    // Method: LoadResult
    //      This method receives state messages.
    //
    // Parameters:
    //      save - The save result
    void LoadResult(Proteus::Core::s32 result);

    // Sets whether the engine or the provider renders the achievement popup.
    // The engine can be faster and it allows for platforms that
    // don't have achievements.
    void SetRender(bool state);

    // Gets the description text for the specified entry by type.
    const char *GetDescriptionText(const char *name, Proteus::Core::s32 type);

    // Gets the description text for the specified entry by type.
    const char *GetDescriptionTextByIndex(Proteus::Core::u32 index, Proteus::Core::s32 type);

    // Set the position of the notification bar.
    void SetNotificationPosition(Proteus::Core::u32 pos);

    // Get number of achievements
    Proteus::Core::u32 GetCount() const;

    // Get the name of the achievement by index.
    const char *GetNameByIndex(Proteus::Core::u32 index);

    // Set the image to use as the bar
    void SetNotificationBar(prSprite *bar);

    // Set the notification bars position.
    void SetNotificationBarPos(Proteus::Core::s32 x, Proteus::Core::s32 y);

    // Method: SetNotificationBarFont
    //      Sets the font used by the notification bar.
    //
    // Parameters:
    //      font    - The font
    //      font    - The width of the font text before a new line occurs
    //      font    - The scale of the font
    //
    // Notes:
    //      This is an optional call
    void SetNotificationBarFont(prBitmapFont *font, float width, float scale);

    // Method: SetLanguage
    //      Set a pointer to the language code.
    //
    // Notes:
    //      This is an optional call.
    //
    // Notes:
    //      If the language provider is not set, then the passed text will be used as is
    void SetLanguage(prLanguage *language);

    // Method: SetTextColour
    //      Set the text colour of the achievement text
    //
    // Notes:
    //      This affects engine rendered achievements only, not client achievements
    void SetTextColour(const prColour &c);

    // Method: Enable
    //      Enable and disable achievements.
    void Enable(bool state);


private:
    // Test to see if an achievement has been achieved.
    bool Achieved(Proteus::Core::s32 value, Proteus::Core::s32 testValue, Proteus::Core::s32 type);


private:
    // Stops passing by value and assignment.
    prAchievementManager(const prAchievementManager&);
    const prAchievementManager& operator = (const prAchievementManager&);


private:
    // Don't change order.
    AchievementManagerImplementation  *pImpl;
    AchievementManagerImplementation  &imp;
};


#endif//__PRACHIEVEMENTMANAGER_H
