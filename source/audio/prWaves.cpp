/**
 * prWaves.cpp
 */
// http://www.gamedev.net/reference/articles/article709.asp


#include "../prConfig.h"


#if defined(PLATFORM_PC) || defined(PLATFORM_ANDROID)

#if defined(PLATFORM_PC)
  #include <windows.h>
  #include <al.h>
  #include <alc.h>
  #include <ks.h>
  #include <ksmedia.h>

#elif defined(PLATFORM_ANDROID)
  #include "../android/AL/al.h"
  #include "../android/AL/alc.h"
  #include "prOpenALErrors.h"

#else
    #error Unsupported platform

#endif


#include <cstring>
#include "prWaves.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../file/prFile.h"
#include "../file/prFileShared.h"


// Defines
#define WAVE_FORMAT_EXTENSIBLE		0xFFFE
#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM             0x0001
#endif


// Enums
enum prWaveFileType
{
    WF_EX  = 1,
    WF_EXT = 2,
};


// File header.
typedef struct prWaveFileHeader
{
    char    riff[4];        // Identifier string = "RIFF"
    u32	    size;           // Remaining length after this header.
    char	wave[4];        // 4-byte data type identifier.  

} prWaveFileHeader;


// Header for a chunk in the wave file.
typedef struct prRiffChunk
{
    char			name[4];
    unsigned long	size;

} prRiffChunk;


// Wave format
typedef struct prWaveFormat
{
    prWaveFormat()
    {
        usFormatTag         = 0;
        usChannels          = 0;
        ulSamplesPerSec     = 0;
        ulAvgBytesPerSec    = 0;
        usBlockAlign        = 0;
        usBitsPerSample     = 0;
        usSize              = 0;
        usReserved          = 0;
        ulChannelMask       = 0;
    }

    u16	    usFormatTag;
    u16	    usChannels;
    u32	    ulSamplesPerSec;
    u32	    ulAvgBytesPerSec;
    u16	    usBlockAlign;
    u16	    usBitsPerSample;
    u16	    usSize;
    u16     usReserved;
    u32	    ulChannelMask;

} prWaveFormat;


// Details of a wave file.
typedef struct prWaveFileInfo
{
    prWaveFileType	        wfType;
    unsigned short          format;
    unsigned short          channels;
    unsigned int            samplesPerSec;
    unsigned int            bytesPerSec;
    unsigned short          blockAlign;
    unsigned short          bitsPerSample;
    char			       *pData;
    unsigned long	        ulDataSize;
    prFile			       *pFile;
    unsigned long	        ulDataOffset;

} prWaveFileInfo;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWaves::prWaves()
{
    memset(m_waveID, 0, sizeof(*m_waveID) * MAX_NUM_WAVEID);
    //prTrace("sizeof(*m_waveID) * MAX_NUM_WAVEID == %i\n", sizeof(*m_waveID) * MAX_NUM_WAVEID);
    TODO("Remove the wave ID list as its not required")
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prWaves::~prWaves()
{
    for (int i=0; i<MAX_NUM_WAVEID; i++)
    {
        if (m_waveID[i])
        {
            PRSAFE_DELETE(m_waveID[i]->pData);
        }

        PRSAFE_DELETE(m_waveID[i]);
    }
}


/// ---------------------------------------------------------------------------
/// Loads a wave file and validates the data
/// ---------------------------------------------------------------------------
prWaveResult prWaves::LoadWaveFile(const char *filename, prWaveID *pWaveID)
{
    PRUNUSED(pWaveID);
    PRASSERT(filename && *filename);

    prWaveResult    wr        = WR_OUTOFMEMORY;
    prWaveFileInfo *pWaveInfo = new prWaveFileInfo;
    if (pWaveInfo)
    {
        memset(pWaveInfo, 0, sizeof(prWaveFileInfo));

        wr = ParseFile(filename, pWaveInfo);

        if (wr == WR_OK)
        {
            PRASSERT(pWaveInfo->pFile);

            pWaveInfo->pData = new char[pWaveInfo->ulDataSize];
            if (pWaveInfo->pData)
            {
                // Seek to start of audio data
                pWaveInfo->pFile->Seek(pWaveInfo->ulDataOffset, PRFILE_SEEK_SET);

                // Read Sample Data
                if (pWaveInfo->pFile->Read(pWaveInfo->pData, pWaveInfo->ulDataSize) == pWaveInfo->ulDataSize)
                {
                    long lLoop = 0;
                    for (lLoop = 0; lLoop < MAX_NUM_WAVEID; lLoop++)
                    {
                        if (!m_waveID[lLoop])
                        {
                            m_waveID[lLoop] = pWaveInfo;
                            *pWaveID = lLoop;
                            wr = WR_OK;
                            break;
                        }
                    }

                    if (lLoop == MAX_NUM_WAVEID)
                    {
                        delete pWaveInfo->pData;
                        wr = WR_OUTOFMEMORY;
                    }
                }
                else
                {
                    delete pWaveInfo->pData;
                    wr = WR_BADWAVEFILE;
                }
            }
            else
            {
                wr = WR_OUTOFMEMORY;
            }


            pWaveInfo->pFile->Close();
            PRSAFE_DELETE(pWaveInfo->pFile);
        }
        else
        {
            PRSAFE_DELETE(pWaveInfo);
        }
    }

    return wr;
}


/// ---------------------------------------------------------------------------
/// Loads a wave file and validates the data
/// ---------------------------------------------------------------------------
prWaveResult prWaves::ParseFile(const char *filename, prWaveFileInfo* pWaveInfo)
{
    PRASSERT(filename && *filename);
    PRASSERT(pWaveInfo);

    prWaveResult      wr = WR_BADWAVEFILE;
    prWaveFileHeader  header;
    prRiffChunk		  riffChunk;
    prWaveFormat      waveFmt;

    // Sanity checks
    if (!filename || !pWaveInfo)
        return WR_INVALIDPARAM;


    // Open the file?
    prFile *file = new prFile(filename);
    if (file && file->Open())
    {
        file->Read(&header, sizeof(prWaveFileHeader));

        if (prStringCompareNoCase(header.riff, "RIFF", 4) == CMP_EQUALTO &&
            prStringCompareNoCase(header.wave, "WAVE", 4) == CMP_EQUALTO
           )
        {
            while (file->Read(&riffChunk, sizeof(prRiffChunk)) == sizeof(prRiffChunk))
            {
                if (prStringCompareNoCase(riffChunk.name, "fmt ", 4) == CMP_EQUALTO)
                {
                    if (riffChunk.size <= sizeof(prWaveFormat))
                    {
                        file->Read(&waveFmt, riffChunk.size);
                    
                        // Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
                        if (waveFmt.usFormatTag == WAVE_FORMAT_PCM || waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
                        {
                            pWaveInfo->wfType           = (waveFmt.usFormatTag == WAVE_FORMAT_PCM) ? WF_EX : WF_EXT; 
                            pWaveInfo->format           = waveFmt.usFormatTag;
                            pWaveInfo->channels         = waveFmt.usChannels;
                            pWaveInfo->bitsPerSample    = waveFmt.usBitsPerSample;
                            pWaveInfo->bytesPerSec      = waveFmt.ulAvgBytesPerSec;
                            pWaveInfo->samplesPerSec    = waveFmt.ulSamplesPerSec;
                        }
                        else
                        {
                            TODO("Unsupported format")
                            prTrace("Unsupported format");
                        }
                    }
                    else
                    {
                        file->Seek(riffChunk.size, PRFILE_SEEK_CUR);
                    }
                }
                else if (prStringCompareNoCase(riffChunk.name, "data", 4) == CMP_EQUALTO)
                {
                    pWaveInfo->ulDataSize   = riffChunk.size;
                    pWaveInfo->ulDataOffset = file->Tell();
                    file->Seek(riffChunk.size, PRFILE_SEEK_CUR);
                }
                else
                {
                    file->Seek(riffChunk.size, PRFILE_SEEK_CUR);
                }

                // Ensure that we are correctly aligned for next chunk
                if (riffChunk.size & 1)
                {
                    file->Seek(1, PRFILE_SEEK_CUR);
            }
            }

            if (pWaveInfo->ulDataSize   > 0 &&
                pWaveInfo->ulDataOffset > 0 && ((pWaveInfo->wfType == WF_EX) || (pWaveInfo->wfType == WF_EXT)))
            {
                pWaveInfo->pFile = file;
                return WR_OK;
            }
            else
            {
                TODO("?")
            }
        }
    }
    else
    {
        wr = WR_INVALIDFILENAME;
    }


    if (file)
    {
        file->Close();
        PRSAFE_DELETE(file);
    }

    return wr;
}


/// ---------------------------------------------------------------------------
/// Validates the wave ID.
/// ---------------------------------------------------------------------------
bool prWaves::IsWaveID(prWaveID id) const
{
    bool bReturn = false;

    if ((id >= 0) && (id < MAX_NUM_WAVEID))
    {
        if (m_waveID[id])
            bReturn = true;
    }

    return bReturn;
}


/// ---------------------------------------------------------------------------
/// Gets the wave data.
/// ---------------------------------------------------------------------------
prWaveResult prWaves::GetWaveData(prWaveID id, void **pAudioData)
{
    if (!IsWaveID(id))
        return WR_INVALIDWAVEID;

    if (!pAudioData)
        return WR_INVALIDPARAM;

    *pAudioData = m_waveID[id]->pData;

    return WR_OK;
}


/// ---------------------------------------------------------------------------
/// Gets the wave size
/// ---------------------------------------------------------------------------
prWaveResult prWaves::GetWaveSize(prWaveID id, u32 *size)
{
    if (!IsWaveID(id))
        return WR_INVALIDWAVEID;

    if (!size)
        return WR_INVALIDPARAM;

    *size = m_waveID[id]->ulDataSize;

    return WR_OK;
}


/// ---------------------------------------------------------------------------
/// Gets the frequency.
/// ---------------------------------------------------------------------------
prWaveResult prWaves::GetWaveFrequency(prWaveID id, u32 *pulFrequency)
{
    prWaveResult wr = WR_OK;
    
    if (IsWaveID(id))
    {
        if (pulFrequency)
        {
            *pulFrequency = m_waveID[id]->samplesPerSec;
        }
        else
        {
            wr = WR_INVALIDPARAM;
        }
    }
    else
    {
        wr = WR_INVALIDWAVEID;
    }

    return wr;
}


/// ---------------------------------------------------------------------------
/// Returns the buffer format
/// ---------------------------------------------------------------------------
prWaveResult prWaves::GetWaveALBufferFormat(prWaveID id, u32 *pulFormat)
{
    prWaveResult wr = WR_OK;

    if (IsWaveID(id))
    {
        if (pulFormat)
        {
            *pulFormat = 0;

            if (m_waveID[id]->wfType == WF_EX)
            {
                if (m_waveID[id]->channels == 1)
                {
                    switch (m_waveID[id]->bitsPerSample)
                    {
                    case 4:
                        *pulFormat = alGetEnumValue("AL_FORMAT_MONO_IMA4");
                        break;
                    case 8:
                        *pulFormat = alGetEnumValue("AL_FORMAT_MONO8");
                        break;
                    case 16:
                        *pulFormat = alGetEnumValue("AL_FORMAT_MONO16");
                        break;
                    }
                }
                else if (m_waveID[id]->channels == 2)
                {
                    switch (m_waveID[id]->bitsPerSample)
                    {
                    case 4:
                        *pulFormat = alGetEnumValue("AL_FORMAT_STEREO_IMA4");
                        break;
                    case 8:
                        *pulFormat = alGetEnumValue("AL_FORMAT_STEREO8");
                        break;
                    case 16:
                        *pulFormat = alGetEnumValue("AL_FORMAT_STEREO16");
                        break;
                    }
                }
                else if ((m_waveID[id]->channels == 4) && (m_waveID[id]->bitsPerSample == 16))
                {
                    *pulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
                }
            }
            //else if (m_waveID[id]->wfType == WF_EXT)
            //{
            //    TODO("Fix and extend")
            //}

            if (*pulFormat == 0)
                wr = WR_INVALIDWAVEFILETYPE;
        }
        else
        {
            wr = WR_INVALIDPARAM;
        }
    }
    else
    {
        wr = WR_INVALIDWAVEID;
    }

    return wr;
}


/// ---------------------------------------------------------------------------
/// Deletes a loaded wave file
/// ---------------------------------------------------------------------------
prWaveResult prWaves::DeleteWaveFile(prWaveID id)
{
    prWaveResult wr = WR_OK;

    if (IsWaveID(id))
    {
        PRSAFE_DELETE(m_waveID[id]->pData);
        PRSAFE_DELETE(m_waveID[id]);
    }
    else
    {
        wr = WR_INVALIDWAVEID;
    }

    return wr;
}


#endif
