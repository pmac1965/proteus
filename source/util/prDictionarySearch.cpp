/**
 * prDictionarySearch.cpp
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


#include <stdio.h>
#include <string.h>
#include "prDictionarySearch.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../core/prDefines.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../file/prFile.h"


// Defines
#define WORD_MIN_SIZE           2
#define WORD_MAX_SIZE           32
#define WORD_FILENAME_SIZE      260


// Enums
enum
{
    WSM_NONE    = -1,
    WSM_START_LOAD,
    WSM_UPDATE_LOAD,
    WSM_SEARCH_FILE,
};


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prDictionarySearch::prDictionarySearch(int minLength, int maxLength)
{
    PRASSERT(minLength >= WORD_MIN_SIZE);
    PRASSERT(maxLength >= WORD_MIN_SIZE);
    PRASSERT(maxLength != minLength);
    PRASSERT(minLength <= WORD_MAX_SIZE);
    PRASSERT(maxLength <= WORD_MAX_SIZE);
    PRASSERT(maxLength >  minLength);

    m_minLength = minLength;
    m_maxLength = maxLength;

    m_file       = NULL;
    m_fileBuffer = NULL;
    m_exp0       = false;
    m_exp1       = false;
    m_exp2       = false;
    m_entries    = 0;

    memset(m_word, 0, sizeof(m_word));

    Clear();
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prDictionarySearch::~prDictionarySearch()
{
    Clear();
}


/// ---------------------------------------------------------------------------
// Starts a search
/// ---------------------------------------------------------------------------
void prDictionarySearch::Start(const char *word, prDictionaryCallback callback)
{
    PRASSERT(callback);

    // Are we searching already?
    if (m_searching)
    {
        prTrace(LogError, "prDictionarySearch::Start - Attempted to start a search while a searh is running.\n");
        return;
    }

    // Prep
    if (callback)
    {
        m_callback  = callback;

        if (word && *word)
        {
            // Check word size.
            m_wordSize = prStringLength(word);
            if (m_wordSize < m_minLength || m_wordSize > m_maxLength)
            {
                prTrace(LogError, "prDictionarySearch::Start - Word size is invalid.\n");
                Report(SEARCH_RESULT_ERROR);
                return;
            }

            // Do we have a start letter?
            char c = *word;
            if (!PRBETWEEN(c, 'a', 'z'))
            {
                prTrace(LogError, "prDictionarySearch::Start - Initial character is invalid.\n");
                Report(SEARCH_RESULT_ERROR);
                return;
            }

            // Start search
            m_mode      = WSM_START_LOAD;
            m_searching = true;
            prStringCopySafe(m_word, word, sizeof(m_word));
        }
        else
        {
            prTrace(LogError, "prDictionarySearch::Start - Word pointer was either null or empty.\n");
            Report(SEARCH_RESULT_ERROR);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Updates the dictionary search.
/// ---------------------------------------------------------------------------
void prDictionarySearch::Update()
{
    switch(m_mode)
    {
    case WSM_NONE:
        break;

    case WSM_START_LOAD:
        StartLoad();
        break;

    case WSM_UPDATE_LOAD:
        UpdateLoad();
        break;

    case WSM_SEARCH_FILE:
        SearchFile();
        break;

    default:
        PRPANIC("DictionarySearch::Update - Unknown mode");
        break;
    }
}


/// ---------------------------------------------------------------------------
/// Reset the search.
/// ---------------------------------------------------------------------------
void prDictionarySearch::Clear()
{
    m_callback  = NULL;
    m_searching = false;
    m_mode      = WSM_NONE;
    m_wordSize  = 0;
    m_fileSize  = 0;

    PRSAFE_DELETE_ARRAY(m_fileBuffer);
}


/// ---------------------------------------------------------------------------
/// Report to the user.
/// ---------------------------------------------------------------------------
void prDictionarySearch::Report(prDictionarySearchResult result)
{
    if (m_callback)
    {
        m_callback(result);
    }

    Clear();
}


/// ---------------------------------------------------------------------------
/// Starts loading the dictionary file.
/// ---------------------------------------------------------------------------
void prDictionarySearch::StartLoad()
{
    char filename[WORD_FILENAME_SIZE];

#if defined (PLATFORM_PC)
    sprintf_s(filename, sizeof(filename), "data/dictionary/%c_%i.dic", m_word[0], m_wordSize);

#else
    sprintf(filename, "data/dictionary/%c_%i.dic", m_word[0], m_wordSize);

#endif
    
    m_file = new prFile(filename);

    if (m_file && m_file->Exists())
    {
        if (m_file->Open())
        {
            m_fileSize = m_file->Size();

            if (m_fileSize == 0)
            {
                prTrace(LogError, "prDictionarySearch::StartLoad - File size is zero.\n");
                m_file->Close();
                PRSAFE_DELETE(m_file);
                Report(SEARCH_RESULT_ERROR);
            }
            else
            {
                m_fileBuffer = new char[m_fileSize];
                m_mode       = WSM_UPDATE_LOAD;
            }
        }
        else
        {
            prTrace(LogError, "prDictionarySearch::StartLoad - Failed to open dictionary file.\n");
            PRSAFE_DELETE(m_file);
            Report(SEARCH_RESULT_ERROR);
        }
    }
    else
    {
        prTrace(LogError, "DictionarySearch::StartLoad - File does not exist or failed to create file.\n");
        PRSAFE_DELETE(m_file);
        Report(SEARCH_RESULT_ERROR);
    }
}


/// ---------------------------------------------------------------------------
/// Updates the loading of the dictionary file.
/// ---------------------------------------------------------------------------
void prDictionarySearch::UpdateLoad()
{
    m_file->Read(m_fileBuffer, m_fileSize);
    m_file->Close();
    PRSAFE_DELETE(m_file);

    m_mode = WSM_SEARCH_FILE;

    m_entries = m_fileSize / (m_wordSize + 1);  // + 1 for null
}


/// ---------------------------------------------------------------------------
/// Updates the loading of the dictionary file.
/// ---------------------------------------------------------------------------
void prDictionarySearch::SearchFile()
{
    int  lower = 0;
    int  upper = m_entries;

    while(lower <= upper)
    {
        int mid = (lower + upper) / 2;

        // Get mid entry
        char *pWord = m_fileBuffer;

        pWord += mid * (m_wordSize + 1);

        prStringResult result = prStringCompare(m_word, pWord);
  
        if (result == CMP_GREATERTHAN)
        {
            lower = mid + 1;
        }
        else if (result == CMP_LESSTHAN)
        {
            upper = mid - 1;
        }
        else
        {
            Report(SEARCH_RESULT_FOUND);
            prTrace(LogDebug, "Found %s\n", m_word);
            return;
        }
    }

    prTrace(LogInformation, "Didn't find %s\n", m_word);
    Report(SEARCH_RESULT_NOT_FOUND);
}
