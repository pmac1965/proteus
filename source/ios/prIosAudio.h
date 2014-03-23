// File: prIosAudio.h
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


// Function: prIosSongPlay
//      Plays a song
void prIosSongPlay(const char *filename);

// Function: prIosSongStop
//      Stops a song
void prIosSongStop();

// Function: prIosSongPause
//      Pause or unpause a song.
void prIosSongPause(bool pause);

// Function: prIosSongSetVolume
//      Sets the song volume
void prIosSongSetVolume(float volume);


#endif//__APPLE_CC__

