// File: prBackground.h
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
#include "../math/prVector2.h"
#include "prColour.h"


// Forward declarations.
class TiXmlNode;
class TiXmlElement;
class prTexture;


// Class: prBackground
//      Background class designed to handle simple and scrolling screens.
//      Intended for title, menu and other types of 2D screens.
class prBackground
{
public:

    // Enum: prBackgroundType
    //      The background type
    //
    //      UNKNOWN - Default type
    //      IMAGE   - A single background image. Generally these are title, loading and menu screens
    //      TILEMAP - A tile mapped background. Generally large scrolling levels
    //
    enum prBackgroundType
    {
        UNKNOWN = -1,
        IMAGE   = 0,
        TILEMAP = 1,
    };


public:

    // Method: GetWidth
    //      Returns the width of the background.
    s32 GetWidth() const { return m_width; }

    // Method: GetHeight
    //      Returns the height of the background.
    s32 GetHeight() const { return m_height; }

    // Method: GetType
    //      Returns the type of the background.
    //
    // See Also:
    //      <prBackgroundType>
    s32 GetType() const { return m_type; }

    // Method: Draw
    //      Draws the background.
    void Draw();

    // Method: SetColour
    //      Sets the tint colour.
    //
    // Parameters:
    //      c - The tint colour
    void SetColour(prColour c);

    // Method: SetVisible
    //      Sets the visibility.
    void SetVisible(PRBOOL state) { mVisible = state; }

    // Method: GetVisible
    //      Gets the visibility.
    PRBOOL GetVisible() const { return mVisible; }


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
    //
    // Parameters:
    //      pParent - A node pointer
    void ParseFile(TiXmlNode* pParent);
    
    // Method: ParseAttribs_File
    //      Attribute parser used to get information about the file.
    //
    // Parameters:
    //      pElement - An element pointer
    void ParseAttribs_File(TiXmlElement* pElement);

    // Method: ParseAttribs_Background
    //      Attribute parser used to get information about the background like its name, type, etc.
    //
    // Parameters:
    //      pElement - An element pointer
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
    bool            m_widthHeightSupplied;
    bool            m_exp1;
    bool            m_exp2;
    prColour        m_colour;
    f32             m_pixelWidth;
    f32             m_pixelHeight;
    f32             m_scrnWidth;
    f32             m_scrnHeight;
    f32             m_v0;
    f32             m_u1;

    PRBOOL          mVisible;
};


#endif//__PRBACKGROUND_H
