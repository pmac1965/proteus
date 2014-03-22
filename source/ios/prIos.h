// File: prIos.h
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


#if defined(__APPLE_CC__)


#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#include <AudioToolbox/AudioServices.h>


// Function: prGetAppDataPath
//      Gets the applications data directory.
const char *prGetAppDataPath();

// Function: prGetDocumentsPath
//      Gets the applications documents path.
void prGetDocumentsPath(char *buffer);

// Function: prLoadAudioFile
//      Loads an audio file.
void *prLoadAudioFile(const char *filename, ALsizei *size, ALsizei *freq, ALenum *format);


#endif//__APPLE_CC__

