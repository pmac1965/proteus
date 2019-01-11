// File: prDictionarySearch.h   
//      Class to search dictionary files
//
// Notes:
//      The dictionary must be split using the engine dictionary splitter tool,
//      which creates the required .dic files.
//
// Notes:
//      The supplied dictionary must be alphabetically sorted
//
// Notes:
//      The update method of this class must be called at least once per frame,
//      else the search won't happen and you will never receive any callbacks.
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


#pragma once


#include "../core/prTypes.h"


// Enum: prDictionarySearchResult
//      Search results.
//
//    SEARCH_RESULT_ERROR       - A search has not been started, or search failed.
//    SEARCH_RESULT_FOUND       - The word was found.
//    SEARCH_RESULT_NOT_FOUND   - The word was not found.
//    SEARCH_RESULT_PENDING     - Search still running
//
typedef enum
{
    SEARCH_RESULT_ERROR,
    SEARCH_RESULT_FOUND,
    SEARCH_RESULT_NOT_FOUND,
    SEARCH_RESULT_PENDING

} prDictionarySearchResult;


// Class: prDictionaryCallbacks
//      A mix-in class to receive search notifications
//
// Notes:
//      This class is *NOT* optional
class prDictionaryCallbacks
{
public:
    virtual void prDictionaryCallback(prDictionarySearchResult result) = 0;
};


// Defines
#define DICT_WORD_BUFFER_SIZE       256


// Forward declarations
class prFile;


// Class: prDictionarySearch
//      Class used to search a text dictionary installed on local storage.
//
// Notes:
//      The update method of this class must be called at least once per frame,
//      else the search won't happen and you will never receive any callbacks.
class prDictionarySearch
{
public:
    // Method: prDictionarySearch
    //      Constructor
    //
    // Parameters:
    //      minLength   - The minimum length of words which can be searched for.
    //      maxLength   - The maximum length of words which can be searched for.
    //      pcb         - A callback class. *Must not be NULL.*
    prDictionarySearch(s32 minLength, s32 maxLength, prDictionaryCallbacks *pcb);

    // Method: ~prDictionarySearch
    //      Destructor
    ~prDictionarySearch();

    // Method: Start
    //      Starts a dictionary search. 
    //
    // Parameters:
    //      word        - The to search for.
    //
    // Notes:
    //      Do not start a search while one is in progress, as the secondary search @n
    //      will be ignored.
    void Start(const char *word);

    // Method: Update
    //      Updates the dictionary search.
    //
    // Notes:
    //      Must be called once per frame.
    void Update();

    // Method: IsSearching
    //      Determines if a search is running.
    PRBOOL IsSearching() const { return m_searching; }


private:
    prDictionaryCallbacks  *m_callback;
    s32                     m_minLength;
    s32                     m_maxLength;
    s32                     m_mode;
    s32                     m_wordSize;
    s32                     m_fileSize;
    s32                     m_entries;
    prFile                 *m_file;
    char                   *m_fileBuffer;
    PRBOOL                  m_searching;
    char                    m_word[DICT_WORD_BUFFER_SIZE];


private:
    void Clear();
    void Report(prDictionarySearchResult result, const char *message = nullptr);
    void StartLoad();
    void UpdateLoad();
    void SearchFile();
};
