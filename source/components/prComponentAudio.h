// File: prComponentAudio.h
//      Actor component for audio
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


#include "../actor/prActorComponent.h"
#include "../math/prVector3.h"


// Class: prComponentAudio
//      Actor audio component
//
// Notes:
//      Used to attach audio data to a game actor object
class prComponentAudio : public Proteus::Actor::prActorComponent
{
public:
    //prComponentAudio(){}
    //~prComponentAudio(){}

    // Method: Play
    //      Plays the audio source
    //
    // Parameters:
    //      type - A user defined actor identifier
    void Play() {}

    // Method: PlayAtPosition
    //      Plays the audio source with positional data included
    //
    // Parameters:
    //      position - World position of the audio
    void PlayAtPosition(Proteus::Math::prVector3 position) {}

    //
    //
//    void Assign() {}

private:
    u8* pAudioData;
};