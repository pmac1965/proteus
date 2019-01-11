/**
 * prOpenALDeviceList.cpp
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


#include "prOpenALDeviceList.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"
#include <al.h>
#include <alc.h>


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prOpenALDeviceList::prOpenALDeviceList()
{
#if defined(SOUND_ALLOW)

    m_defaultDeviceIndex = 0;

    if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT"))
    {
        char *pDeviceList    = (char *)alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
        char *pDefaultDevice = (char *)alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
        s32   index          = 0;


        // Go through device list (each device terminated with a single nullptr, list terminated with double nullptr)
        while (*pDeviceList != NULL) 
        {
            // Default device?
            if (strcmp(pDefaultDevice, pDeviceList) == 0) 
            {
                m_defaultDeviceIndex = index;
            }


            // Get current device and add it to device list.
            ALCdevice *pDevice = alcOpenDevice(pDeviceList);
            if (pDevice) 
            {
                //prTrace(LogError, "Audio device %i: %s, Default: %s - Sources %i\n", index, pDeviceList, PRBOOL_TO_STRING(m_defaultDeviceIndex == index), GetNumSources());
                ALCcontext *pContext = alcCreateContext(pDevice, nullptr);
                if (pContext) 
                {
                    alcMakeContextCurrent(pContext);
                    
                    // if the actual device name isn't already in the list, then add it.
                    char *pActualDeviceName = (char*)alcGetString(pDevice, ALC_DEVICE_SPECIFIER);
                    PRASSERT(pActualDeviceName);                   
                    if (pActualDeviceName && *pActualDeviceName)
                    {
                        // Search for duplicate
                        bool newName = true;

                        for (s32 i=0; i<GetNumDevices(); i++)
                        {
                            if (strcmp(GetDeviceName(i), pActualDeviceName) == 0)
                            {
                                newName = false;
                            }
                        }

                        // Add to device list?
                        if (newName)
                        {
                            prAudioDeviceInfo info;
                            alcGetIntegerv(pDevice, ALC_MAJOR_VERSION, sizeof(int), &info.versionMajor);
                            alcGetIntegerv(pDevice, ALC_MINOR_VERSION, sizeof(int), &info.versionMinor);
                            info.deviceName = pActualDeviceName;
                            info.maxSources = GetNumSources();
                            m_deviceList.push_back(info);
                        }
                    }


                    alcMakeContextCurrent(nullptr);
                    alcDestroyContext(pContext);
                }


                alcCloseDevice(pDevice);
            }

            
            // Next
            pDeviceList += strlen(pDeviceList) + 1;
            index++;
        }
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prOpenALDeviceList::~prOpenALDeviceList()
{
}


/// ---------------------------------------------------------------------------
/// Returns the number of devices in the audio device list.
/// ---------------------------------------------------------------------------
s32 prOpenALDeviceList::GetNumDevices() const
{
#if defined(SOUND_ALLOW)
    return (s32)m_deviceList.size();

#else
    return 0;

#endif
}


/// ---------------------------------------------------------------------------
/// Returns the device name at a specified index in the audio device list.
/// ---------------------------------------------------------------------------
char *prOpenALDeviceList::GetDeviceName(s32 index)
{
#if defined(SOUND_ALLOW)

    if (index < GetNumDevices())
    {
        return (char*)m_deviceList[index].deviceName.c_str();
    }

#else

    PRUNUSED(index);

#endif

    return nullptr;
}


/// ---------------------------------------------------------------------------
/// Returns the maximum number of Sources that can be generate on the given device
/// ---------------------------------------------------------------------------
u32 prOpenALDeviceList::GetMaxSources(int index) const
{
    if (index < GetNumDevices())
    {
        return m_deviceList[index].maxSources;
    }
    else
    {
        return 0;
    }
}



/// ---------------------------------------------------------------------------
/// Internal function to detemine max number of Sources that can be generated
/// ---------------------------------------------------------------------------
u32 prOpenALDeviceList::GetNumSources() const
{
    // Clear AL Error Code
    alGetError();

    // Generate up to 256 Sources, checking for any errors
    ALuint sources[256];
    u32    sourceCount;

    memset(sources, 0, sizeof(sources));

    for (sourceCount = 0; sourceCount < 256; sourceCount++)
    {
        alGenSources(1, &sources[sourceCount]);
        if (alGetError() != AL_NO_ERROR)
            break;
    }

    // Release the Sources
    alDeleteSources(sourceCount, sources);
    if (alGetError() != AL_NO_ERROR)
    {
        for (u32 i = 0; i < 256; i++)
        {
            alDeleteSources(1, &sources[i]);
        }
    }

    return sourceCount;
}


#endif//PLATFORM_PC
