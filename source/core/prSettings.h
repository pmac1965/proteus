// File: prSettings.h
// Description:
//      Contains a class to load and save a simple XML based settings file.
//
// Notes:
//      Allows for multiple settings files.
//
// Notes:
//      As you can supply the folder/filename you can use the class for more than settings,
//      but this is *not* recommended, as well as its not its intended use
//
// Warning:
//      *Do not use for game saves. The XML files are not secure and human readable*
//
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


#pragma once


#include "../prConfig.h"
#include "../tinyxml/tinyxml.h"
#include <string>
#include <map>


// Class: prSettingsCallbacks
//      A mix-in class to receive notifications about settings
//
// Notes:
//      This class is optional
class prSettingsCallbacks
{
public:
    // Method: SettingLoaded
    //      Received when a setting is loaded
    //
    // Parameters:
    //      key   - Name of the setting
    //      value - Default value the setting
    virtual void SettingLoaded(const char *key, const char *value) = 0;

    // Method: SettingChanged
    //      Received when a setting is changed
    //
    // Parameters:
    //      key   - Name of the setting
    //      value - Default value the setting
    virtual void SettingChanged(const char *key, const char *value) = 0;
};


// Class: prSettings
//      A class which allows the saving and loading of XML based settings files.
class prSettings
{
public:
    // Method: prSettings
    //      Ctor
    //
    // Parameters:
    //      folder     - The folder where to store the settings (This is usually the apps name)
    //      filename   - The filename. e.g settings.xml
    //      pCallbacks - Optional callbacks handler
    //
    // Notes:
    //      *folder and filename must not be null or empty. As this will cause an assertion*
    explicit prSettings(const char *folder, const char *filename, prSettingsCallbacks *pCallbacks = nullptr);
    
    // Method: ~prSettings
    //      Dtor
    ~prSettings();

    // Method: Add
    //      Adds a default setting and its value
    //
    // Parameters:
    //      key   - Name of the setting
    //      value - Default value the setting
    //
    // Notes:
    //      The default settings should be added at game start if they are not loaded.
    //
    // Notes:
    //      *key and key must not be null or empty. As this will cause an assertion*
    void Add(const char *key, const char *value);
    
    // Method: Save
    //      Save the current settings
    void Save();

    // Method: Load
    //      Load the current settings
    bool Load();

    // Method: GetValueForKey
    //      Fetches a settings value.
    //
    // Parameters:
    //      key   - Name of the setting
    const char *GetValueForKey(const char *key);

    // Method: SetValueForKey
    //      Sets a settings value.
    //
    // Parameters:
    //      key   - Name of the setting
    //      value - Default value the setting
    void SetValueForKey(const char *key, const char *value);

    // Method: Display
    //      Debug function which displays the current settings
    //
    // Notes:
    //      Does nothing on release builds
    void Display();


private:
#if defined(PLATFORM_PC)                    // Isolate until we do for other platforms
    // Generate the filename
    void GenerateFileName(char *pBuffer);

    // Generate the folder name
    void GenerateFolderName(char *pBuffer);
#endif

    // Setting file reading
    void ParseFile(TiXmlNode* pParent);
    void ParseAttribs_Setting(TiXmlElement* pElement);

    // Test if a setting exists
    bool Exists(std::string key);


private:    
    std::string                         mFolder;
    std::string                         mFilename;
    std::map<std::string, std::string>  values;
    prSettingsCallbacks                *mprSettingsCallbacks;
    bool                                mCorrectFileType;
};
