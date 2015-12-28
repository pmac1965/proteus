// File: prDictionarySearch.h
//
// Notes:
//      Provides a way to quickly search a language dictionary using the engine
//      dictionary splitter tool. Dictionary must be alphabetically sorted
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
//
typedef enum //prDictionarySearchResult
{
    SEARCH_RESULT_ERROR,
    SEARCH_RESULT_FOUND,
    SEARCH_RESULT_NOT_FOUND

} prDictionarySearchResult;


// Typedef: prDictionaryCallback
//      The dictionary callback type.
typedef void (*prDictionaryCallback)(prDictionarySearchResult result);


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
    prDictionarySearch(int minLength, int maxLength);

    // Method: ~prDictionarySearch
    //      Destructor
    ~prDictionarySearch();

    // Method: Start
    //      Starts a dictionary search. 
    //
    // Parameters:
    //      word        - The to search for.
    //      callback    - A callback pointer. *Must not be NULL.*
    //
    // Notes:
    //      Do not start a search while one is in progress, as the secondary search @n
    //      will be ignored.
    void Start(const char *word, prDictionaryCallback callback);

    // Method: Update
    //      Updates the dictionary search.
    //
    // Notes:
    //      Must be called once per frame.
    void Update();

    // Method: IsSearching
    //      Determines if a search is running.
    bool IsSearching() const { return m_searching; }


private:
    prDictionaryCallback    m_callback;
    Proteus::Core::s32      m_minLength;
    Proteus::Core::s32      m_maxLength;
    Proteus::Core::s32      m_mode;
    Proteus::Core::s32      m_wordSize;
    Proteus::Core::s32      m_fileSize;
    Proteus::Core::s32      m_entries;
    prFile                 *m_file;
    char                   *m_fileBuffer;
    bool                    m_searching;
    bool                    m_exp2;
    bool                    m_exp1;
    bool                    m_exp0;
    char                    m_word[DICT_WORD_BUFFER_SIZE];


private:
    void Clear();
    void Report(prDictionarySearchResult result);
    void StartLoad();
    void UpdateLoad();
    void SearchFile();
};
