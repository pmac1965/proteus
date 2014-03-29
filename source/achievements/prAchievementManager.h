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
    s32     state;                          // State
    u32     hash;                           // Hash
    u32     count;                          // Count

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
class prAchievementManager
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
    void Update(f32 dt);

    // Method: Render
    //      Renders the achievement dropdowns if required.
    void Render();

    // Method: Award
    //      Award an achievement
    //
    // Parameters:
    //      key         - key desc
    //      awardValue  - desc
    void Award(const char *key, s32 awardValue);

    // Is an achievement awarded
    bool IsAwarded(const char *key);

    // Loads the achievement definition file and the achievement status file.
    void Load(const char *filename);

    // Saves the achievements states.
    void Save();

    // Sets whether the engine or the provider renders the achievement popup.
    // The engine can be faster and it allows for platforms that
    // don't have achievements.
    void SetRender(bool state);

	// Gets the description text for the specified entry by type.
	const char *GetDescriptionText(const char *name, s32 type);

    // Gets the description text for the specified entry by type.
	const char *GetDescriptionTextByIndex(u32 index, s32 type);

    // Set the position of the notification bar.
    void SetNotificationPosition(u32 pos);

    // Get number of achievements
    u32 GetCount();

    // Get the name of the achievement by index.
    const char *GetNameByIndex(u32 index);

    // Set the image to use as the bar
    void SetNotificationBar(prSprite *bar);

    // Set the notification bars position.
    void SetNotificationBarPos(s32 x, s32 y);

    // Sets the font used.
    void SetNotificationBarFont(prBitmapFont *font, float width, float scale);

    // Set a pointer to the language code.
    void SetLanguage(prLanguage *language);

    //// Sets if its a lite build - quick hack - will add to system states once all set up
    //void IsLiteBuild(bool state);

    //// Another hack :-(
    //void HiRes(bool state);

    // Set the text colour
    void SetTextColour(const prColour &c);

    // Enable and disable achievements.
    void Enable(bool state);


private:

    bool Achieved(s32 value, s32 testValue, s32 type);


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
