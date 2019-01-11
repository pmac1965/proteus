// File: prFadeManager.h
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


#include "../core/prCoreSystem.h"
#include "../core/prTypes.h"


// Enum: prFade
//      The type of fade to update and draw
//
//  - FadeToBlack
//  - FadeToWhite
//  - FadeToNormal
typedef enum prFade
{
    FadeToBlack,
    FadeToWhite,
    FadeToNormal

} prFade;


// Enum: prFadeColour
//      The fade colour
//
//  - FadeColourNone
//  - FadeColourBlack
//  - FadeColourWhite 
typedef enum prFadeColour
{
    FadeColourNone,
    FadeColourBlack,
    FadeColourWhite

} prFadeColour;


// Fade type. 
// This is an internal enumeration and is not documented externally
typedef enum prFadeType
{
    FadeTypeNone,
    FadeTypeColour,
    FadeTypeNormal

} prFadeType;


// Class: prFadeManager
//      Does basic screen fading to/from black, white and normal (No fade)
class prFadeManager : public prCoreSystem
{
public:
    // Method: prFadeManager
    //      Ctor
    prFadeManager();

    // Method: ~prFadeManager
    //      Dtor
    ~prFadeManager();

    // Method: Update
    //      Updates the fade.
    //
    // Parameters:
    //      dt - Delta time
    void Update(f32 dt);
    
    // Method: Draw
    //      Draws the fade.
    void Draw();
    
    // Method: Start
    //      Starts a fade running.
    //
    // Parameters:
    //      fade    - The type of fade
    //      runtime - How long should fading take?
    //
    // See Also:
    //      <prFade>
    void Start(prFade fade, f32 runtime);

    // Method: Isfading
    //      Determines if a fade is running.
    bool Isfading() const;

    // Method: IsFadeVisible
    //      Determines if the fade is visible.
    bool IsFadeVisible() const;

    // Method: SetBlack
    //      Set fade to black
    //
    // Parameters:
    //      in - The fade value
    //
    // Notes:
    //      The in value will be clamped to between 0 and 255
    void SetBlack(f32 in = 255.0f);

    // Method: SetWhite
    //      Set fade to white
    void SetWhite();

    // Method: SetNormal
    //      Turns off the fade
    void SetNormal();


private:
    // Resets to normal
    void Reset();


private:
    prFadeColour    colour;
    prFadeType      type;
    f32             alpha;
    f32             time;
    f32             step;
    bool            fading;
};
