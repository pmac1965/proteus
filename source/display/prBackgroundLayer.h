// File: prBackgroundLayer.h
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
#include "../core/prString.h"


// Forward declarations
class prTexture;


// Class: prBackgroundLayer
//      Represents a single background layer
class prBackgroundLayer
{
public:
    // Method: prBackgroundLayer
    //      Ctor
    //
    // Parameters:
    //      width       - The width of the background layer in tiles
    //      height      - The height of the background layer in tiles
    //      tileWidth   - The width of the tile
    //      tileHeight  - The height of the tile
    //      pTexture    - The texture for the layer
    prBackgroundLayer(s32 width, s32 height, s32 tileWidth, s32 tileHeight, prTexture *pTexture);


#if defined(PROTEUS_TOOL)
#endif

    // Method: ~prBackgroundLayer
    //      Dtor
    ~prBackgroundLayer();

    // Method: Draw
    //      Draws this layer
    void Draw();

#if defined(PROTEUS_TOOL)
    // Method: GetLayerWidth
    //      Gets the layers width. *Only available on tool builds*
    s32 GetLayerWidth() const { return mLayerWidth; }

    // Method: GetLayerHeight
    //      Gets the layers height. *Only available on tool builds*
    s32 GetLayerHeight() const { return mLayerHeight; }

    // Method: GetTileWidth
    //      Gets the width of the tiles used. *Only available on tool builds*
    s32 GetTileWidth() const { return mTileWidth; }

    // Method: GetTileHeight
    //      Gets the height of the tiles used. *Only available on tool builds*
    s32 GetTileHeight() const { return mTileHeight; }

    // Method: SetTextureFilename
    //      Sets the textures filename. *Only available on tool builds*
    void SetTextureFilename(const char *filename) { mTextureFilename.Set(filename); }

    // Method: GetTextureFilename
    //      Gets the textures filename. *Only available on tool builds*
    const char *GetTextureFilename() { return mTextureFilename.Text(); }

    // Method: GetMapData
    //      Gets the map data. *Only available on tool builds*
    s32 *GetMapData() { return mMapData; }
#endif


private:
    // Stop passing by value and assignment.
    prBackgroundLayer(const prBackgroundLayer&);
    const prBackgroundLayer& operator = (const prBackgroundLayer&);

    #if defined(PROTEUS_TOOL)
    prString    mTextureFilename;
    #endif

    s32         mLayerWidth;
    s32         mLayerHeight;
    s32         mTileWidth;
    s32         mTileHeight;
    s32         mTilesAcross;
    s32         mTilesDown;
    f32         mPixelWidth;
    f32         mPixelHeight;
    f32         mTileWidthInPixels;
    f32         mTileHeightInPixels;

    s32        *mMapData;
    prTexture  *mpTexture;
    PRBOOL      mWrap;
};
