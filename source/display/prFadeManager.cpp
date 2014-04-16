/**
 * prFadeManager.cpp
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
 *
 */


#include "../prConfig.h"


// Platform specifics.
#if defined(PLATFORM_PC)
    #include <Windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
    #include <OpenGLES/ES1/gl.h>
    #include <cstdlib>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <cstdlib>

#elif defined(PLATFORM_BADA)
    #include <FGraphicsOpengl.h>
    #include <cstdlib>
    using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <cstdlib>

#else
  #error No platform defined.

#endif


#include "prFadeManager.h"
#include "prTexture.h"
#include "prRenderer.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"


#define MINIMUM_FADE_TIME   17.0f


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prFadeManager::prFadeManager() : prCoreSystem(PRSYSTEM_FADEMANAGER, "prFadeManager")
{
    Reset();
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFadeManager::~prFadeManager()
{
}


/// ---------------------------------------------------------------------------
/// Updates the fade.
/// ---------------------------------------------------------------------------
void prFadeManager::Update(float dt)
{
    if (fading)
    {
        time -= dt;

        if (time < 0.0f)
        {
            fading = false;
            time   = 0.0f;
            step   = 0.0f;
            alpha  = (type == FadeTypeColour) ? 255.0f : 0.0f;
        }
        else
        {
            alpha = time / step;
            alpha = (type == FadeTypeColour) ? (255.0f - alpha) : alpha;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Draws the fade.
/// ---------------------------------------------------------------------------
void prFadeManager::Draw()
{
    if (alpha > 0.0f)
    {
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        if (colour == FadeColourBlack)
        {
            glColor4f(0.0f, 0.0f, 0.0f, alpha / 255.0f);
        }
        else
        {
            float a = alpha / 255.0f;
            glColor4f(a, a, a, a);
        }

        prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
        if (pRenderer)
        {
            // Get screen size.
            prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (pReg)
            {
                f32 scrnWidth  = (f32)atof(pReg->GetValue("ScreenWidth"));
                f32 scrnHeight = (f32)atof(pReg->GetValue("ScreenHeight"));

                pRenderer->DrawFilledRect(0, 0, scrnWidth, scrnHeight);
            }
        }

        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glPopMatrix();
    }
}


/// ---------------------------------------------------------------------------
/// Starts a fade running.
/// ---------------------------------------------------------------------------
void prFadeManager::Start(prFade fade, float runtime)
{
    if (!fading)
    {
        // Check fade time
        PRASSERT(runtime > MINIMUM_FADE_TIME, "Fade time is too small!\n"
                                              "Possibly less than a single frame.");

        // Check start state
        PRASSERT(fade != FadeNone, "You can't start fading with 'Fade_None'\n\t\t  "
                                   "If you want to cancel a fade use Stop()");

        // Set
        if (fade == FadeToBlack)
        {
            colour = FadeColourBlack;
            time   = runtime;
            fading = true;
            step   = runtime / 255.0f;
            type   = FadeTypeColour;
        }
        else if (fade == FadeToWhite)
        {
            colour = FadeColourWhite;
            time   = runtime;
            fading = true;
            step   = runtime / 255.0f;
            type   = FadeTypeColour;
        }
        else if (fade == FadeToNormal)
        {
            time   = runtime;
            fading = true;
            step   = runtime / 255.0f;
            type   = FadeTypeNormal;
        }
        else
        {
            prTrace("Invalid fade state\n");
            Reset();
        }
    }
    else
    {
        prTrace("Attempted to start a fade operation while another fade was in progress!\n");
    }
}


/// ---------------------------------------------------------------------------
/// Determines if a fade is running.
/// ---------------------------------------------------------------------------
bool prFadeManager::Isfading() const
{
    return fading;
}


/// ---------------------------------------------------------------------------
/// Determines if the fade textures are visible.
/// ---------------------------------------------------------------------------
bool prFadeManager::IsFadeVisible() const
{
    return (alpha > 0.0001f);
}


/// ---------------------------------------------------------------------------
/// Resets the fade managers data to their default values.
/// ---------------------------------------------------------------------------
void prFadeManager::Reset()
{
    colour = FadeColourNone;
    type   = FadeTypeNone;
    alpha  = 0.0f;
    time   = 0.0f;
    step   = 0.0f;
    fading = false;
}


/// ---------------------------------------------------------------------------
/// Set to specific state.
/// ---------------------------------------------------------------------------
void prFadeManager::SetBlack(f32 in)
{
    colour = FadeColourBlack;
    alpha  = PRCLAMP(in, 0.0f, 255.0f);
}


/// ---------------------------------------------------------------------------
/// Set to specific state.
/// ---------------------------------------------------------------------------
void prFadeManager::SetWhite()
{
    TODO("Add in colour")

    colour = FadeColourWhite;
    alpha  = 255.0f;
}


/// ---------------------------------------------------------------------------
/// Set to specific state.
/// ---------------------------------------------------------------------------
void prFadeManager::SetNormal()
{
    Reset();
}
