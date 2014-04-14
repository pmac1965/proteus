// File: prJNIAudio.h
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


#ifndef __PRJNIAUDIO_H
#define __PRJNIAUDIO_H


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)


// Function: prJNI_SongPlay
//      Plays a song.
void prJNI_SongPlay(const char *filename);

// Function: prJNI_SongStop
//      Stops a song.
void prJNI_SongStop();

// Function: prJNI_SongSetVolume
//      Sets song volume.
void prJNI_SongSetVolume(float volume);

// Function: prJNI_SongPause
//      Pauses the currently playing song.
void prJNI_SongPause();

// Function: prJNI_SongResume
//      Resumes playing the current song.
void prJNI_SongResume();


#endif//PLATFORM_ANDROID


#endif//__PRJNIAUDIO_H
