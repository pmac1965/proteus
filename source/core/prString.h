// File: prString.h
//      Fixed size string class. This class is designed not to
//      allocate memory during usage. Use where you know string size
//      will be limited.
//
// Changes:
//      July 2019 - STRING_BUFFER_SIZE define to constant.
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


#include "prTypes.h"
#include "prStringShared.h"


// Constants
const s32 STRING_BUFFER_SIZE = 256;


// Class: prString
//      Fixed size string class.
class prString
{
public:
    // Method: prString
    //      Default constructor
    prString();

    // Method: prString
    //      Constructor
    //
    // Parameters:
    //      text - The string to set
    explicit prString(const char *text);

    // Method: prString
    //      Copy constructor
    //
    // Parameters:
    //      str - String to copy
    prString(const prString &str);

    // Method: Clear
    //      Sets the string as empty, but not NULL
    void Clear();

    // Method: Set
    //      Sets the strings text
    //
    // Parameters:
    //      text - The string to set
    //
    // Notes:
    //      The passed text can be NULL or empty ""
    void Set(const char *text);

    // Method: Append
    //      Adds to the string.
    //
    // Parameters:
    //      str - String to add
    void Append(const prString &str);

    // Method: Append
    //      Adds to the string.
    //
    // Parameters:
    //      text - Text to add
    //
    // Notes:
    //      The passed text can be NULL or empty ""
    void Append(const char *text);

    // Method: TrimFront
    //      Removes any leading spaces.
    void TrimFront();

    // Method: TrimBack
    //      Removes any trailing spaces.
    void TrimBack();

    // Method: Trim
    //      Removes any leading/trailing spaces.
    void Trim();

    // Method: Compare
    //  Compares string for equality.
    //
    // Parameters
    //      text - Text to compare
    //
    // Returns:
    //      prStringResult
    //
    // See Also:
    //      <prStringResult>
    prStringResult Compare(const char *text);

    // Method: Compare
    //  Compares string for equality.
    //
    // Parameters
    //      str - String to compare
    //
    // Returns:
    //      prStringResult
    //
    // See Also:
    //      <prStringResult>
    prStringResult Compare(const prString &str);

    // Method: Replace
    //      Replaces all occurences of 'findChar' with 'replaceChar'
    void Replace(char findChar, char replaceChar);

    // Method: ToUpper
    //      Makes the string uppercase
    void ToUpper();

    // Method: ToLower
    //      Makes the string lowercase
    void ToLower();

    // Method: Sprintf
    //      Creates a string using a standard C format string
    //
    // Parameters:
    //      fmt - The format string
    //      ... - Optional parameters
    void Sprintf(const char *fmt, ...);

    // Method: Length
    //      Gets the length of the string
    s32 Length() const { return m_length; }

    // Method: Text
    //      Gets the strings text
    const char *Text() const { return m_text; }

    //bool Contains(const char *text);
    //bool Contains(const prString &str);
    //bool IsEmpty();
    //void Remove(char c);?
    // Tests the strings, sets length, use if [] sets null
    //void Rescan();


    // -- Operators --

    // Operator +
    prString operator + (const prString& rhs) const;
    prString operator + (const char rhs) const;
    prString operator + (const s8 rhs) const;
    prString operator + (const s16 rhs) const;
    prString operator + (const s32 rhs) const;
    prString operator + (const f32 rhs) const;
    prString operator + (const s64 rhs) const;
    prString operator + (const f64 rhs) const;

    // Operator ==, !=
    inline bool operator == (const prString &a) { return Compare(a) == 0; }
    inline bool operator != (const prString &a) { return Compare(a) != 0; }

    // Array access.
    char& operator [] (unsigned int index);

private:
    char   *m_text;
    char    m_buffer[STRING_BUFFER_SIZE];
    s32     m_length;
    u32     m_hash;
};
