/**
 * @file        prTexture.h
 * @brief       Contains a class that represents a texture.
 * @copyright   Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRTEXTURE_H
#define __PRTEXTURE_H


#include "../core/prTypes.h"
#include "../core/prResource.h"


// Forward declarations
struct prPVRTextureHeader;


/// Class that represents a texture.
class prTexture : public prResource
{
public:

    /// Bind the texture.
    bool Bind();

    /// Unbind the texture.
    void Unbind();

    /// Gets the textures width.
    s32 GetWidth() const { return m_width; }

    /// Gets the textures height.
    s32 GetHeight() const { return m_height; }

    /// Gets the textures alpha state.
    bool GetHasAlpha() const { return m_alpha; }

    /// Get the texture ID.
    u32 GetTexID() const { return m_texID; }


private:

    // A friend 
    friend class prResourceManager;


    /// Keep  ctor/dtor private so only the resource manager can create/destroy.
    prTexture(const char *filename);

    /// Dtor
    ~prTexture();

    /// Load texture.
    void Load();

    /// Unload texture.
    void Unload();

    /// Allows embedded data to be used.
    void LoadFromMemory(void *pData, u32 size);

    /// Allows raw data to be used
    //void LoadFromRaw(void *pData, u32 size, u32 width, u32 height);

    /// Checks for power of two compliance.
    /// @param      size - A texture width/height
    /// @return     true or false
    bool ValidateSize(s32 size);

    /// Acquires the texture format
    bool GetTextureFormat(u32 texFormat, int &internalFormat, int &format, int &type, bool &compressed);

    /// Validates the header
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
    bool    m_duplicate;
    bool    m_exp1;
    bool    m_exp2;
};


#endif//__PRTEXTURE_H
