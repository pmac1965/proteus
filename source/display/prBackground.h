// File: prBackground.h
//      prBackground is designed to handle single and multi-layered backgrounds
//
//
//      *IMAGE* type backgrounds single layered. Intended for simple 2D screens, like logo/title screens, etc
//
//
//      *TILED* type backgrounds can be multi or single layered.
//
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


#ifndef __PRBACKGROUND_H
#define __PRBACKGROUND_H


#include "../core/prTypes.h"
#include "../core/prString.h"
#include "../math/prVector2.h"
#include "prColour.h"


// Forward declarations.
class TiXmlNode;
class TiXmlElement;
class prTexture;
class prBackgroundLayer;


// Defines
#define BACKGROUND_MAX_LAYERS   4


// Class: prBackground
//      A class designed to handle single and multiplayered backgrounds.
class prBackground
{
public:
    // Enum: prBackgroundType
    //      The background types
    //
    // Values:
    //      UNKNOWN - Default type
    //      IMAGE   - A single background image. Generally these are title, loading and menu screens
    //      TILEMAP - A tile mapped background. Generally large scrolling levels
    enum prBackgroundType
    {
        UNKNOWN = -1,
        IMAGE   = 0,
        TILEMAP = 1,
    };


public:
    // Method: GetWidth
    //      Returns the width of the background.
    Proteus::Core::s32 GetWidth() const { return m_width; }

    // Method: GetHeight
    //      Returns the height of the background.
    Proteus::Core::s32 GetHeight() const { return m_height; }

    // Method: GetType
    //      Returns the type of the background.
    //
    // See Also:
    //      <prBackgroundType>
    Proteus::Core::s32 GetType() const { return m_type; }

    // Method: Draw
    //      Draws the background.
    void Draw();

    // Method: SetColour
    //      Sets the tint colour.
    //
    // Parameters:
    //      c - The tint colour
    void SetColour(prColour c);

    // Method: GetLayer
    //      Gets a layer of a tiled background
    //
    // Parameters:
    //      c - The tint colour
    //
    // See also:
    //      <prBackgroundLayer>
    //
    // Returns:
    //      Either a layer or NULL
    prBackgroundLayer* GetLayer(Proteus::Core::s32 index);

    // Method: SetVisible
    //      Sets the visibility.
    void SetVisible(Proteus::Core::PRBOOL state) { mVisible = state; }

    // Method: GetVisible
    //      Gets the visibility.
    Proteus::Core::PRBOOL GetVisible() const { return mVisible; }

    // Method: GetName
    //      Gets the backgrounds name.
    const char *GetName() const { return m_name.Text(); }


#if defined(PROTEUS_TOOL)
    // Method: SetName
    //      Sets the backgrounds name. *Only available on tool builds*
    void SetName(const char *name) { m_name.Set(name); }

    // Method: GetTextureFilename
    //      Gets the textures filename. *Only available on tool builds*
    const char *GetTextureFilename() const { return m_filenameTexture.Text(); }

    // Method: SetTextureFilename
    //      Sets the textures filename. *Only available on tool builds*
    //
    // Parameters:
    //      filename - A filename
    void SetTextureFilename(const char *filename) { m_filenameTexture.Set(filename); }

    // Method: SetBackgroundFilename
    //      Sets the backgrounds filename. *Only available on tool builds*
    //
    // Parameters:
    //      filename - A filename
    void SetBackgroundFilename(const char *filename) { m_filenameBackground.Set(filename); }

    // Method: GetBackgroundFilename
    //      Gets the backgrounds filename. *Only available on tool builds*
    const char *GetBackgroundFilename() const { return m_filenameBackground.Text(); }

    // Method: SetTexture
    //      Sets the backgrounds texture. *Only available on tool builds*
    //
    // Parameters:
    //      pTex - A texture pointer. *The_pointer_should_not_be_NULL*
    void SetTexture(prTexture* pTex);

    // Method: Save
    //      Saves the background. *Only available on tool builds*
    //
    // Notes:
    //      Uses the background filename set the by the user.
    //
    // See Also:
    //      <SetBackgroundFilename>
    //
    // Returns:
    //      false if save fails for any reason, else true
    bool Save();

    // Method: SetBackgroundType
    //      Sets the background type. *Only available on tool builds*
    //
    // Parameters:
    //      type - The background type
    //
    // See Also:
    //      <prBackgroundType>
    void SetBackgroundType(prBackgroundType type) { m_type = type; }

    // Method: GetMapSize
    //      Adds a new map layer
    prBackgroundLayer *AddNewLayer(s32 width, s32 height, s32 tileWidth, s32 tileHeight, prTexture *pTexture);
#endif


public:
    Proteus::Math::prVector2 pos;


private:
    friend class prBackgroundManager;

    // Method: prBackground
    //      Ctor
    //
    // Parameters:
    //      filename - A background file
    prBackground(const char *filename);

    // Method: ~prBackground
    //      Dtor
    ~prBackground();


private:
    // Method: ParseFile
    //      Parses the xml file.
    void ParseFile(TiXmlNode* pParent);
    
    // Method: ParseAttribs_File
    //      Attribute parser used to get information about the file.
    void ParseAttribs_File(TiXmlElement* pElement);

    // Method: ParseAttribs_Background
    //      Attribute parser used to get information about the background like its name, type, etc.
    void ParseAttribs_Background(TiXmlElement* pElement);

    // Method: ParseAttribs_Background
    //      Attribute parser used to get information about the backgrounds layers.
    void ParseAttribs_Layer(TiXmlElement* pElement);


private:
    // Stops passing by value and assignment.
    prBackground(const prBackground&);
    const prBackground& operator = (const prBackground&);

    
private:
    prTexture              *m_texture;
    prString                m_filenameTexture;
    prString                m_name;
    Proteus::Core::s32      m_width;
    Proteus::Core::s32      m_height;
    Proteus::Core::s32      m_type;
    bool                    m_correctFileType;
    bool                    m_widthHeightSupplied;
    bool                    m_exp1;
    bool                    m_exp2;
    prColour                m_colour;
    Proteus::Core::f32      m_pixelWidth;
    Proteus::Core::f32      m_pixelHeight;
    Proteus::Core::f32      m_scrnWidth;
    Proteus::Core::f32      m_scrnHeight;
    Proteus::Core::f32      m_v0;
    Proteus::Core::f32      m_u1;
    Proteus::Core::PRBOOL   mVisible;
    
    // Tiled map layers
    prBackgroundLayer *mLayers[BACKGROUND_MAX_LAYERS];

    #if defined(PROTEUS_TOOL)
    prString        m_filenameBackground;
    #endif
};


#endif//__PRBACKGROUND_H
