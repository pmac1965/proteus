// File: prTexture.h
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


#ifndef __PRTEXTURE_H
#define __PRTEXTURE_H


#include "../core/prTypes.h"
#include "../core/prResource.h"


// Forward declarations
struct prPVRTextureHeader;


// Class: prTexture
//      Class that represents a texture.
class prTexture : public prResource
{
public:
    // Method: Bind
    //      Bind the texture.
    bool Bind();

    // Method: Unbind
    //      Unbind the texture.
    void Unbind();

    // Method: GetWidth
    //      Gets the textures width.
    s32 GetWidth() const { return m_width; }

    // Method: GetHeight
    //      Gets the textures height.
    s32 GetHeight() const { return m_height; }

    // Method: GetHasAlpha
    //      Gets the textures alpha state.
    bool GetHasAlpha() const { return m_alpha; }

    // Method: GetTexID
    //      Gets the textures ID.
    u32 GetTexID() const { return m_texID; }


private:

    // A friend 
    friend class prResourceManager;


    // Keep ctor/dtor private so only the resource manager can create/destroy.
    prTexture(const char *filename);

    // Dtor
    ~prTexture();

    // Load texture.
    void Load();

    // Unload texture.
    void Unload();

    // Allows embedded data to be used.
    void LoadFromMemory(void *pData, u32 size);

    // Allows raw data to be used
    void LoadFromRaw(void *pData, u32 size, u32 width, u32 height);

    // Checks for power of two compliance.
    //
    //      size - A texture width/height
    bool ValidateSize(s32 size);

    // Acquires the texture format
    bool GetTextureFormat(u32 texFormat, int &internalFormat, int &format, int &type, bool &compressed);

    // Validates the header
    bool ValidateHeader(prPVRTextureHeader *header);


private:

    // Stops passing by value and assignment.
    prTexture(const prTexture&);
    const prTexture& operator = (const prTexture&);


private:

    // Data
    s32     m_width;
    s32     m_height;
    u32     m_texID;
    bool    m_alpha;
    bool    m_exp0;
    bool    m_exp1;
    bool    m_exp2;
};


#endif//__PRTEXTURE_H
