// File: prLanguage.h
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


#ifndef __PRLANGUAGE_H
#define __PRLANGUAGE_H


#include "../core/prTypes.h"
#include "../core/prDefines.h"
#include "../core/prMacros.h"
#include "../core/prList.h"
#include "../tinyxml/tinyxml.h"
#include "prLocales.h"


// Class: prStringTableEntry
//      String table data.
class prStringTableEntry
{
public:
    // Method: prStringTableEntry
    //      Ctor
    prStringTableEntry()
    {
        for (int i=0; i<Proteus::Locale::MAX; i++)
        {
            text[i] = NULL;
        }

        hash = 0;
    }

    // Method: ~prStringTableEntry
    //      Dtor
    ~prStringTableEntry()
    {
        for (int i=0; i<Proteus::Locale::MAX; i++)
        {
            PRSAFE_DELETE_ARRAY(text[i]);
        }
    }


    u32   hash;
    char *text[Proteus::Locale::MAX];
};


// Class: prLanguage
//      Container for any loaded language data
class prLanguage
{
public:
    // Method: prLanguage
    //      Ctor
    prLanguage();

    // Method: ~prLanguage
    //      Dtor
    ~prLanguage();

    // Method: Reset
    //      Resets the language system and clears all data.
    //
    // Notes:
    //      After use the language will be set back to the default. All langauge data will be gone
    //
    //      Use with <Load> to reload the language you wish to use. Also call <Set>
    void Reset();

    // Method: Load
    //      Loads the language data.
    void Load(const char *filename);

    // Method: Set
    //      Sets the language used.
    //
    // Parameters:
    //      lang - Should be one of the values below 
    //
    //  - Proteus::Locale::EN_US    English = United States
    //  - Proteus::Locale::EN_GB    English = United Kingdom
    //  - Proteus::Locale::FR_FR    French  = France
    //  - Proteus::Locale::IT_IT    Italian = Italy
    //  - Proteus::Locale::DE_DE    German  = German
    //  - Proteus::Locale::ES_ES    Spanish = Spain
    //  - Proteus::Locale::ZH_CN    Chinese = China
    void Set(s32 lang);

    // Method: Load
    //      Gets the language used.
    s32 Get() const;

    // Method: GetString
    //      Gets a localised string.
    const char *GetString(const char *name) const;


private:
    // XML parser
    void ParseFile(TiXmlNode* pParent);

    // XML parser
    void ParseAttribs_File(TiXmlElement* pElement);

    // XML parser
    void ParseAttribs_Entry(TiXmlElement* pElement);


private:
    // Stops passing by value and assignment.
    prLanguage(const prLanguage&);
    const prLanguage& operator = (const prLanguage&);


private:
    s32                           language;                 // The language id.
    s32                           count;                    // Number of entries.
    PRBOOL                        correctFileType;          // Internal safety check
    prList<prStringTableEntry*>   entries;                  // Used to sort entries for binary search
    prStringTableEntry          **pStringTableEntries;      // The sorted entries.
};


#endif//__PRLANGUAGE_H
