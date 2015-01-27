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
//#include "../core/prCore.h"
//#include "../core/prRegistry.h"
//#include "../display/prRenderer.h"


// Namespaces
namespace Proteus {
namespace Gui {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenu::prMenu(const char *name, prSpriteManager *pSpriteManager) : prWidget(WT_Menu, name, pSpriteManager)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prMenu::~prMenu()
{
}


/// ---------------------------------------------------------------------------
/// Updates the menu strip.
/// ---------------------------------------------------------------------------
void prMenu::Update(f32 dt)
{
}


/// ---------------------------------------------------------------------------
/// Draw the dialog.
/// ---------------------------------------------------------------------------
void prMenu::Draw()
{
    /*prRenderer *pRenderer = (prRenderer *)prCoreGetComponent(PRSYSTEM_RENDERER);
    if (pRenderer)
    {
        pRenderer->TexturesEnabled(false);

        pRenderer->SetColour(prColour::LiteGray);
        pRenderer->DrawFilledRect(0, 0, mScreenWidth, 28);

        pRenderer->SetColour(prColour::White);
        pRenderer->TexturesEnabled(true);
    }*/
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenu::OnPressed(const prTouchEvent &e)
{
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenu::OnMove(const prTouchEvent &e)
{
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prMenu::OnReleased(const prTouchEvent &e)
{
}


}}// Namespaces
