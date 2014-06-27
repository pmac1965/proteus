/**
 * prFontManager.cpp
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


#if defined(PLATFORM_PC)


// FreeType Headers
#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>


#include "prFontManager.h"
#include "../core/prCore.h"
#include "../core/prDefines.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prFontManager::prFontManager() : prCoreSystem(PRSYSTEM_RENDERER, "prFontManager")
{
    FT_Library library = NULL;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFontManager::~prFontManager()
{
}


/// ---------------------------------------------------------------------------
/// Load a font
/// ---------------------------------------------------------------------------
void prFontManager::Load(const char *fontname)
{
}


#endif
