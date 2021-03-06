// File: prLocales.h
// About:
//      The supported locales
//
//  - Proteus::Locale::EN_US    English = United States
//  - Proteus::Locale::EN_GB    English = United Kingdom
//  - Proteus::Locale::FR_FR    French  = France
//  - Proteus::Locale::IT_IT    Italian = Italy
//  - Proteus::Locale::DE_DE    German  = German
//  - Proteus::Locale::ES_ES    Spanish = Spain
//  - Proteus::Locale::ZH_CN    Chinese = China
/**
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


#pragma once


#include "../core/prTypes.h"


namespace Proteus
{
    namespace Locale
    {
        enum
        {
            EN_US,      // English - United States
            EN_GB,      // English - United Kingdom
            FR_FR,      // French
            IT_IT,      // Italian
            DE_DE,      // German
            ES_ES,      // Spanish
            ZH_CN,      // Chinese
            MAX,        // Max languages
        };
    }
}


// Function: prGetPlatformsLanguage
//      Get the platforms default language.
//
// Returns:
//      Proteus::Locale::EN_US - United States
//      Proteus::Locale::EN_GB - United Kingdom
//      Proteus::Locale::FR_FR - France
//      Proteus::Locale::IT_IT - Italy
//      Proteus::Locale::DE_DE - German
//      Proteus::Locale::ES_ES - Spain
//      Proteus::Locale::ZH_CN - China
s32 prGetPlatformsLanguage();

// Function: prGetPlatformsLanguageString
//      Get the platforms default language as a string.
//
// Returns:
//       - "EN_US"
//       - "EN_GB"
//       - "FR_FR"
//       - "IT_IT"
//       - "DE_DE"
//       - "ES_ES"
//       - "ZH_CN"
const char *prGetPlatformsLanguageString();
