// File: prFile.h
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


#include "../core/prTypes.h"


// Forward declarations
struct FileImplementation;


// Class: prFile
//      Class used to represent a file in a cross platform manner
class prFile
{
public:
    // Friends
    friend class prFileManager;

    // Method: prFile
    //      Ctor
    explicit prFile(const char *filename);

    // Method: ~prFile
    //      Dtor
    ~prFile();

    // Method: Size
    //      Gets the size of the file.
    u32 Size() const;

    // Method: Exists
    //      Determines if the file exists
    bool Exists();

    // Method: Open
    //      Opens a file for reading.
    bool Open();
    
    // Method: Close
    //      Closes a file
    void Close();
    
    // Method: Read
    //      Reads data from an open file
    u32 Read(void *pDataBuffer, u32 size);

    // Method: Seek
    //      Seek within an open file.
    void Seek(s32 offset, s32 origin);

    // Method: Tell
    //      Returns the file pointers position.
    s32 Tell() const;

    // Method: Rewind
    //      Sets the file pointer back to the start of the file.
    void Rewind();


private:
    // For engine use.
    void Internal_Rewind();
    void Internal_Seek(s32 offset, s32 origin);
    s32  Internal_Tell() const;


private:
    // Stops passing by value and assignment.
    prFile(const prFile&);
    const prFile& operator = (const prFile&);


private:
    // Don't change order.
    FileImplementation  *pImpl;
    FileImplementation  &imp;
};
