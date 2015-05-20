// File: prFileSystem.h
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


#ifndef __PRFILESYSTEM_H
#define __PRFILESYSTEM_H


#include "../core/prTypes.h"


// Safely closes a file handle.
#define PRSAFE_CLOSE(fp)  if ((fp)) { fclose((fp)); (fp) = 0; }


// Function: prFileSave
//      Saves a binary file.
bool prFileSave(const char* filename, const void *data, Proteus::Core::u32 size);


// Function: prFileExist
//      Determines if the passed file exists.
bool prFileExist(const char *filename);


// Function: prFileRemoveReadOnly
//      Removes read only attributes if required.
bool prFileRemoveReadOnly(const char* filename);


// Function: prFileDelete
//      Deletes a file.
bool prFileDelete(const char* filename);


// Function: prFileApplyReadOnly
//      Applies the read only attributes if possible.
bool prFileApplyReadOnly(const char* filename);


// Function: prFileSize
//      Returns the size of a file or -1 on error.
Proteus::Core::s32 prFileSize(const char *filename);


// Function: prFileLoad
//      Loads a binary file into the supplied buffer.
bool prFileLoad(const char *filename, Proteus::Core::u32 filesize, void *buffer, Proteus::Core::u32 bufferSize);


// Function: prFileChangeExtension
//      Changes a filenames extension.
void prFileChangeExtension(char* filename, const char* extension, Proteus::Core::s32 bufferSize);


#endif//__PRFILESYSTEM_H
