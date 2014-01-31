/**
 * @file       prBackground.h
 * @brief      Contains 2D background class designed to handle simple
 * @n          and scrolling screens
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
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


#ifndef __PRBACKGROUND_H
#define __PRBACKGROUND_H


#include "../core/prTypes.h"
#include "../math/prVector2.h"
#include "prColour.h"


// Forward declarations.
class TiXmlNode;
class TiXmlElement;
class prTexture;


/// @brief      Background class designed to handle simple and scrolling screens
/// @note       Intended for title, menu and other types of 2D screens.
class prBackground
{
public:

    /**
     * The background type
     */
    enum prBackgroundType
    {
        UNKNOWN = -1,           ///< Defaults to unknown
        IMAGE   = 0,            ///< A single background image - Generally these are title, loading and menu screens
        TILEMAP = 1,            ///< A tile mapped background - Generally large scrolling levels
    };


public:

    /// @brief      Returns the width of the background.
    /// @return     The width
    s32 GetWidth() const { return m_width; }

    /// @brief      Returns the height of the background.
    /// @return     The height
    s32 GetHeight() const { return m_height; }

    /// @brief      Returns the type of the background.
    /// @return     The background type
    /// @see        prBackgroundType
    s32 GetType() const { return m_type; }

    /// @brief      Draws the background.
    void Draw();

    /// @brief      Sets a tint colour.
    /// @param      c - The tint colour
    void SetColour(prColour c);


public:

    prVector2 pos;


private:

    friend class prBackgroundManager;

    /// @brief      Ctor.
    /// @param      filename - A background file
    prBackground(const char *filename);

    /// @brief      Dtor.
    ~prBackground();


private:

    /// @brief      Parses the xml file.
    /// @param      pParent - A node pointer
    void ParseFile(TiXmlNode* pParent);
    
    /// @brief      Attribute parser
    /// @n          Used to get information about the file.
    /// @param      pElement - An element pointer
    void ParseAttribs_File(TiXmlElement* pElement);

    /// @brief      Attribute parser
    /// @n          Used to get information about the background like its name, type, etc.
    /// @param      pElement - An element pointer
    void ParseAttribs_Background(TiXmlElement* pElement);


private:

    // Stops passing by value and assignment.
    prBackground(const prBackground&);
    const prBackground& operator = (const prBackground&);

    
private:

    prTexture      *m_texture;
    char           *m_filename;
    s32             m_width;
    s32             m_height;
    s32             m_type;
    bool            m_correctFileType;
    bool            m_exp0;
    bool            m_exp1;
    bool            m_exp2;
    prColour        m_colour;
    f32             m_pixelWidth;
    f32             m_pixelHeight;
    f32             m_scrnWidth;
    f32             m_scrnHeight;
    f32             m_v0;
    f32             m_u1;
    f32             m_xAdjust;
    f32             m_yAdjust;
};


#endif//__PRBACKGROUND_H
