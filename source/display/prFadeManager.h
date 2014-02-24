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


#ifndef __PRFADEMANAGER_H
#define __PRFADEMANAGER_H


#include "../core/prCoreSystem.h"


// Enum: prFade
//      The type of fade to draw.
//
//  - FadeNone
//  - FadeToBlack
//  - FadeToWhite
//  - FadeToNormal
typedef enum prFade
{
    FadeNone,
    FadeToBlack,
    FadeToWhite,
    FadeToNormal

} prFade;


// Enum: prFadeColour
//      Fade to colour
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


// Enum: prFadeType
//      Fade type.
typedef enum prFadeType
{
    FadeTypeNone,
    FadeTypeColour,
    FadeTypeNormal

} prFadeType;


// Class:
//      Does simple screen fades to black, to white and to normal.
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
    void Update(float dt);
    
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
    void Start(prFade fade, float runtime);

    // Method: Isfading
    //      Determines if a fade is running.
    bool Isfading() const;

    // Method: IsFadeVisible
    //      Determines if the fade is visible.
    bool IsFadeVisible() const;

    // Method: SetBlack
    //      Set to specific state
    //
    // Parameters:
    //      in - The fade value
    //
    // Notes:
    //      The in value will be clamped to between 0 and 255
    void SetBlack(f32 in = 255.0f);

    // Method: SetWhite
    //      Set to specific state
    void SetWhite();

    // Method: SetNormal
    //      Set to specific state
    void SetNormal();


private:

    // Resets to normal
    void Reset();


private:

    prFadeColour colour;
    prFadeType   type;
    float        alpha;
    float        time;
    float        step;
    bool         fading;
};


#endif//__PRFADEMANAGER_H
