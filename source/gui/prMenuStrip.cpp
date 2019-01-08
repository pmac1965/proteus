/**
 * prMenuStrip.cpp
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


#include "prMenuStrip.h"
#include "prMenu.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../display/prRenderer.h"
#include "../display/prBitmapFont.h"
#include "../display/prTrueTypeFont.h"
#include "../math/prVector2.h"
#include "../math/prRect.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"

#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
#include "../input/prMouse.h"
#endif

#include <cstdlib>
#include <cstdio>


using namespace Proteus::Math;
using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Gui {


const f32 MS_YPIXEL_BUFFER      = 8.0f;                         // Buffer size in the Y direction between the top and the bottom of the strip with the text in the middle
const f32 MS_YPIXEL_BUFFER_HALF = (MS_YPIXEL_BUFFER / 2);       // Half size
const f32 MS_XPIXEL_BUFFER      = 12.0f;                        // Buffer size in the X direction between menu strip menu names
const f32 MS_XPIXEL_BUFFER_HALF = (MS_XPIXEL_BUFFER / 2);       // Half size
const f32 MS_STRIP_HEIGHT       = 12.0f;                        // Default string height - shouldn't get used, but just in case
const f32 MS_STARTX             = MS_XPIXEL_BUFFER;             // Text start position X
const f32 MS_STARTY             = MS_YPIXEL_BUFFER;             // Text start position Y


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenuStrip::prMenuStrip(const char *name, prSpriteManager *pSpriteManager) : prWidget(WT_MenuStrip, name, pSpriteManager)
{
    // For quick access to the registry
    mpRegistry = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
    PRASSERT(mpRegistry);

    // Get the mouse if correct system
    #if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
        mpMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
        PRASSERT(mpMouse);
    #else
        mpMouse = nullptr;
    #endif

    // Init
    mScreenWidth = (f32)atof(mpRegistry->GetValue("ScreenWidth"));
    mStartX      = MS_STARTX;
    mStartY      = MS_STARTY;
    mOpen        = PRFALSE;
    mInit        = PRFALSE;
}


/// ---------------------------------------------------------------------------
/// Updates the menu strip.
/// ---------------------------------------------------------------------------
void prMenuStrip::Update(f32 dt)
{
    PRUNUSED(dt);
    PRASSERT(mpRegistry);

    // Initialise strip height on first update (put in ctor once fonts have base class)
    TODO("(Put fonts in ctor once fonts have base class)")
    if (!mInit)
    {
        // Set the strip height
        if (m_pBmpfont)
        {
            prVector2 size = m_pBmpfont->MeasureString("Y", 1.0f);
            mStripHeight = MS_YPIXEL_BUFFER + size.y + MS_YPIXEL_BUFFER;
        }
        else if (m_pTtfFont)
        {
            prVector2 size = m_pTtfFont->MeasureString("Y", 1.0f);
            mStripHeight = MS_YPIXEL_BUFFER + size.y + MS_YPIXEL_BUFFER;
        }
        else
        {
            mStripHeight = MS_YPIXEL_BUFFER + MS_STRIP_HEIGHT + MS_YPIXEL_BUFFER;
            PRLOGI("Using the default menu strip height\n");
        }

        mInit = PRTRUE;
    }

    mScreenWidth = (f32)atof(mpRegistry->GetValue("ScreenWidth"));
}


/// ---------------------------------------------------------------------------
/// Draw the dialog.
/// ---------------------------------------------------------------------------
void prMenuStrip::Draw()
{
    prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
    if (pRenderer)
    {
        // Draw strip
        pRenderer->TexturesEnabled(false);
        pRenderer->SetColour(prColour::LiteGray);
        pRenderer->DrawFilledRect(0, 0, mScreenWidth, mStripHeight);
        pRenderer->TexturesEnabled(true);

        // Draw text
        f32 xpos = mStartX;
        f32 ypos = mStartY;

        auto it  = mMenus.begin();
        auto end = mMenus.end();
        for (s32 i=0; it != end; ++it, ++i)
        {
            if (m_pBmpfont)
            {
                char name[256];
                sprintf(name, "%s", (*it)->GetText());

                const prVector2 &size = (*it)->GetTextSize();

                // Is the menu open? If so draw the menu items
                if ((*it)->IsOpened())
                {
                    pRenderer->TexturesEnabled(false);

                    pRenderer->SetColour(prColour::LiteGray + prColour(.1f, .1f, .1f, 0.f));//prColour::Yellow);
                    pRenderer->DrawFilledRect(xpos - MS_XPIXEL_BUFFER_HALF, ypos, xpos + MS_XPIXEL_BUFFER_HALF + size.x, ypos + size.y);
                    
                    pRenderer->SetColour(prColour::Black);
                    pRenderer->DrawRect(xpos - MS_XPIXEL_BUFFER_HALF, ypos, xpos + MS_XPIXEL_BUFFER_HALF + size.x, ypos + size.y);
                    
                    //pRenderer->SetColour(prColour::LiteGray);
                    pRenderer->TexturesEnabled(true);

                    if ((*it)->DrawMenu(xpos - MS_XPIXEL_BUFFER_HALF, ypos + size.y, MS_XPIXEL_BUFFER_HALF + size.x + MS_XPIXEL_BUFFER_HALF))
                    {
                        SetAllMenusClosed();
                    }
                }
                // On desktop we highlight the mouse cursor
                #if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
                else if (InMenuRect((s32)(xpos - MS_XPIXEL_BUFFER_HALF), (s32)ypos, (s32)(size.x + MS_XPIXEL_BUFFER - 1.0f), (s32)size.y, mpMouse->x, mpMouse->y))
                {
                    pRenderer->TexturesEnabled(false);

//                    pRenderer->SetColour(prColour::Cyan);
                    pRenderer->SetColour(prColour::LiteGray + prColour(.1f, .1f, .1f, 0.f));//prColour::Yellow);
                    pRenderer->DrawFilledRect(xpos - MS_XPIXEL_BUFFER_HALF, ypos, xpos + MS_XPIXEL_BUFFER_HALF + size.x, ypos + size.y);
                    
                    pRenderer->SetColour(prColour::Black);
                    pRenderer->DrawRect(xpos - MS_XPIXEL_BUFFER_HALF, ypos, xpos + MS_XPIXEL_BUFFER_HALF + size.x, ypos + size.y);
                    pRenderer->DrawPoint(xpos - MS_XPIXEL_BUFFER_HALF - 1, ypos + size.y + 1); /* TEMP */
                    
                    //pRenderer->SetColour(prColour::LiteGray);
                    pRenderer->TexturesEnabled(true);
                }
                #endif

                pRenderer->SetColour(prColour::White);
                m_pBmpfont->Draw(xpos, ypos, name);

                xpos += (size.x + MS_XPIXEL_BUFFER);
            }
            else if (m_pTtfFont)
            {
            }
        }
    }

    // On desktop we check for mouse input here, as normal touch input is for
    // game use and has some issues on tool builds
    #if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
    if (mpMouse->ButtonPressed(prMouseButton::MOUSE_BUTTON_LEFT))
    {
        f32 xpos = mStartX;
        f32 ypos = mStartY;

        auto it  = mMenus.begin();
        auto end = mMenus.end();
        for (s32 i=0; it != end; ++it, i++)
        {
            if (m_pBmpfont)
            {
                const prVector2 &size = (*it)->GetTextSize();

                // On menu? Toggle
                if (InMenuRect((s32)xpos, (s32)ypos, (s32)size.x, (s32)size.y, mpMouse->x, mpMouse->y))
                {
                    if ((*it)->IsOpened())
                    {
                        (*it)->SetOpened(PRFALSE);
                    }
                    else
                    {
                        (*it)->SetOpened(PRTRUE);
                    }
                }
                // else close
                else
                {
                    (*it)->SetOpened(PRFALSE);
                }

                xpos += (size.x + MS_XPIXEL_BUFFER);
            }
            else if (m_pTtfFont)
            {
            }
        }
    }
    #endif
}


/// ---------------------------------------------------------------------------
/// Adds a menu to the menu strip
/// ---------------------------------------------------------------------------
void prMenuStrip::AddMenu(prMenu *pMenu)
{
    if (pMenu)
    {
        mMenus.push_back(pMenu);
    }
    else
    {
        PRWARN("You cannot add a null pointer to a menu strip");
    }
}


/// ---------------------------------------------------------------------------
/// Closes all the menus
/// ---------------------------------------------------------------------------
void prMenuStrip::SetAllMenusClosed()
{
    auto it  = mMenus.begin();
    auto end = mMenus.end();
    for (; it != end; ++it)
    {
        (*it)->SetOpened(PRFALSE);
    }
}


/// ---------------------------------------------------------------------------
/// Tests if a touch is *over* a menu strip item
/// ---------------------------------------------------------------------------
bool prMenuStrip::InMenuRect(s32 x, s32 y, s32 width, s32 height, s32 xpos, s32 ypos)
{
    prRect<s32> rect = prRect<s32>(y, x, y + height, x + width);
    return rect.PointInside(xpos, ypos);
}


}}// Namespaces
