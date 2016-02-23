/**
 * prMenu.cpp
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


#include "prMenu.h"
#include "prMenuItem.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../core/prMacros.h"
#include "../core/prMessageManager.h"
#include "../debug/prTrace.h"
#include "../display/prBitmapFont.h"
#include "../display/prTrueTypeFont.h"
#include "../display/prRenderer.h"
#include "../display/prSprite.h"
#include "../math/prVector2.h"
#include "../math/prRect.h"


#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
#include "../input/prMouse.h"
#endif


using namespace Proteus::Core;
using namespace Proteus::Math;


// Namespaces
namespace Proteus {
namespace Gui {


const f32 icon_gap   = 24.0f;
const f32 text_gap_x = 8.0f;
const f32 text_gap_y = 8.0f;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenu::prMenu(const char *name, prSpriteManager *pSpriteManager) : prWidget(WT_Menu, name, pSpriteManager)
{
    mMaxWidth        = 0;
    mMaxHeight       = 0;
    mOpened          = PRFALSE;
    mpGrayTextBMF    = nullptr;
    mpMessageManager = nullptr;
    
    // Get the mouse if correct system
    #if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
        mpMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
        PRASSERT(mpMouse);
    #else
        mpMouse = nullptr;
    #endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prMenu::~prMenu()
{
    for (auto it = mItems.begin(); it != mItems.end(); ++it)        
    {
        PRSAFE_DELETE((*it).second);
    }

    mItems.clear();
}


/// ---------------------------------------------------------------------------
/// Updates the menu strip.
/// ---------------------------------------------------------------------------
void prMenu::Update(f32 dt)
{
    PRUNUSED(dt);
}


/// ---------------------------------------------------------------------------
/// Draw the menu. Default version is not used
/// ---------------------------------------------------------------------------
void prMenu::Draw()
{
}

/// ---------------------------------------------------------------------------
/// Draws the menu.
/// ---------------------------------------------------------------------------
void prMenu::DrawMenu(f32 x, f32 y, f32 width)
{
    prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
    if (pRenderer)
    {
        pRenderer->TexturesEnabled(false);

        // Set menu panel colour
        pRenderer->SetColour(prColour::LiteGray + prColour(.1f, .1f, .1f, 0.f));

        f32 heightY = (f32)((mItems.size() * mMaxHeight));

        // Draw fill
        pRenderer->DrawFilledRect(x, y, x + mMaxWidth, y + heightY);

        // Draw borders
        pRenderer->SetColour(prColour::Black);
        pRenderer->DrawLine(x, y, x, y + heightY);
        pRenderer->DrawLine(x, y + heightY, x + mMaxWidth, y + heightY);
        pRenderer->DrawLine(x + mMaxWidth, y + heightY, x + mMaxWidth, y);
        pRenderer->DrawLine(x + width - 1, y, x + mMaxWidth, y);
        pRenderer->DrawPoint(x - 1, y + heightY + 1); /* TEMP */

        // Draw icon panel
        pRenderer->SetColour(prColour::LiteGray - prColour(.1f, .1f, .1f, 0.f));
        pRenderer->DrawFilledRect(x, y, x + icon_gap, y + heightY);

        pRenderer->TexturesEnabled(true);
                    
        // Draw the text
        pRenderer->SetColour(prColour::White);
        y += text_gap_y;

        for (auto it = mItems.begin(); it != mItems.end(); ++it)        
        {
            // Get the items text
            const char *mMenuItemText = (*it).second->GetText();

            // Is it a separator?
            if (*mMenuItemText == '-')
            {
                pRenderer->TexturesEnabled(false);
                 
                f32 xPos    = x + icon_gap + text_gap_x;
                f32 xPosEnd = x + mMaxWidth - text_gap_x;
                f32 yPos    = y + (mSize.y / 2);

                pRenderer->SetColour(prColour::Black);
                pRenderer->DrawLine(xPos, yPos, xPosEnd, yPos);
                
                yPos++;

                pRenderer->SetColour(prColour::White);
                pRenderer->DrawLine(xPos, yPos, xPosEnd, yPos);

                pRenderer->TexturesEnabled(true);
            }
            else
            {
                // Enabled?
                if ((*it).second->GetEnabled())
                {
                    #if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
                    f32 xPos    = x + icon_gap;
                    f32 xPosEnd = mMaxWidth - text_gap_x - icon_gap;

                    // On desktop we highlight the mouse cursor
                    if (InMenuItemRect(xPos, y - 3.0f, xPosEnd, mSize.y, mpMouse->x, mpMouse->y))
                    {
                        pRenderer->TexturesEnabled(false);
                        pRenderer->SetColour(prColour::LiteGray + prColour(.2f, .2f, .2f, 0.f));
                        pRenderer->DrawFilledRect(xPos, y - 3.0f, x + mMaxWidth - text_gap_x, y + mSize.y);
                        pRenderer->TexturesEnabled(true);  

                        if (mpMouse->ButtonPressed(prMouseButton::MOUSE_BUTTON_LEFT))
                        {
                            if (mpMessageManager)
                            {
                                prGameMessage msg;
                                msg.type = (*it).second->GetID();
                                mpMessageManager->Send(msg);
                            }
                        }
                    }
                    #endif

                    m_pBmpfont->Draw(x + icon_gap + text_gap_x, y, (*it).second->GetText());
                }
                else
                {
                    mpGrayTextBMF->Draw(x + icon_gap + text_gap_x, y, (*it).second->GetText());
                }
            }

            // Draw icon centrally if it exists
            prSprite *pIcon = (*it).second->GetIcon();
            if (pIcon)
            {
                pIcon->pos.x = x + (icon_gap / 2)   - (pIcon->GetFrameWidth() / 2);
                pIcon->pos.y = y + (mMaxHeight / 2) - (pIcon->GetFrameHeight() / 2) - (text_gap_y / 2);
                pIcon->Draw();
            }

            y += mMaxHeight;
        }
                    
        pRenderer->SetColour(prColour::LiteGray);
    }
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenu::OnPressed(const prTouchEvent &e)
{
    PRUNUSED(e);
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenu::OnMove(const prTouchEvent &e)
{
    PRUNUSED(e);
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenu::OnReleased(const prTouchEvent &e)
{
    PRUNUSED(e);
}


/// ---------------------------------------------------------------------------
///
/// ---------------------------------------------------------------------------
bool prMenu::AddMenuItem(prMenuItem *pMenuItem)
{
    bool added = false;

    if (pMenuItem)
    {
        u32 key = pMenuItem->GetID();

        if (mItems.find(key) == mItems.end())
        {
            mItems.insert
            (
                std::pair<u32, prMenuItem *>(key, pMenuItem)
            );

            // Get the width of the text, set max width
            if (m_pBmpfont)
            {
                prVector2 size = m_pBmpfont->MeasureString(pMenuItem->GetText(), 1.0f);
                AdjustSize(size);
            }
            else if (m_pTtfFont)
            {
                prVector2 size = m_pTtfFont->MeasureString(pMenuItem->GetText(), 1.0f);
                AdjustSize(size);
            }

            added = true;
        }
        else
        {
            prTrace(LogError, "Failed to add menu item. Is the menu item ID unique?\n - [%s]\n", pMenuItem->GetText());
        }
    }

    return added;
}


/// ---------------------------------------------------------------------------
/// Sets the menus text. E.g. like 'File'
/// ---------------------------------------------------------------------------
void prMenu::SetText(const char *text)
{
    // Set text
    mText.Set(text);

    // And the size
    if (m_pBmpfont)
    {
        mSize = m_pBmpfont->MeasureString(text, 1.0f);
    }
    else if (m_pTtfFont)
    {
        mSize = m_pTtfFont->MeasureString(text, 1.0f);
    }
}


/// ---------------------------------------------------------------------------
/// Sets the text max width and height, includes icon and text gaps, etc
/// ---------------------------------------------------------------------------
void prMenu::AdjustSize(const prVector2 &size)
{
    s32 xWidth = (s32)(size.x + icon_gap + text_gap_x + text_gap_x);

    if (xWidth > mMaxWidth)
    {
        mMaxWidth = xWidth;
    }

    if (size.y > mMaxHeight)
    {
        mMaxHeight = (s32)(text_gap_y + size.y);
    }
}


/// ---------------------------------------------------------------------------
/// Tests if a touch is *over* a menu strip item
/// ---------------------------------------------------------------------------
bool prMenu::InMenuItemRect(s32 x, s32 y, s32 width, s32 height, s32 xpos, s32 ypos)
{
    prRect rect = prRect(y, x, y + height, x + width);
    return rect.PointInside(xpos, ypos);
}


}}// Namespaces
