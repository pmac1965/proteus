/**
 * prSettings.cpp
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


#include "prSettings.h"
#include "prStringUtil.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../file/prFileSystem.h"


#if defined(PLATFORM_PC)
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#endif


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSettings::prSettings(const char *folder, const char *filename, prSettingsCallbacks *pCallbacks) : mFolder                 (folder)
                                                                                                  , mFilename               (filename)
                                                                                                  , mprSettingsCallbacks    (pCallbacks)
{
    PRASSERT(folder && *folder);
    PRASSERT(filename && *filename);
    mCorrectFileType = false;

    TODO("Make work across all platforms")
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSettings::~prSettings()
{
}


/// ---------------------------------------------------------------------------
/// Add a default value
/// ---------------------------------------------------------------------------
void prSettings::Add(const char *key, const char *value)
{
    PRASSERT(key && *key);
    PRASSERT(value && *value);

    if (Exists(key))
    {
        prTrace(LogError, "Can't add value %s twice\n", key);
    }
    else
    {
        //prTrace(LogError, "Added setting:  %s, %s\n", key, value); 
        values.insert
        (
            std::pair<std::string, std::string>(key, value)
        );
    }
}


/// ---------------------------------------------------------------------------
/// Save the current settings
/// ---------------------------------------------------------------------------
void prSettings::Save()
{
#if defined(PLATFORM_PC)

    char filename[MAX_PATH] = { '\0' };
    GenerateFileName(filename);


    // Validate file and location.
    if (prFileExist(filename))
    {
        prFileRemoveReadOnly(filename);
    }
    else
    {
        // Else, create the files directory
        GenerateFolderName(filename);

        // Make the directory
        CreateDirectoryA(filename, NULL);

        GenerateFileName(filename);
    }


    // Create the doc header
	TiXmlDocument doc;  
 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");  
	doc.LinkEndChild(decl);  

    // Add root
    TiXmlElement *root = new TiXmlElement("settings_file");
    root->SetAttribute("version", "1.0.0");
    doc.LinkEndChild(root);

    // Write the variables
    std::map<std::string, std::string>::iterator it;
    for (it = values.begin(); it != values.end(); ++it)
    {
        TiXmlElement *element = new TiXmlElement( "setting" );
	    root->LinkEndChild(element);
        element->SetAttribute("key",   ((*it).first).c_str());
        element->SetAttribute("value", ((*it).second).c_str());
    }

    // Now save
    doc.SaveFile(filename);

#endif
}


/// ---------------------------------------------------------------------------
/// Load new settings
/// ---------------------------------------------------------------------------
bool prSettings::Load()
{
    bool result = false;

#if defined(PLATFORM_PC)

    char filename[MAX_PATH] = { '\0' };    
    GenerateFileName(filename);


    // Parse the document
    TiXmlDocument* doc = new TiXmlDocument(filename);
    if (doc)
    {
        bool loaded = doc->LoadFile();      
        if (loaded)
        {
            result = true;
            ParseFile(doc);
        }
        else
        {
            prTrace(LogError, "Failed to Load %s\n", filename);
        }

        delete doc;
    }

#endif

    return (result && mCorrectFileType);
}


/// ---------------------------------------------------------------------------
/// Fetches a settings value.
/// ---------------------------------------------------------------------------
const char *prSettings::GetValueForKey(const char *key)
{
    const char *value = NULL;
    
    if (key && *key)
    {
        if (Exists(key))
        {
            std::string akey(key);
        
            for (auto it = values.begin(); it != values.end(); ++it)
            {
                if (akey.compare((*it).first) == 0)
                {
                    value = (*it).second.c_str();
                    break;
                }
            }
        }
    }

    return value;
}


/// ---------------------------------------------------------------------------
/// Sets a settings value.
/// ---------------------------------------------------------------------------
void prSettings::SetValueForKey(const char *key, const char *value)
{
    if (key && *key)
    {
        if (Exists(key))
        {
            std::string akey(key);
        
            for (auto it = values.begin(); it != values.end(); ++it)
            {
                if (akey.compare((*it).first) == 0)
                {
                    (*it).second = value;
                     
                    if (mprSettingsCallbacks)
                    {
                        mprSettingsCallbacks->SettingChanged(key, value);
                    }
                    break;
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Generate the filename
/// ---------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void prSettings::GenerateFileName(char *pBuffer)
{
    if (pBuffer)
    {
        if (SHGetFolderPathA(HWND_DESKTOP, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, pBuffer) != S_OK)
        {
            prTrace(LogError, "Failed to acquire save/load path.\n");
            *pBuffer = 0;
        }
        else
        {
            strcat_s(pBuffer, MAX_PATH, "\\");
            strcat_s(pBuffer, MAX_PATH, mFolder.c_str());
            strcat_s(pBuffer, MAX_PATH, "\\");
            strcat_s(pBuffer, MAX_PATH, mFilename.c_str());
        }
    }
}
#endif


/// ---------------------------------------------------------------------------
/// Generate the folder name
/// ---------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void prSettings::GenerateFolderName(char *pBuffer)
{
    if (pBuffer)
    {
        if (SHGetFolderPathA(HWND_DESKTOP, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, pBuffer) != S_OK)
        {
            prTrace(LogError, "Failed to acquire save/load path.\n");
            *pBuffer = 0;
        }
        else
        {
            strcat_s(pBuffer, MAX_PATH, "\\");
            strcat_s(pBuffer, MAX_PATH, mFolder.c_str());
        }
    }
}
#endif


/// ---------------------------------------------------------------------------
/// Setting file reading
/// ---------------------------------------------------------------------------
void prSettings::ParseFile(TiXmlNode* pParent)
{
    switch (pParent->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        {
            // File type
            if (prStringCompare(pParent->Value(), "settings_file") == 0)
            {
                // Correct file type
                mCorrectFileType = true;
            }
            // A setting
            else if (prStringCompare(pParent->Value(), "setting") == 0)
            {
                ParseAttribs_Setting(pParent->ToElement());
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
/// Attribute parser - Used to get information about a variable
/// ---------------------------------------------------------------------------
void prSettings::ParseAttribs_Setting(TiXmlElement* pElement)
{
    PRASSERT(pElement);
    if (pElement)
    {
        // Sanity checks
        const char *key   = pElement->Attribute("key");
        const char *value = pElement->Attribute("value");
        PRASSERT(key);
        PRASSERT(value);
        
        Add(key, value);
        
        if (mprSettingsCallbacks)
        {
            mprSettingsCallbacks->SettingLoaded(key, value);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Test if a setting exists
/// ---------------------------------------------------------------------------
bool prSettings::Exists(std::string key)
{
    bool result = false;
        
    for (auto it = values.begin(); it != values.end(); ++it)
    {
        if (key.compare((*it).first) == 0)
        {
            result = true;
            break;
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Debug function which displays the current settings
/// ---------------------------------------------------------------------------
void prSettings::Display()
{
    prTrace(LogDebug, "----------------------------------------------------------------\n");
    prTrace(LogDebug, "Settings:\n", values.size());
    prTrace(LogDebug, "----------------------------------------------------------------\n");

    for (auto it = values.begin(); it != values.end(); ++it)
    {
        prTrace(LogDebug, "%s, %s\n", (*it).first.c_str(),  (*it).second.c_str());
    }

    prTrace(LogDebug, "----------------------------------------------------------------\n");
}
