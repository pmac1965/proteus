// File: prAchievementManager.h
//      Cross platform achievement manager with support for non-networked
//      management of achievements.
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


#pragma once


#include "../core/prTypes.h"
#include "../persistence/prSave.h"
#include "../display/prColour.h"
#include "prAchievement_base.h"
#include <list>


// Enums
namespace Proteus
{
    namespace Achievement
    {
        // Enum: Status
        //      The achievements status
        //
        // NotAwarded   - Award not given
        // Awarding     - The award process has started
        // Awarded,     - Award confimation has been received.
        // Received     - For games where an award can be received,
        //                but not awarded until they purchase
        //                the full version.
        //
        // Notes:
        //      The status is independently tracked and verified with the specific OS.
        enum Status
        {
            NotAwarded,
            Awarding,
            Awarded,
            Received
        };

        // Enum: DescriptionType
        //      The type of description. There are 3 types
        //
        // Brief       - The brief description.      
        // HowTo       - The how to achieve description.
        // Achieved    - The achieved description.
        enum DescriptionType
        {
            Brief,
            HowTo,
            Achieved
        };

        // Enum: NotificationPos
        //      Notification position. Allows for a total of nine default positions
        //
        // Top      - Top of the screen.
        // Centre   - Middle of the screen (Y axis)
        // Bottom   - Botton of the screen.
        // Left     - Left of the screen.
        // Middle   - Middle of the screen (X axis)
        // Right    - Right of the screen.
        //
        // Notes:
        //      Defaults to (Top | Middle)
        enum NotificationPos
        {
            Top             = 0x00000001,
            Centre          = 0x00000002,
            Bottom          = 0x00000004,
            Left            = 0x00010000,
            Middle          = 0x00020000,
            Right           = 0x00040000,
            TopLeft         = Top | Left,
            TopMiddle       = Top | Middle,
            TopRight        = Top | Right,
            CentreLeft      = Centre | Left,
            CentreMiddle    = Centre | Middle,
            CentreRight     = Centre | Right,
            BottomLeft      = Bottom | Left,
            BottomMiddle    = Bottom | Middle,
            BottomRight     = Bottom | Right,
        };
    }
}


// The achievement status.
typedef struct prAchievementStatus
{
    Proteus::Core::s32  state;                  // State
    Proteus::Core::u32  hash;                   // Hash
    Proteus::Core::u32  count;                  // Count

} prAchievementStatus;


// Forward declarations.
struct prAchievementDefinition;
class prSprite;
class prBitmapFont;
class prLanguage;
class prColour;
//class prAchievementBase;
class prSpriteManager;
class TiXmlNode;
class TiXmlElement;


// Class: prAchievementDisplayCallback
//      This mix-in class is used to informs clients of the achievement displays discreet steps
//
// Notes:
//      You can use this to customize the display of the achievement notification panel
class prAchievementDisplayCallback
{
public:
    // Enum: AchievementStep
    //      The discreet steps for displaying the achievement
    //
    // None         - The default idle state
    // BeginDisplay - The display is beginning. Sprite will have its default position reset
    // PanelOn      - If your sliding the panel on if should be done here. Runs for 1 second (0.0f to 1.0f)
    // PanelShow    - The panel is being displayed. Time will be 0.0f to run time
    // PanelOff     - If your sliding the panel off if should be done here. Runs for 1 second (0.0f to 1.0f)
    // EndDisplay   - The display has ended
    enum AchievementStep
    {
        None,
        BeginDisplay,
        PanelOn,
        PanelShow,
        PanelOff,
        EndDisplay
    };

    // Method: AwardDisplayNotification
    //      Informs the client of an individual step in the display of an achievement notification
    //
    // Parameters:
    //      step   - The discreet step
    //      pPanel - The display panel sprite
    //      time   - The run iome discreet step
    //
    // See Also:
    //      <AchievementStep>
    virtual void AwardDisplayNotification(AchievementStep step, prSprite *pPanel, Proteus::Core::f32 time) = 0;
};


// Class: prAchievementManager
//      Achievements manager
//
// Notes:
//      Uses its own internal sprite manager
class prAchievementManager : public prIoResultCallback
{
public:
    // Method: prAchievementManager
    //      Ctor
    //
    // Parameters:
    //      provider - The required achievements provider
    //
    // Notes:
    //      The provider enum is determined by platform
    //
    // See Also:
    //      <prAchievementProvider>
    explicit prAchievementManager(prAchievementProvider provider);

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
    void SaveResult(Proteus::Core::s32 result) override;

    // Method: LoadResult
    //      This method receives state messages.
    //
    // Parameters:
    //      save - The save result
    void LoadResult(Proteus::Core::s32 result) override;

    // Method: SetRender
    //      Sets whether the engine or the provider renders the achievement popup.
    //
    // Notes:
    //      The engine can be faster, and the engine allows for platforms that
    //      don't have achievements and finally it allows for a uniform appearance.
    //
    // Notes:
    //      Defaults to false
    void SetRender(bool state) { mRender = state; }

    // Method: GetDescriptionText
    //      Gets the description text for a specific achivevment by name, then sub-type.
    //
    // Parameters:
    //      name - The name of the achievement
    //      type - The type of description
    //
    // See Also:
    //      <DescriptionType>
    const char *GetDescriptionText(const char *name, Proteus::Achievement::DescriptionType type);

    // Method: GetDescriptionTextByIndex
    //      Gets the description text for the specified entry by index.
    //
    // Parameters:
    //      index - Index of the achievement
    //      type  - The type of description
    //
    // See Also:
    //      <DescriptionType>
    const char *GetDescriptionTextByIndex(Proteus::Core::u32 index, Proteus::Achievement::DescriptionType type);

    // Method: GetCount
    //      Gets number of achievements
    Proteus::Core::u32 GetCount() const { return mAchievementsCount; }

    // Method: GetNameByIndex
    //      Get the name of the achievement by index.
    const char *GetNameByIndex(Proteus::Core::u32 index);

    // Method: SetNotificationBar
    //      Set the image to use as the bar
    //
    // Parameters:
    //      filename - Filename of the sprite to use
    //      positiom - Position of the notification bar
    //
    // Notes:
    //      Any unset position will remain at 0
    void SetNotificationBar(const char *filename, Proteus::Achievement::NotificationPos position = Proteus::Achievement::NotificationPos::TopMiddle);

    // Method: SetNotificationBarFont
    //      Sets the font used by the notification bar.
    //
    // Parameters:
    //      font    - The font
    //      width   - The width of the font text before a new line occurs
    //      scale   - The scale of the font
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
    void SetLanguage(prLanguage *language) { mpLanguage = language; }

    // Method: SetTextColour
    //      Set the text colour of the achievement text
    //
    // Notes:
    //      This affects engine rendered achievements only, not client achievements
    void SetTextColour(const prColour &c) { mTextColour = c; }

    // Method: Enable
    //      Enable and disable achievements.
    //
    // Notes:
    //      Achievements are enabled by default
    void Enable(bool state) { mEnabled = state; }

    // Method: IsEnabled
    //      Returns enabled status.
    bool IsEnabled() const { return mEnabled; }

    // Method: SetDisplayStepCallbacks
    //      Sets the display step callback notifier.
    //
    // Parameters:
    //      pCallback - A class to receive the messages
    void SetDisplayStepCallbacks(prAchievementDisplayCallback *pCallback) { mprAchievementDisplayCallback = pCallback; }

private:
    // Test to see if an achievement has been achieved.
    bool Achieved(Proteus::Core::s32 value, Proteus::Core::s32 testValue, Proteus::Core::s32 type);

    // Load the XML descriptions of the achievements.
    void LoadAchievementDescriptions(const char *filename);

    // Parses the xml achievements descriptions file.
    void ParseFile(TiXmlNode* pParent);

    // Attribute parser.
    void ParseAttribs_Achievement(TiXmlElement* pElement);

    // Gets the identifer for the achievement.
    const char *GetIdentifierByIndex(Proteus::Core::u32 index);

private:
    // Stops passing by value and assignment.
    prAchievementManager(const prAchievementManager&);
    const prAchievementManager& operator = (const prAchievementManager&);

private:
    prSpriteManager                     *mpSpriteManager;
    prAchievementBase                   *mpAchievementProvider;                 // Base class for platform specific providers
    prSave                              *mpSave;                                // Allows saving
    prAchievementStatus                 *mpAchievements;                        // Status data about the achievements
    prLanguage                          *mpLanguage;                            // The language class (Optional)
    prSprite                            *mpNotificationBar;                     // The notification bar sprite
    prAchievementDisplayCallback        *mprAchievementDisplayCallback;         // Callback notifier (Optional)
    prColour                            mTextColour;                            // The text colour
    std::list<prAchievementDefinition>  mAchievementsList;                      // The achievement definitions
    Proteus::Core::s32                  mAchievementsCount;                     // The number of achievements
    Proteus::Core::s32                  mCheckIndex;                            // The index of an achievement to check
    Proteus::Core::s32                  mDisplayMode;                           // The index of an achievement to check
    Proteus::Core::f32                  mTestTimer;                             // Test for awards given?
    Proteus::Core::f32                  mDefaultPanelXPos;                      // Default panel position
    Proteus::Core::f32                  mDefaultPanelYPos;                      // Default panel position
    bool                                mIoSuccess;                             // Allows tracking of io sucess/failure
    bool                                mCorrectFile;                           // Ensures we loaded a achievements file
    bool                                mRender;                                // Does the engine render?
    bool                                mEnabled;                               // Update/render?
};
