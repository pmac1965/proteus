// File: prStringUtil.h
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


#ifndef __PRSTRINGUTIL_H
#define __PRSTRINGUTIL_H


#include "prTypes.h"
#include "prStringShared.h"


// Function: prStringHash
//      Turns a string into a number.
//
// Parameters:
//      string - The string to convert
//
// Returns:
//      The string as a number
u32 prStringHash(const char* string);

// Function: prStringReplaceChar
//      Changes every occurrence of the search character with the replace character.
//
// Parameters:
//      string  - The string to convert
//      search  - The character to replace
//      replace - The replace character
void prStringReplaceChar(char *string, char search, char replace);

// Function: prStringIsNumber
//      Simple check to test if a string is a number.
//
// Notes:
//      Meant for integers only - examples 23, -10
//
// Parameters:
//      string - The string to test
bool prStringIsNumber(const char *string);

// Function: prStringFindLastIndex
//      Finds the index of the last occurrence of the specified character within the supplied string.
//
// Parameters:
//      string     - The string to search
//      character  - The character to find
//
// Returns:
//      index of character, or -1 on failure
s32 prStringFindLastIndex(const char* string, char character);

// Function: prStringFindFirstIndex
//      Finds the index of the first occurrence of the specified character within the supplied string.
//
// Parameters:
//      string     - The string to search
//      character  - The character to find
//
// Returns:
//      index of character, or -1 on failure
s32 prStringFindFirstIndex(const char* string, char character);

// Function: prStringWriteNumber
//      This function outputs a number to the supplied stream.
//
// Parameters:
//      number   - The number to convert to a string
//      base     - The number base 8 == octal, 10 == decimal, 16 == hexadecimal
//      sign     - indicates if number is negative
//      leading  - The number of leading zeroes in the output stream
//      upper    - Indicates upper case if hexadecimal
//      stream   - The output stream
//
// Returns:
//      The number of characters written to the output stream
s32 prStringWriteNumber(u32 number, u32 base, bool sign, s32 leading, bool upper, char *stream);

// Function: prStringIntToString
//      Converts a number to a text string.
//
// Parameters:
//      num        - The number of convert
//      buffer     - The buffer to write to
//      bufferSize - Size of the write to buffer
void prStringIntToString(s32 num, char *buffer, u32 bufferSize);

// Function: prStringCopySafe
//      Safely copies a string.
//
// Parameters:
//      dst      - The destination buffer
//      src      - The source buffer
//      destSize - The destination buffer size in bytes
//
// Returns:
//      The number of characters copied
u32 prStringCopySafe(char* dst, const char* src, u32 destSize);

// Function: prStringPrintf
//      Creates then returns a formatted string.
//
// Parameters:
//      The format string
//      Optional parameters
//
// Returns:
//      The resultant string
const char *prStringPrintf(const char* fmt, ...);

// Function: prStringSprintf
//      Creates a formatted string.
//
// Parameters:
//      buffer     - The destination buffer
//      bufferSize - The destination buffer size in bytes
//      fmt        - The format string
//      ...        - Optional parameters
void prStringSprintf(char *buffer, s32 bufferSize, const char* fmt, ...);

// Function: prStringToLower
//      Sets a string to lower case.
//
// Parameters:
//      string - The string of convert
void prStringToLower(char* string);

// Function: prStringToUpper
//      Sets a string to upper case.
//
// Parameters:
//      string - The string of convert
void prStringToUpper(char* string);

// Function: prStringLength
//      Returns the length of a passed string.
//
// Parameters:
//      string - The string to count
//
// Returns:
//      The strings length. Zero if null or empty
s32 prStringLength(const char *string);

// Function: prStringLengthW
//      Returns the length of a passed string.
//
// Parameters:
//      string - The string of count
//
// Returns:
//      The strings length. Zero if null or empty
s32 prStringLengthW(const wchar_t *string);

// Function: prStringWordWrap
//      Function places '\n' in a string to indicate where word wrapping should take place.
//
// Parameters:
//      string           - The string of convert
//      max_string_width - The width of the wrapped string
//
// Returns:
//      The number of lines
s32 prStringWordWrap(char *string, u32 max_string_width);

// Function: prStringRemoveNewLines
//      Changes new lines to spaces. (Part of word wrap group)
//
// Parameters:
//      string - The string of convert
void prStringRemoveNewLines(char *string);

// Function: prStringCompare
//      Compares two strings
//
// Parameters:
//      firstString  - String to compare
//      secondString - String to compare
//
// See Also:
//      <prStringResult>
//
// Returns:
//      prStringResult
prStringResult prStringCompare(const char *firstString, const char *secondString);

// Function: prStringCompareW
//      Compares two strings
//
// Parameters:
//      firstString  - String to compare
//      secondString - String to compare
//
// See Also:
//      <prStringResult>
//
// Returns:
//      prStringResult
prStringResult prStringCompareW(const wchar_t *firstString, const wchar_t *secondString);

// Function: prStringCompareNoCase
//      Compares two strings
//
// Parameters:
//      firstString  - String to compare
//      secondString - String to compare
//
// See Also:
//      <prStringResult>
//
// Returns:
//      prStringResult
prStringResult prStringCompareNoCase(const char *firstString, const char *secondString);

// Function: prStringCompareNoCase
//      Compares two strings
//
// Parameters:
//      firstString  - String to compare
//      secondString - String to compare
//      max          - Max characters to compare starting from zero
//
// See Also:
//      <prStringResult>
//
// Returns:
//      prStringResult
prStringResult prStringCompareNoCase(const char *firstString, const char *secondString, int max);

// Function: prStringCopyUntil
//      Copies from the source string to the destination string all characters upto, but not including the until character.
//
// Parameters:
//      dst      - The destination buffer
//      src      - The source buffer
//      until    - The end character
//
// Returns:
//      The number of characters copied
s32 prStringCopyUntil(char *src, char *dst, char until);

// Function: prStringFindSubString
//      Finds a sub string within a string.
//
// Parameters:
//      string - The string to search
//      search - The search string
//
// Returns:
//      The sub string, or null on failure
char *prStringFindSubString(char *string, char *search);

// Function: prStringCopy
//      Copies one string to another.
//
// Parameters:
//      dst      - The destination buffer
//      src      - The source buffer
//
// Returns:
//      The number of characters copied
s32 prStringCopy(const char *src, char *dst);

// Function: prStringAddString
//      Adds one string to another.
//
// Parameters:
//      dst      - The destination buffer
//      src      - The source buffer
//
// Returns:
//      The number of characters added
s32 prStringAddString(const char *src, char *dst);

// Function: prStringAddChar
//      Adds a single character to the supplied string.
//
// Parameters:
//      string    - The string to add to
//      character - The character to add
void prStringAddChar(char *string, char character);

// Function: prStringToHex
//      Converts a hex string like 'FAC5012E' to its numerical equivalent.
//
// Parameters:
//      string - The string to convert
//
// Returns:
//      The hex number
u32 prStringToHex(const char* string);

// Function: prStringContainsCharacters
//      Returns true if a string contains any NON white space characters, else returns false.
//
// Parameters:
//      text - The string to test
//
// Returns:
//      true or false
bool prStringContainsCharacters(const char* text);

// Function: prStringParseInts
//      Parses a string in the format of "0,0" into integer values
//
// Parameters:
//      text - The string to parse
//      a    - Variable to receive parsed value
//      b    - Variable to receive parsed value
void prStringParseInts(const char* text, s32 &a, s32 &b);

// Function: prStringParseInts
//      Parses a string in the format of "0,0,0" into integer values
//
// Parameters:
//      text - The string to parse
//      a    - Variable to receive parsed value
//      b    - Variable to receive parsed value
//      c    - Variable to receive parsed value
void prStringParseInts(const char* text, s32 &a, s32 &b, s32 &c);

// Function: prStringParseFloats
//      Parses a string in the format of "0,0" into float values
//
// Parameters:
//      text - The string to parse
//      a    - Variable to receive parsed value
//      b    - Variable to receive parsed value
void prStringParseFloats(const char* text, f32 &a, f32 &b);

// Function: prStringParseFloats
//      Parses a string in the format of "0,0,0" into float values
//
// Parameters:
//      text - The string to parse
//      a    - Variable to receive parsed value
//      b    - Variable to receive parsed value
//      c    - Variable to receive parsed value
void prStringParseFloats(const char* text, f32 &a, f32 &b, f32 &c);

// Function: prStringFind
//      Finds if the specified character exists within the supplied string.
//
// Parameters:
//      string    - The string to search
//      character - The character to find
//
// Returns:
//      true or false
bool prStringFind(const char* string, char character);

// Function: prStringExtractUtf8
//      Decodes max four bytes only as this allows for characters from the Greek,
//      Cyrillic, Coptic, Armenian, Hebrew, Arabic, Syriac and Tana alphabets.
//      And almost everything else.
//
// Parameters:
//      string    - The string to search
//      extracted - Variable to receive extracted value
//
// Returns:
//      String adjust value. Size is the number of bytes used to encode the character
s32 prStringExtractUtf8(const char *string, s32 &extracted);

// Function: prStringParseControls
//      Parses controls in text docs into the equivalent embedded control codes.
//
// Notes:
//      Currently does;
//      - \n
//      - \t
//
// Parameters:
//      text    - The string to convert
//      buffer  - Buffer to receive converted string
void prStringParseControls(const char *text, char *buffer);

// Function: prSprintf
//      Creates a formatted string.
//
// Parameters:
//      buffer     - The destination buffer
//      fmt        - The format string
//      ...        - Optional parameters
void prSprintf(char *buffer, const char* fmt, ...);


// Function: prStringDup
//      Duplicates a string
//
// Parameters:
//      str - The string to copy
// 
// Notes:
//      The user is responsible for deleting the returned string.
//
// Notes:
//      Can return NULL if the passed string is NULL or empty
//
char *prStringDup(const char* str);


#endif//__PRSTRINGUTIL_H
