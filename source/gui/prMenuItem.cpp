// File: prMenuItem.cpp
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


#include "prMenuItem.h"


using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Gui {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenuItem::prMenuItem(prString text, u32 id) : mText(text), mId(id)
{
    mIcon    = nullptr;
    mKeys    = 0;
    mEnabled = PRTRUE;
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenuItem::prMenuItem(const char *text, Proteus::Core::u32 id) : mText(text), mId(id)
{
    mIcon    = nullptr;
    mKeys    = 0;
    mEnabled = PRTRUE;
}


/// ---------------------------------------------------------------------------
/// Updates the menu item
/// ---------------------------------------------------------------------------
void prMenuItem::Update()
{
}


/// ---------------------------------------------------------------------------
/// Draws the menu item
/// ---------------------------------------------------------------------------
void prMenuItem::Draw()
{
}


}}// Namespaces
