/**
 * prLocales.cpp
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
  #include <windows.h>

#elif defined(PLATFORM_IOS)
    // To be added

#elif defined(PLATFORM_ANDROID)
    // To be added

#elif defined(PLATFORM_LINUX)
    // To be added

#elif defined(PLATFORM_MAC)
    // To be added

#else
  #error "Unsupported platform."

#endif


#include "prLocales.h"
#include "../debug/prTrace.h"


//using namespace Proteus::Core;


namespace
{
    const char *lang_string = "";
}


#if defined(PLATFORM_PC)
    /// -----------------------------------------------------------------------
    /// Get the platforms default language.
    /// -----------------------------------------------------------------------
    s32 prGetPlatformsLanguage()
    {
        int lang;

        // Localisation
        LCID locale   = GetUserDefaultLCID();
        WORD language = PRIMARYLANGID(locale);
        WORD sublang  = SUBLANGID(locale);

        switch(language)
        {
        case LANG_ENGLISH:
            if (sublang == SUBLANG_ENGLISH_US)
            {
                lang_string = "EN_US";
                lang = Proteus::Locale::EN_US;
            }
            else
            {
                lang_string = "EN_GB";
                lang = Proteus::Locale::EN_GB;
            }
            break;

        case LANG_FRENCH:
            lang_string = "FR_FR";
            lang = Proteus::Locale::FR_FR;
            break;

        case LANG_ITALIAN:
            lang_string = "IT_IT";
            lang = Proteus::Locale::IT_IT;
            break;

        case LANG_SPANISH:
            lang_string = "ES_ES";
            lang = Proteus::Locale::ES_ES;
            break;

        case LANG_GERMAN:
            lang_string = "DE_DE";
            lang = Proteus::Locale::DE_DE;
            break;

        case LANG_CHINESE_SIMPLIFIED:
            lang_string = "ZH_CN";
            lang = Proteus::Locale::ZH_CN;
            break;

        case LANG_CHINESE_TRADITIONAL:
            lang_string = "ZH_CN";
            lang = Proteus::Locale::ZH_CN;
            break;

        default:
            prTrace(prLogLevel::LogError, "Platform language: Unknown. Defaulting to English\n");
            lang_string = "EN_US";
            lang = Proteus::Locale::EN_US;
            break;
        }

        prTrace(prLogLevel::LogError, "Platform language: %s\n", lang_string);
        return lang;
    }

#elif defined(PLATFORM_IOS)
    /// -----------------------------------------------------------------------
    /// Get the platforms default language.
    /// -----------------------------------------------------------------------
    s32 prGetPlatformsLanguage()
    {
        return Proteus::Locale::EN_US;
    }

#elif defined(PLATFORM_ANDROID)
    /// -----------------------------------------------------------------------
    /// Get the platforms default language.
    /// -----------------------------------------------------------------------
    s32 prGetPlatformsLanguage()
    {
        return Proteus::Locale::EN_US;
    }

#elif defined(PLATFORM_LINUX)
    /// -----------------------------------------------------------------------
    /// Get the platforms default language.
    /// -----------------------------------------------------------------------
    s32 prGetPlatformsLanguage()
    {
        return Proteus::Locale::EN_US;
    }

#elif defined(PLATFORM_MAC)
    /// -----------------------------------------------------------------------
    /// Get the platforms default language.
    /// -----------------------------------------------------------------------
    s32 prGetPlatformsLanguage()
    {
        return Proteus::Locale::EN_US;
    }

#else
    #error Platform not defined.

#endif


/// ---------------------------------------------------------------------------
/// Get the platforms default language as a string.
/// ---------------------------------------------------------------------------
const char *prGetPlatformsLanguageString()
{
    return lang_string;
}
