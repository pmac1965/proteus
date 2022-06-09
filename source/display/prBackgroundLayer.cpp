/**
 * prBackgroundLayer.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "../prConfig.h"
#include "prColour.h"
#include "prRenderer.h"
#include "../core/prCore.h"
#include "../core/prDefines.h"
#include "../display/prOglUtils.h"
#include "../display/prTexture.h"
#include <cstring>


// ----------------------------------------------------------------------------
// Platform specifics
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>  

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <cstring>

#else
  #error No platform defined.

#endif


#include "prBackgroundLayer.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../debug/prAlert.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prBackgroundLayer::prBackgroundLayer(s32 width, s32 height, s32 tileWidth, s32 tileHeight, prTexture *pTexture)
{
    PRASSERT(width > 0);
    PRASSERT(height > 0);
    PRASSERT(tileWidth > 0);
    PRASSERT(tileHeight > 0);
    PRASSERT(pTexture);

    mLayerWidth         = width;
    mLayerHeight        = height;
    mTileWidth          = tileWidth;
    mTileHeight         = tileHeight;
    mMapData            = new s32[width * height];
    mpTexture           = pTexture;
    mWrap               = FALSE;
    mPixelWidth         = 1.0f / pTexture->GetWidth();
    mPixelHeight        = 1.0f / pTexture->GetHeight();
    mTileWidthInPixels  = mPixelWidth * tileWidth;
    mTileHeightInPixels = mPixelHeight * tileHeight;
    
//    m_framesAcross = pTexture->GetWidth()  / frameWidth;
//    m_framesDown   = pTexture->GetHeight() / frameHeight;

    mTilesAcross        = pTexture->GetWidth()  / tileWidth;
    mTilesDown          = pTexture->GetHeight()  / tileWidth;

    prTrace(prLogLevel::LogError, "prBackgroundLayer: Map:(%i, %i), Tile:(%i, %i), Tile count:(%i, %i)\n", width, height, tileWidth, tileHeight, mTilesAcross, mTilesDown);
    prTrace(prLogLevel::LogError, "Map size         : %i\n", sizeof(s32) * (width * height));

    memset(mMapData, -1, sizeof(s32) * (width * height));
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prBackgroundLayer::~prBackgroundLayer()
{
    PRSAFE_DELETE_ARRAY(mMapData);
}


/// ---------------------------------------------------------------------------
/// Draws this layer
/// ---------------------------------------------------------------------------
void prBackgroundLayer::Draw()
{
    // TODO: Add screen offset for positioning

    if (mpTexture)
    {
        mpTexture->Bind();

        s32 tile = 0;
        //f32 posX;

        ERR_CHECK(glEnable(GL_BLEND));
        //ERR_CHECK();

        ERR_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        //ERR_CHECK();

   //         glPushMatrix();
     //       ERR_CHECK();

            //// translate to quad center, then translate for position
            //glTranslatef((GLfloat)(mTileWidth >> 1), (GLfloat)(mTileHeight >> 1), 0);
            //ERR_CHECK();

            //glTranslatef(0, 28, 0);
            //ERR_CHECK();

            //glScalef((GLfloat)mTileWidth, (GLfloat)mTileHeight, 0);
            //ERR_CHECK();

            
            //glTranslatef(mTileWidth, 0, 0);

            for (s32 y=0; y<mLayerHeight; y++)
            {
                s32 row = y*mLayerWidth;

                for (s32 x=0; x<mLayerWidth; x++)
                {
                    tile = mMapData[row + x];

                    if (tile == -1)
                        continue;
        //// Set the frame source rect position indices
        //s32 x = frame % m_framesAcross;
        //s32 y = frame / m_framesAcross;

                    // Set the tile image source rect
                    s32 xpos = tile % mTilesAcross;
                    s32 ypos = tile / mTilesAcross;
                    f32 m_u0 = xpos * mTileWidthInPixels;
                    f32 m_u1 = m_u0 + mTileWidthInPixels;            
                    f32 m_v0 = 1.0f - ((ypos * mTileHeightInPixels) + mTileHeightInPixels);
                    f32 m_v1 = m_v0 + mTileHeightInPixels;

                    prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
                    if (pRenderer)
                    {
                        ERR_CHECK(glPushMatrix());
                        //ERR_CHECK();

            // translate to quad center, then translate for position
            ERR_CHECK(glTranslatef((GLfloat)(mTileWidth >> 1), (GLfloat)(mTileHeight >> 1), 0));
            //ERR_CHECK();

            ERR_CHECK(glTranslatef(0, 28, 0));
            //ERR_CHECK();

                        glTranslatef((GLfloat)mTileWidth * x, (GLfloat)mTileHeight * y, 0.f);
                        glScalef((GLfloat)mTileWidth, (GLfloat)mTileHeight, 0);

                        pRenderer->DrawQuad(m_u0, m_v0, m_u1, m_v1, prColour::White);// m_colour);

    //                    prTrace(LogError, "Tile size %i, %i\n", mTileWidth , mTileHeight);
    //                    prTrace(LogError, "Tile %i, at (%i,  %i)\n", tile, mTileWidth * x, mTileHeight * y);
                
                        ERR_CHECK(glPopMatrix());
                        //ERR_CHECK();
                    }


    //#if defined(PLATFORM_PC)
    //        m_v0 += (m_ph / 2);             // Sub half pixel to stop blurring
    //#endif
                }
            }

//            glPopMatrix();
  //          ERR_CHECK();

        ERR_CHECK(glDisable(GL_BLEND));
        //ERR_CHECK();

    }
}
/*
        glEnable(GL_BLEND);
        ERR_CHECK();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ERR_CHECK();

            glPushMatrix();
            ERR_CHECK();
            
            // translate to quad center, then translate for position
            glTranslatef((float)(m_frameWidth >> 1), (float)(m_frameHeight >> 1), 0);
            ERR_CHECK();
            glTranslatef(pos.x, pos.y, 0);
            ERR_CHECK();

            float width  = (GLfloat)(m_frameWidth  * m_scaleX);
            float height = (GLfloat)(m_frameHeight * m_scaleY);

            // Set flips
            if ((m_flip & FLIP_LEFTRIGHT) == FLIP_LEFTRIGHT)
            {
                width = -width;
            }

            if ((m_flip & FLIP_UPDOWN) == FLIP_UPDOWN)
            {
                height = -height;
            }

            glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
            ERR_CHECK();
            glScalef(width, height, 0);
            ERR_CHECK();

            m_pTexture->Bind();

            prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
            if (pRenderer)
            {
                pRenderer->DrawQuad(m_u0, m_v0, m_u1, m_v1, m_colour);
            }
            
            glPopMatrix();
            ERR_CHECK();

        glDisable(GL_BLEND);
        ERR_CHECK();
*/
