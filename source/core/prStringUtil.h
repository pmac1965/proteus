/**
 * @file       prStringUtil.h
 * @brief      Contains string functions.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRSTRINGUTIL_H
#define __PRSTRINGUTIL_H


#include "prTypes.h"
#include "prStringShared.h"


/// @brief      Turns a string into a number.
/// @param      string - The string to convert
/// @return     The string as a number
u32 prStringHash(const char* string);

/// @brief      Changes every occurrence of the search character with the replace character.
/// @param      string  - The string to convert
/// @param      search  - The character to replace
/// @param      replace - The replace character
void prStringReplaceChar(char *string, char search, char replace);

/// @brief      Simple check to test if a string is a number.
/// @note       Meant for integers only
/// @param      string - The string to test
/// @return     true if number, false otherwise.
bool prStringIsNumber(const char *string);

/// @brief      Finds the index of the last occurrence of the specified character within the supplied string.
/// @param      string     - The string to search
/// @param      character  - The character to find
/// @return     index of character, or -1 on failure
s32 prStringFindLastIndex(const char* string, char character);

/// @brief      Finds the index of the first occurrence of the specified character within the supplied string.
/// @param      string     - The string to search
/// @param      character  - The character to find
/// @return     index of character, or -1 on failure
s32 prStringFindFirstIndex(const char* string, char character);

/// @brief      This function outputs a number to the supplied stream.
/// @param      number   - The number to convert to a string
/// @param      base     - The number base 8 == octal, 10 == decimal, 16 == hexadecimal
/// @param      sign     - indicate if number is negative
/// @param      leading  - The number of leading zeroes in the output stream
/// @param      upper    - Indicates upper case if hexadecimal
/// @param      stream   - The output stream
/// @return     The number of characters written to the output stream
s32 prStringWriteNumber(u32 number, u32 base, bool sign, s32 leading, bool upper, char *stream);

/// @brief      Converts a number to a text string.
/// @param      num        - The number of convert
/// @param      buffer     - The buffer to write to
/// @param      bufferSize - Size of the write to buffer
void prStringIntToString(s32 num, char *buffer, u32 bufferSize);

/// @brief      Safely copies a string.
/// @param      dst      - The destination buffer
/// @param      src      - The source buffer
/// @param      destSize - The destination buffer size in bytes
/// @return     The number of characters copied
u32 prStringCopySafe(char* dst, const char* src, u32 destSize);

/// @brief      Creates then returns a formatted string.
/// @param      fmt  - The format string
/// @param      ...  - Optional parameters
/// @return     The resultant string
const char *prStringPrintf(const char* fmt, ...);

/// @brief      Creates a formatted string.
/// @param      buffer     - The destination buffer
/// @param      bufferSize - The destination buffer size in bytes
/// @param      fmt        - The format string
/// @param      ...        - Optional parameters
void prStringSprintf(char *buffer, s32 bufferSize, const char* fmt, ...);

/// @brief      Sets a string to lower case.
/// @param      string - The string of convert
void prStringToLower(char* string);

/// @brief      Sets a string to upper case.
/// @param      string - The string of convert
void prStringToUpper(char* string);

/// @brief      Returns the length of a passed string.
/// @param      string - The string of count
/// @return     The strings length. Zero if null or empty
s32 prStringLength(const char *string);
s32 prStringLengthW(const wchar_t *string);

/// @brief      Function places '\\n' in a string to indicate where word wrapping should take place.
/// @param      string           - The string of convert
/// @param      max_string_width - The width of the wrapped string
/// @return     The number of lines
s32 prStringWordWrap(char *string, u32 max_string_width);

/// @brief      Changes new lines to spaces. (Part of word wrap group)
/// @param      string - The string of convert
void prStringRemoveNewLines(char *string);

/// @brief      Compares two strings
/// @param      firstString  - String to compare
/// @param      secondString - String to compare
/// @see        prStringResult
/// @return     Comparison result
prStringResult prStringCompare(const char *firstString, const char *secondString);
prStringResult prStringCompareW(const wchar_t *firstString, const wchar_t *secondString);

/// @brief      Compares two strings
/// @param      firstString  - String to compare
/// @param      secondString - String to compare
/// @see        prStringResult
/// @return     Comparison result
prStringResult prStringCompareNoCase(const char *firstString, const char *secondString);

/// @brief      Compares two strings
/// @param      firstString  - String to compare
/// @param      secondString - String to compare
/// @param      max          - Max characters to compare starting from zero
/// @see        prStringResult
/// @return     Comparison result
prStringResult prStringCompareNoCase(const char *firstString, const char *secondString, int max);

/// @brief      Copies from the source string to the destination string all characters upto, but not including the until character.
/// @param      dst      - The destination buffer
/// @param      src      - The source buffer
/// @param      until    - The end character
/// @return     The number of characters copied
s32 prStringCopyUntil(char *src, char *dst, char until);

/// @brief      Finds a sub string within a string.
/// @param      string - The string to search
/// @param      search - The search string
/// @return     The sub string, or null on failure
char *prStringFindSubString(char *string, char *search);

/// @brief      Copies one string to another.
/// @param      dst      - The destination buffer
/// @param      src      - The source buffer
/// @return     The number of characters copied
s32 prStringCopy(char *src, char *dst);

/// @brief      Adds one string to another.
/// @param      dst      - The destination buffer
/// @param      src      - The source buffer
/// @return     The number of characters added
s32 prStringAddString(const char *src, char *dst);

/// @brief      Adds a single character to the supplied string.
/// @param      string    - The string to add to
/// @param      character - The character to add
void prStringAddChar(char *string, char character);

/// @brief      Converts a hex string like 'FAC5012E' to its numerical equivalent.
/// @param      string - The string to convert
/// @return     The hex number
u32 prStringToHex(const char* string);

/// @brief      Returns true if a string contains any NON white space characters, else returns false.
/// @param      text - The string to test
/// @return     true or false
bool prStringContainsCharacters(const char* text);

/// @brief      Parses a string in the format of "0,0" into integer values
/// @param      text - The string to parse
/// @param      a    - Variable to receive parsed value
/// @param      b    - Variable to receive parsed value
void prStringParseInts(const char* text, s32 &a, s32 &b);

/// @brief      Parses a string in the format of "0,0,0" into integer values
/// @param      text - The string to parse
/// @param      a    - Variable to receive parsed value
/// @param      b    - Variable to receive parsed value
/// @param      c    - Variable to receive parsed value
void prStringParseInts(const char* text, s32 &a, s32 &b, s32 &c);

/// @brief      Parses a string in the format of "0,0" into float values
/// @param      text - The string to parse
/// @param      a    - Variable to receive parsed value
/// @param      b    - Variable to receive parsed value
void prStringParseFloats(const char* text, f32 &a, f32 &b);

/// @brief      Parses a string in the format of "0,0,0" into float values
/// @param      text - The string to parse
/// @param      a    - Variable to receive parsed value
/// @param      b    - Variable to receive parsed value
/// @param      c    - Variable to receive parsed value
void prStringParseFloats(const char* text, f32 &a, f32 &b, f32 &c);

/// @brief      Finds if the specified character exists within the supplied string.
/// @param      string    - The string to search
/// @param      character - The character to find
/// @return     true or false
bool prStringFind(const char* string, char character);

/// @brief      Decodes max four bytes only as this allows for characters from the Greek,
/// @n          Cyrillic, Coptic, Armenian, Hebrew, Arabic, Syriac and Tana alphabets.
/// @n          And almost everything else.
/// @param      string    - The string to search
/// @param      extracted - Variable to receive extracted value
/// @return     String adjust value. Size is the number of bytes used to encode the character
s32 prStringExtractUtf8(const char *string, s32 &extracted);

/// @brief      Parses controls in text docs into the equivalent embedded control codes.
/// @n          Currently does;
/// @n          - \\n
/// @n          - \\t
/// @param      text    - The string to convert
/// @param      buffer  - Buffer to receive converted string
void prStringParseControls(const char *text, char *buffer);

/// @brief      Creates a formatted string.
/// @param      buffer     - The destination buffer
/// @param      fmt        - The format string
/// @param      ...        - Optional parameters
void prSprintf(char *buffer, const char* fmt, ...); // TO BE MOVED!


#endif//__PRSTRINGUTIL_H
