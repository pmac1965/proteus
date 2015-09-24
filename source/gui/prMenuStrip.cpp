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
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include <cstdlib>
#include <cstdio>


#define MS_PIXEL_BUFFER     6.0f


using namespace Proteus::Math;
using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Gui {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenuStrip::prMenuStrip(const char *name, prSpriteManager *pSpriteManager) : prWidget(WT_MenuStrip, name, pSpriteManager)
{
    // Get screen width
    prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
    PRASSERT(pReg);
    mScreenWidth = (f32)atof(pReg->GetValue("ScreenWidth"));
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prMenuStrip::~prMenuStrip()
{
}


/// ---------------------------------------------------------------------------
/// Updates the menu strip.
/// ---------------------------------------------------------------------------
void prMenuStrip::Update(f32 dt)
{
    PRUNUSED(dt);
}


/// ---------------------------------------------------------------------------
/// Draw the dialog.
/// ---------------------------------------------------------------------------
void prMenuStrip::Draw()
{
    prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
    if (pRenderer)
    {
        f32 stripSize = 32.0f;

        // Calculate the strips size. Will text height plus a pixel buffer either side
        if (m_pBmpfont)
        {
            stripSize  = m_pBmpfont->MeasureString("Y", 1.0f).y;
            stripSize += MS_PIXEL_BUFFER;
        }
        else if (m_pTtfFont)
        {
            stripSize  = m_pTtfFont->MeasureString("Y", 1.0f).y;
            stripSize += MS_PIXEL_BUFFER;
        }

        // Draw strip
        pRenderer->TexturesEnabled(false);
        pRenderer->SetColour(prColour::LiteGray);
        pRenderer->DrawFilledRect(0, 0, mScreenWidth, stripSize);
        pRenderer->TexturesEnabled(true);

        // Draw text
        f32 xpos = 0;
        f32 ypos = MS_PIXEL_BUFFER / 2;
        pRenderer->SetColour(prColour::Black);        
        std::list<prMenu *>::iterator it  = mMenus.begin();
        std::list<prMenu *>::iterator end = mMenus.end();
        for (; it != end; ++it)
        {
            if (m_pBmpfont)
            {
                char name[256];
                sprintf(name, "  %s  ", (*it)->GetText());
                prVector2 size = m_pBmpfont->MeasureString(name, 1.0f);
                m_pBmpfont->Draw(xpos, ypos, name);
                xpos += size.x;
            }
            else if (m_pTtfFont)
            {
            }
        }


        pRenderer->SetColour(prColour::White);
    }
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenuStrip::OnPressed(const prTouchEvent &e)
{
    PRUNUSED(e);
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenuStrip::OnMove(const prTouchEvent &e)
{
    PRUNUSED(e);
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenuStrip::OnReleased(const prTouchEvent &e)
{
    PRUNUSED(e);
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


}}// Namespaces
