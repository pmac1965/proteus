/**
 * prPane.cpp
 *
 *  Copyright 2016 Paul Michael McNab
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


#include "prPane.h"
#include "../core/prCore.h"
//#include "../core/prRegistry.h"
#include "../display/prRenderer.h"


//namespace Proteus::Core;
using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Gui {

    /// ---------------------------------------------------------------------------
    /// Ctor
    /// ---------------------------------------------------------------------------
    prPane::prPane(const char *name, prSpriteManager *pSpriteManager) : prWidget    (WT_Pane, name, pSpriteManager)
                                                                      //, m_textColour(prColour::White)
    {
    }


    /// ---------------------------------------------------------------------------
    ///
    /// ---------------------------------------------------------------------------
    void prPane::Update(f32 dt)
    {
    }


    /// ---------------------------------------------------------------------------
    /// Draws the widget
    /// ---------------------------------------------------------------------------
    void prPane::Draw()
    {
        prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
        if (pRenderer)
        {
            // Draw strip
            pRenderer->TexturesEnabled(false);
            pRenderer->SetColour(prColour::LiteGray);
            pRenderer->DrawFilledRect(mXpos, mYpos, mXpos + mWidth, mYpos + mHeight);
            pRenderer->TexturesEnabled(true);

            // Draw text
            /*f32 xpos = mStartX;
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
                    
            //            //pRenderer->SetColour(prColour::LiteGray);
            //            pRenderer->TexturesEnabled(true);
            //        }
            //        #endif

            //        pRenderer->SetColour(prColour::White);
            //        m_pBmpfont->Draw(xpos, ypos, name);

            //        xpos += (size.x + MS_XPIXEL_BUFFER);
            //    }
            //    else if (m_pTtfFont)
            //    {
            //    }
            //}//*/
        }

    }////


    /// ---------------------------------------------------------------------------
    /// A touch event handler
    /// ---------------------------------------------------------------------------
    void prPane::OnPressed(const prTouchEvent &e)
    {}


    /// ---------------------------------------------------------------------------
    /// A touch event handler
    /// ---------------------------------------------------------------------------
    void prPane::OnMove(const prTouchEvent &e)
    {}


    /// ---------------------------------------------------------------------------
    /// A touch event handler
    /// ---------------------------------------------------------------------------
    void prPane::OnReleased(const prTouchEvent &e)
    {}


}} // Namespaces
