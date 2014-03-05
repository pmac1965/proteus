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
 *
 */


#include "../prConfig.h"


#include "prLanguage.h"
#include "../core/prStringUtil.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"


// Stores info about a specific locale.
typedef struct LocaleInfo
{
    u32 hash;
    u32 locale;

} LocaleInfo;


//// ----------------------------------------------------------------------------
//// Implementation data.
//// ----------------------------------------------------------------------------
//typedef struct LanguageImplementation
//{
//
//    // Finds a string by its name.
//    //
//    const char *Find(const char *name)
//    {
//        if (count > 0)
//        {
//            u32 hash = prStringHash(name);
//
//            // ...
//            int  lower = 0;
//            int  upper = count - 1;
//            //int  mid;
//
//            while(lower <= upper)
//            {
//                int mid = (lower + upper) / 2;
//
//                if (hash > pStringTableEntries[mid]->hash)
//                {
//                    lower = mid + 1;
//                }
//                else if (hash < pStringTableEntries[mid]->hash)
//                {
//                    upper = mid - 1;
//                }
//                else
//                {
//                    return pStringTableEntries[mid]->text[language];
//                }
//            }
//        }
//
//        prTrace("Failed to find string: %s\n", name);
//        return name;
//    }
//
//
//    // Implementation data
//    s32                     language;
//    s32                     count;
//    bool                    correctFileType;
//    bool                    exp0;
//    bool                    exp1;
//    bool                    exp2;
//    StringTableEntry      **pStringTableEntries;
//    //List<StringTableEntry*> entries;
//
//} LanguageImplementation;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prLanguage::prLanguage()
{
    language            = prGetPlatformsLanguage();
    count               = 0;
    correctFileType     = PRFALSE;
    pStringTableEntries = NULL;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prLanguage::~prLanguage()
{
    for (int i=0; i<count; i++)
    {
        PRSAFE_DELETE(pStringTableEntries[i]);
    }

    PRSAFE_DELETE_ARRAY(pStringTableEntries);
}


/// ---------------------------------------------------------------------------
/// Resets the language system.
/// ---------------------------------------------------------------------------
void prLanguage::Reset()
{
    for (int i=0; i<count; i++)
    {
        PRSAFE_DELETE(pStringTableEntries[i]);
    }

    PRSAFE_DELETE_ARRAY(pStringTableEntries);

    language            = prGetPlatformsLanguage();
    count               = 0;
    correctFileType     = PRFALSE;
}


/// ---------------------------------------------------------------------------
/// Loads the language data.
/// ---------------------------------------------------------------------------
void prLanguage::Load(const char *filename)
{
    PRASSERT(filename && *filename);

    // Parse the document
    TiXmlDocument* doc = new TiXmlDocument(filename);
    if (doc)
    {
        bool loaded = doc->LoadFile();      
        if (loaded)
        {
            prTrace("Loaded language file: %s\n", filename);
            ParseFile(doc);
        }
        else
        {
            PRWARN("Failed to Load %s\n", filename);
        }

        delete doc;
    }


    // Store the sorted data table.
    count = entries.Size();
    if (count > 0)
    {
        pStringTableEntries = new prStringTableEntry*[count];

        prList<prStringTableEntry*>::prIterator it = entries.Begin();            
            
        int index = 0;
        while(it.Okay())
        {
            pStringTableEntries[index++] = *it;
            ++it;
        }

        entries.Clear();
    }
}


/// ---------------------------------------------------------------------------
/// Sets the language used.
/// ---------------------------------------------------------------------------
void prLanguage::Set(s32 lang)
{
    switch(lang)
    {
    case Proteus::Locale::EN_GB:
    case Proteus::Locale::EN_US:
    case Proteus::Locale::FR_FR:
    case Proteus::Locale::IT_IT:
    case Proteus::Locale::DE_DE:
    case Proteus::Locale::ES_ES:
    case Proteus::Locale::ZH_CN:
        language = lang;
        break;

    default:
        PRWARN("Attempted to set an invalid language");
        break;
    }
}


/// ---------------------------------------------------------------------------
/// Gets the language used.
/// ---------------------------------------------------------------------------
s32 prLanguage::Get() const
{
    return language;
}


/// ---------------------------------------------------------------------------
/// Gets a localised string.
/// ---------------------------------------------------------------------------
const char *prLanguage::GetString(const char *name)
{
    if (count > 0)
    {
        u32 hash  = prStringHash(name);
        s32 lower = 0;
        s32 upper = count - 1;

        while(lower <= upper)
        {
            s32 mid = (lower + upper) / 2;

            if (hash > pStringTableEntries[mid]->hash)
            {
                lower = mid + 1;
            }
            else if (hash < pStringTableEntries[mid]->hash)
            {
                upper = mid - 1;
            }
            else
            {
                return pStringTableEntries[mid]->text[language];
            }
        }
    }

    prTrace("Failed to find string: %s\n", name);
    return name;
}


/// ---------------------------------------------------------------------------
/// Parses the xml file.
/// ---------------------------------------------------------------------------
void prLanguage::ParseFile(TiXmlNode* pParent)
{
    switch (pParent->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        {
            // File data
            if (strcmp(pParent->Value(), "translations_file") == 0)
            {
                ParseAttribs_File(pParent->ToElement());
            }
            // Entries
            else if (strcmp(pParent->Value(), "entry") == 0)
            {
                ParseAttribs_Entry(pParent->ToElement());
            }
        }
        break;

    default:
	    break;
    } 


    for (TiXmlNode *pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        ParseFile(pChild);
    }
}


/// ---------------------------------------------------------------------------
/// Attribute parser - Used to get information from the file like the version number.
/// ---------------------------------------------------------------------------
void prLanguage::ParseAttribs_File(TiXmlElement* pElement)
{
    PRASSERT(pElement);
    if (pElement)
    {
        PRASSERT(pElement->Attribute("version"));
        prTrace("Language: File version: %s\n", pElement->Attribute("version"));
    }

    // Indicate correct file type.
    correctFileType = PRTRUE;
}


/// ---------------------------------------------------------------------------
/// Parses the entries.
/// ---------------------------------------------------------------------------
void prLanguage::ParseAttribs_Entry(TiXmlElement* pElement)
{
    static const LocaleInfo locales[] = 
    {
        { prStringHash("en-US"),   Proteus::Locale::EN_US },
        { prStringHash("en-GB"),   Proteus::Locale::EN_GB },
        { prStringHash("fr-FR"),   Proteus::Locale::FR_FR },
        { prStringHash("it-IT"),   Proteus::Locale::IT_IT },
        { prStringHash("de-DE"),   Proteus::Locale::DE_DE },
        { prStringHash("es-ES"),   Proteus::Locale::ES_ES },
        { prStringHash("zh-CN"),   Proteus::Locale::ZH_CN },
    };

    PRASSERT(correctFileType);
    PRASSERT(pElement);

    if (pElement)
    {
        // Set string ID
        const char *id = pElement->Attribute("enum");
        PRASSERT(id);

        // Create new entry and hash it.
        prStringTableEntry *entry = new prStringTableEntry();
        PRASSERT(entry);
        entry->hash = prStringHash(id);

        // Acquire the string data.
        TiXmlHandle root(pElement);
        TiXmlElement *pElem = root.FirstChild("string").Element();
        for (; pElem; pElem = pElem->NextSiblingElement())
        {
            const char *locale = pElem->Attribute("locale");
            const char *text   = pElem->Attribute("text");                
            PRASSERT(locale);
            PRASSERT(text);

            if (locale && text)
            {
                u32 hash = prStringHash(locale);

                // Find each locale.
                for (u32 i=0; i<PRARRAY_SIZE(locales); i++)
                {
                    if (locales[i].hash == hash)
                    {
                        s32 locale = locales[i].locale;

                        entry->text[locale] = new char [strlen(text) + 1];

                        prStringCopy((char *)text, entry->text[locale]);
                        //Trace("> %s - %s - %i\n", entry->text[locale], text, strlen(text) + 1);
                        break;
                    }
                }
            }
        }

        // Add the entry.
        if (entries.Size() > 0)
        {
            prList<prStringTableEntry*>::prIterator it = entries.Begin();

            bool added = false;

            while(it.Okay())
            {
                // Is a duplicate?
                if (entry->hash == (*it)->hash)
                {
                    prTrace("Language: Duplicate text entry: %s\n", id);
                    prTrace("Language: First text entries == %s, %s\n", entry->text[0], (*it)->text[0]);
                    PRSAFE_DELETE(entry);
                    return;
                }


                if (entry->hash < (*it)->hash)
                {
                    entries.Insert(it.Curr(), entry, INSERT_POS_BEFORE);
                    added = true;
                    break;
                }

                ++it;
            }

            if (!added)
            {
                entries.AddTail(entry);
            }
        }
        else
        {
            entries.AddHead(entry);
        }
    }
}
