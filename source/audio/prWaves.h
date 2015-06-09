// File: prWaves.h
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


// Max wave ID's
#define MAX_NUM_WAVEID          1024


// Enum: prWaveResult
//      Error codes
//
//  - WR_OK
//  - WR_INVALIDFILENAME
//  - WR_BADWAVEFILE
//  - WR_INVALIDPARAM
//  - WR_INVALIDWAVEID
//  - WR_NOTSUPPORTEDYET
//  - WR_WAVEMUSTBEMONO
//  - WR_WAVEMUSTBEWAVEFORMATPCM
//  - WR_WAVESMUSTHAVESAMEBITRESOLUTION
//  - WR_WAVESMUSTHAVESAMEFREQUENCY
//  - WR_WAVESMUSTHAVESAMEBITRATE
//  - WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT
//  - WR_OFFSETOUTOFDATARANGE
//  - WR_FILEERROR
//  - WR_OUTOFMEMORY
//  - WR_INVALIDSPEAKERPOS
//  - WR_INVALIDWAVEFILETYPE
//  - WR_NOTWAVEFORMATEXTENSIBLEFORMAT
enum prWaveResult
{
    WR_OK                               =   0,
    WR_INVALIDFILENAME                  = - 1,
    WR_BADWAVEFILE                      = - 2,
    WR_INVALIDPARAM                     = - 3,
    WR_INVALIDWAVEID                    = - 4,
    WR_NOTSUPPORTEDYET                  = - 5,
    WR_WAVEMUSTBEMONO                   = - 6,
    WR_WAVEMUSTBEWAVEFORMATPCM          = - 7,
    WR_WAVESMUSTHAVESAMEBITRESOLUTION   = - 8,
    WR_WAVESMUSTHAVESAMEFREQUENCY       = - 9,
    WR_WAVESMUSTHAVESAMEBITRATE         = -10,
    WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT  = -11,
    WR_OFFSETOUTOFDATARANGE             = -12,
    WR_FILEERROR                        = -13,
    WR_OUTOFMEMORY                      = -14,
    WR_INVALIDSPEAKERPOS                = -15,
    WR_INVALIDWAVEFILETYPE              = -16,
    WR_NOTWAVEFORMATEXTENSIBLEFORMAT    = -17
};


// Types
typedef int prWaveID;


// Forward declarations
struct prWaveFileInfo;


// Class: prWaves
//      This class is created by the sound manager. Do not create another instance.
//      Uses the OpenAL sample class as a basis
class prWaves
{
public:
    // Method: prWaves
    //      Ctor
    prWaves();

    // Method: ~prWaves
    //      Dtor
    ~prWaves();

    // Method: LoadWaveFile
    //      Loads a wave file and validates the data
    //
    // See Also:
    //      <prWaveResult>
    prWaveResult LoadWaveFile(const char *filename, prWaveID *id);

    // Method: GetWaveData
    //      Gets the wave data
    //
    // See Also:
    //      <prWaveResult>
    prWaveResult GetWaveData(prWaveID id, void **pAudioData) const;

    // Method: GetWaveSize
    //      Gets the wave size
    //
    // See Also:
    //      <prWaveResult>
    prWaveResult GetWaveSize(prWaveID id, Proteus::Core::u32 *size) const;

    // Method: GetWaveFrequency
    //      Gets the frequency
    //
    // See Also:
    //      <prWaveResult>
    prWaveResult GetWaveFrequency(prWaveID id, Proteus::Core::u32 *pulFrequency) const;

    // Method: GetWaveALBufferFormat
    //      Returns the buffer format
    //
    // See Also:
    //      <prWaveResult>
    prWaveResult GetWaveALBufferFormat(prWaveID id, Proteus::Core::u32 *pulFormat) const;

    // Method: DeleteWaveFile
    //      Deletes a loaded wave file
    //
    // See Also:
    //      <prWaveResult>
    prWaveResult DeleteWaveFile(prWaveID id);


private:    
    prWaveResult ParseFile(const char *filename, prWaveFileInfo *pWaveInfo);
    bool         IsWaveID(prWaveID id) const;


private:
    prWaveFileInfo *m_waveID[MAX_NUM_WAVEID];
};
