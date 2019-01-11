// File: prOpenALDeviceList.h
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


#include <vector>
#include <string>
#include "../core/prTypes.h"


// Structs
typedef struct prAudioDeviceInfo
{
    std::string deviceName;
    s32         versionMajor;
    s32         versionMinor;
    u32         maxSources;

} prAudioDeviceInfo;


// Class: prOpenALDeviceList
//      A class which generates information about the attached audio devices.
class prOpenALDeviceList
{
public:
    // Method: prOpenALDeviceList
    //      Ctor
    prOpenALDeviceList();

    // Method: ~prOpenALDeviceList
    //      Dtor
    ~prOpenALDeviceList();

    // Method: GetNumDevices
    //      Returns the number of devices in the audio device list.
    s32 GetNumDevices() const;

    // Method: GetDeviceName
    //      Returns the device name at a specified index in the audio device list.
    char *GetDeviceName(s32 index);

    // Method: GetMaxSources
    //      Returns the maximum number of sources that can be generated on the given device.
    u32 GetMaxSources(int index) const;


private:
    // Internal function to detemine max number of Sources that can be generated
    u32 GetNumSources() const;


private:
  std::vector<prAudioDeviceInfo>    m_deviceList;
  s32                               m_defaultDeviceIndex;
};
