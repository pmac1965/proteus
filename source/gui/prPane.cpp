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
#include "../core/prRegistry.h"
#include "../debug/prTrace.h"
#include "../display/prRenderer.h"
#include "../display/prBitmapFont.h"
#include "../display/prTrueTypeFont.h"
#include "../display/prSprite.h"


#if defined(PLATFORM_PC)
  #include "../core/prWindow_PC.h"
#endif

//namespace Proteus::Core;
using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Gui {

    /// ---------------------------------------------------------------------------
    /// Ctor
    /// ---------------------------------------------------------------------------
    prPane::prPane(const char *name, prSpriteManager *pSpriteManager) : prWidget    (WT_Pane, name, pSpriteManager)
																	  , mName		("Unnamed")
    {
		mXpos			= 32;			// Default settings which should be overriden 
		mYpos			= 32;
		mWidth			= 256;
		mHeight			= 256;
		mpPaneIcon		= nullptr;
		mpOptionsIcon	= nullptr;
	}


    /// ---------------------------------------------------------------------------
    /// Update the pane
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
            // Get the screens details
            prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            PRASSERT(pReg);
            s32 scrnHeight = atoi(pReg->GetValue("ScreenHeight"));

#if defined(PLATFORM_PC)
            glScissor(mXpos, scrnHeight - (mYpos + mHeight), mWidth, mHeight);
            glEnable(GL_SCISSOR_TEST);
#endif

            // Draw pane
            pRenderer->TexturesEnabled(false);
            pRenderer->SetColour(prColour::LiteGray);
            pRenderer->DrawFilledRect((f32)mXpos, (f32)mYpos, (f32)(mXpos + mWidth), (f32)(mYpos + mHeight));
            pRenderer->SetColour(prColour(.85f, .85f, .85f));
            pRenderer->DrawFilledRect((f32)mXpos, (f32)mYpos, (f32)(mXpos + mWidth), (f32)(mYpos + 18));
            pRenderer->TexturesEnabled(true);

			// Draw pane icon if we have one
			s32 xpos = mXpos + 2;				// + 2 == edge gap
			s32 ypos = mYpos + 2;				// + 2 == edge gap

			if (mpPaneIcon)
			{
				mpPaneIcon->pos.x = (f32)xpos;
				mpPaneIcon->pos.y = (f32)ypos;
				mpPaneIcon->Draw();
				xpos += mpPaneIcon->GetFrameWidth();
			}

            // Draw pane title text
			if (m_pBmpfont)
			{
				pRenderer->SetColour(prColour::Black);
				m_pBmpfont->Draw((f32)xpos, (f32)mYpos, mName.Text());
			}

			// Draw default pane icons
			if (mpOptionsIcon)
			{
				mpOptionsIcon->pos.x = (f32)((mXpos + mWidth) - mpOptionsIcon->GetFrameWidth());
				mpOptionsIcon->pos.y = (f32)ypos;
				mpOptionsIcon->Draw();
			}

#if defined(PLATFORM_PC)
            glDisable(GL_SCISSOR_TEST);
#endif
        }
    }


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
