/**
 * @file       prStringUtil.h
 * @brief      Contains string functions.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 */


#include "../prConfig.h"


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "prStringUtil.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"


#define         MASK2BYTES              0xC0
#define         MASK3BYTES              0xE0
#define         MASK4BYTES              0xF0
#define         MASKBITS                0x3F


/// ---------------------------------------------------------------------------
/// Turns a string into a number.
/// ---------------------------------------------------------------------------
u32 prStringHash(const char* string)
{
    PRASSERT( string);
    PRASSERT(*string);

    u32 hash = 0;

    while(*string)
    {
        hash += *string;
        hash *= *string++;
    }

    return hash;
}


/// ---------------------------------------------------------------------------
/// Changes every occurrence of the search character with the replace character.
/// ---------------------------------------------------------------------------
void prStringReplaceChar(char *string, char search, char replace)
{
    PRASSERT( string);
    PRASSERT(*string);

    if (string && *string)
    {
        while(*string)
        {
            if (*string == search)
                *string = replace;

            string++;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Simple check to test if a string is a number.
/// ---------------------------------------------------------------------------
bool prStringIsNumber(const char *string)
{
    if (string && *string)
    {
        // Bypass minus
        if (*string == '-')
        {
            string++;
        }

        // Check for non numerals
        while(*string)
        {
            char c = *string++;
            
            if (c < '0' || c > '9')
            {
                return false;
            }
        }

        return true;
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Finds the index of the last occurrence of the specified character within the
/// supplied string.
/// ---------------------------------------------------------------------------
s32 prStringFindLastIndex(const char* string, char character)
{
    s32 lastIndex = -1;

    if (string)
    {
        char c;
        s32 index = 0;

        do
        {
            c = string[index];
            
            if (c == character)
            {
                lastIndex = index;
            }

            index++;
        }
        while(c);
    }

    return lastIndex;
}


/// ---------------------------------------------------------------------------
/// Finds the index of the first occurrence of the specified character within
/// the supplied string.
/// ---------------------------------------------------------------------------
s32 prStringFindFirstIndex(const char* string, char character)
{
    if (string)
    {
        char c;
        s32 index = 0;

        do
        {
            c = string[index];
            
            if (c == character)
            {
                return index;
            }

            index++;
        }
        while(c);
    }

    return -1;
}


/// ---------------------------------------------------------------------------
/// This function outputs a number to the supplied stream.
/// ---------------------------------------------------------------------------
s32 prStringWriteNumber(u32 number, u32 base, bool sign, s32 leading, bool upper, char *stream)
{
    char buffer[32];
    s32  index = 0;
    s32  count = 0;


    // Is the number signed?
    if (sign)
    {
        s32 num = (s32)number;

        // Is negative?
        if (num < 0)
        {
            number = (u32)-num;

            *stream++ = '-';

            count++;
        }
    }


    // Convert the number.
    do
    {                   
        char c = (char)('0' + number % base);

        // Hex?
        if (c > '9')
        {
            c += (upper) ? 7 : 39;
        }

        buffer[index++] = c;
        
        number /= base;

        count++;
    }
    while(number > 0);


    // Any leading zeroes?
    if (leading - count > 0)
    {
        leading -= count;

        while(--leading >= 0)
        {
            *stream++ = '0';
            count++;
        }
    }

    
    // Output to stream.
    while(--index >= 0)
    {
        *stream++ = buffer[index];
    }


    return count;
}


/// ---------------------------------------------------------------------------
/// Converts a number to a text string.
/// ---------------------------------------------------------------------------
void prStringIntToString(s32 num, char *buffer, u32 bufferSize)
{        
    PRASSERT(buffer);
    PRASSERT(bufferSize > 0);


    char work[32];

    s32 index = 0;
    s32 count = prStringWriteNumber((u32)num, 10, (num<0), 0, false, work);


    if ((u32)count < (bufferSize-1))
    {
        while(count-- > 0)
        {
            buffer[index] = work[index];            
            index++;
        }
    }

    // Terminate the string.
    buffer[index] = 0;
}


/// ---------------------------------------------------------------------------
/// Safely copies a string.
/// ---------------------------------------------------------------------------
u32 prStringCopySafe(char* dst, const char* src, u32 destSize)
{
    PRASSERT(dst);
    PRASSERT(src);
    PRASSERT(destSize);
    

    // Need to reduce destination size to allow for terminating null.    
    destSize--;
    
    u32 count = 0;

    while(*src && count < destSize)
    {
        *dst++ = *src++;
        
        count++;        
    }
    
    // Ensure destination string is null terminated.
    *dst = 0;

    return count;
}


/// ---------------------------------------------------------------------------
/// Creates then returns a formatted string.
/// ---------------------------------------------------------------------------
const char *prStringPrintf(const char* fmt, ...)
{
    static char buffer[1024];

    if (fmt && *fmt)
    {
        // Format the output.
        va_list args;        
        va_start(args, fmt);
    
    #if defined(PLATFORM_PC)
        vsprintf_s(buffer, sizeof(buffer), fmt, args);
    #else
        vsnprintf(buffer, sizeof(buffer), fmt, args);
    #endif

        va_end(args);
    }
    else
    {
        buffer[0] = 0;
    }

    return buffer;
}


/// ---------------------------------------------------------------------------
/// Creates a formatted string.
/// ---------------------------------------------------------------------------
void prStringSprintf(char *buffer, s32 bufferSize, const char* fmt, ...)
{
    PRASSERT(buffer);
    PRASSERT(bufferSize > 0);
    PRUNUSED(bufferSize);

    if (fmt && *fmt)
    {
        // Format the output.
        va_list args;
        
        va_start(args, fmt);
    
    #if defined(PLATFORM_PC)
        vsprintf_s(buffer, bufferSize, fmt, args);
    #else
        snprintf(buffer, bufferSize, fmt, args);
    #endif

        va_end(args);
    }
    else
    {
        buffer[0] = 0;
    }
}


/// ---------------------------------------------------------------------------
/// Sets a string to lower case.
/// ---------------------------------------------------------------------------
void prStringToLower(char* string)
{
    if (string)
    {
        s32 index = 0;

        while(string[index])
        {
            string[index] = (char)tolower(string[index]);

            index++;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets a string to upper case.
/// ---------------------------------------------------------------------------
void prStringToUpper(char* string)
{
    PRASSERT(string);

    if (string)
    {
        s32 index = 0;

        while(string[index])
        {
            string[index] = (char)toupper(string[index]);

            index++;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Returns the length of a passed string.
/// ---------------------------------------------------------------------------
s32 prStringLength(const char *string)
{
    PRASSERT(string);

    // Set default return value
    s32 length = 0;

    if (string)
    {
        while (*string++) length++;
    }

    return length;
}


/// ---------------------------------------------------------------------------
/// Function places '\n' in a string to indicate where word wrapping should take place.
/// ---------------------------------------------------------------------------
s32 prStringWordWrap(char *string, u32 max_string_width)
{
    s32 line_count = -1;            // Number of lines main string wraps s32o.

    if (string && *string)
    {
        // Start at 1.
        line_count = 1;

        s32 last_space      = -1;           // Index of the last space.
        s32 string_length   =  0;           // Length of the current line.
        s32 word_length     =  0;           // Length of the current word.
        s32 index           =  0;           // Index into the string being marked.

        do
        {
            // Found space or null?
            if (string[index] == ' ' || string[index] == (char)NULL)
            {
                // Exceeded max length?
                if ((string_length + word_length) > (s32)max_string_width)
                {
                    // As we now have a wrap pos32, the new string length must be set to the word length plus 1 for the space.
                    string_length = word_length + 1;

                    // Reset for next word.
                    word_length = 0;

                    // Set a wrap marker in the last space found.
                    if (last_space > -1)
                    {
                        string[last_space] = '\n';

                        // Set number of lines.
                        line_count++;
                    }
                }

                // Nope, still under max length... so
                else
                {
                    // Add words length to current strings length plus 1 for the space.
                    string_length += word_length + 1;

                    // Set where the last space is.
                    last_space = index;

                    // Reset for next word.
                    word_length = 0;
                }
            }

            // No space or null found, so word size must go up.
            else
            {
                // ---------------------------------------------------------
                // Should add check to see if word has exceeded line length.
                // This is however extremely improbable.
                // ---------------------------------------------------------
                word_length++;
            }
        }
        while(string[index++]);
    }

    return line_count;
}


/// ---------------------------------------------------------------------------
/// Changes new lines to spaces. (Part of word wrap group)
/// ---------------------------------------------------------------------------
void prStringRemoveNewLines(char *string)
{
    // PRASSERT invariants.
    PRASSERT(string && *string, "RemoveNewLines - String was null or empty" );
    
    if (string && *string)
    {
        while(*string)
        {
            if (*string == '\n')
                *string =  ' ';

            string++;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Compares two strings
/// ---------------------------------------------------------------------------
prStringResult prStringCompare(const char *firstString, const char *secondString)
{
    if (firstString && secondString)
    {
        bool loop = true;

        do
        {
            char first  = *firstString  ++;
            char second = *secondString ++;

            // First string greater than?
            if (first > second)
            {
                return CMP_GREATERTHAN;
            }

            // First string less than?
            if (first < second)
            {
                return CMP_LESSTHAN;
            }

            // Equals test.
            if (first == (char)NULL && second == (char)NULL)
            {
                return CMP_EQUALTO;
            }
        }
        while(loop);
    }

    return CMP_ERROR;
}


/// ---------------------------------------------------------------------------
/// Compares two strings
/// ---------------------------------------------------------------------------
prStringResult prStringCompareNoCase(const char *firstString, const char *secondString)
{
    if (firstString && secondString)
    {
        bool loop = true;

        do
        {
            char first  = *firstString  ++;
            char second = *secondString ++;

            first  = (char)toupper(first);
            second = (char)toupper(second);

            // First string greater than?
            if (first > second)
            {
                return CMP_GREATERTHAN;
            }

            // First string less than?
            if (first < second)
            {
                return CMP_LESSTHAN;
            }

            // Equals test.
            if (first == (char)NULL && second == (char)NULL)
            {
                return CMP_EQUALTO;
            }
        }
        while(loop);
    }

    return CMP_ERROR;
}


/// ---------------------------------------------------------------------------
/// Compares two strings
/// ---------------------------------------------------------------------------
prStringResult prStringCompareNoCase(const char *firstString, const char *secondString, int max)
{
    if (firstString && secondString)
    {
        bool loop = true;

        int count = 0;

        do
        {
            char first  = *firstString  ++;
            char second = *secondString ++;

            first  = (char)toupper(first);
            second = (char)toupper(second);

            // First string greater than?
            if (first > second)
            {
                return CMP_GREATERTHAN;
            }

            // First string less than?
            if (first < second)
            {
                return CMP_LESSTHAN;
            }

            // Equals test.
            if (first == (char)NULL && second == (char)NULL)
            {
                return CMP_EQUALTO;
            }


            // Max chars to compare?
            count++;
            if (count == max)
            {
                return CMP_EQUALTO;
            }
        }
        while(loop);
    }

    return CMP_ERROR;
}


/// ---------------------------------------------------------------------------
/// Copies from the source string to the destination string all characters
/// upto, but not including the until character.
/// ---------------------------------------------------------------------------
s32 prStringCopyUntil(char *src, char *dst, char until)
{
    s32 copied = 0;

    if (src && dst)
    {
        // While there's no null character and no 'until' character.
        while(*src && *src != until)
        {
            *dst++ = *src++;
        }

        // Add terminating null.
        *dst = (char)NULL;
    }

    return copied;
}


/// ---------------------------------------------------------------------------
/// Finds a sub string within a string.
/// ---------------------------------------------------------------------------
char *prStringFindSubString(char *string, char *search)
{
    if (search && string && *search && *string)
    {
        while (*string)
        {
            // Found first character in search string?
            if (*search == *string)
            {
                char *_search = search;
                char *_string = string;

                do
                {
                    // Pos32 to next 2 characters.
                    ++_search;
                    ++_string;

                    // At end of search string?
                    if (*_search == (char)NULL)
                    {
                        // -----------------------------------------------------
                        // Then search match has been found.
                        // Return start of string where search string was found.
                        // -----------------------------------------------------
                        return string;
                    }
                }
                while(*_search  ==  *_string);
            }

            string++;
        }
    }
    
    return NULL;
}


/// ---------------------------------------------------------------------------
/// Copies one string to another.
/// ---------------------------------------------------------------------------
s32 prStringCopy(char *src, char *dst)
{
    s32 count= 0;

    if (src && dst)
    {
        while(*src)
        {       
            *dst++ = *src++;

            count++;
        }

        // Add terminating null.
        *dst = (char)NULL;
    }

    return count;
}


/// ---------------------------------------------------------------------------
/// Adds one string to another.
/// ---------------------------------------------------------------------------
s32 prStringAddString(const char *src, char *dst)
{
    s32 count = 0;

    if (src && dst)
    {
        // Move to NULL in destination string.
        while(*dst) dst++;

        // Add string.
        while(*src)
        {
            *dst++ = *src++;

            count++;
        }

        // Add terminating null.
        *dst = (char)NULL;
    }

    return count;
}


/// ---------------------------------------------------------------------------
/// Adds a single character to the supplied string.
/// ---------------------------------------------------------------------------
void prStringAddChar(char *string, char character)
{
    if (string)
    {
        while (*string) string++;

        *string++ = character;
        *string   = (char)NULL;
    }
}


/// ---------------------------------------------------------------------------
/// Converts a hex string like 'FAC5012E' to its numerical equivalent.
/// ---------------------------------------------------------------------------
u32 prStringToHex(const char* string)
{
    static const char hex_characters[] = 
    {
        '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
    };

    PRASSERT(string && *string);

    char c;
    u32  num = 0;
    s32  i;

    do
    {
        c = *string++;

        if (c > 0)
        {
            num <<= 4;

            c = (char)toupper(c);

            for (i=0; i<16; i++)
            {
                if (hex_characters[i] == c)
                {
                    break;
                }
            }

            PRASSERT(i<16);

            num |= i;
        }
    }
    while(c);

    return num;
}


/// ---------------------------------------------------------------------------
/// Returns true if a string contains any NON white space characters, else 
/// returns false.
/// ---------------------------------------------------------------------------
bool prStringContainsCharacters(const char* text)
{
    if (text && *text)
    {
        char c;

        do
        {
            c = *text++;

            if (c != ' '  &&
                c != '\r' &&
                c != '\t' &&
                c != '\n' &&
                c != (char)NULL)
            {
                return true;
            }
        }
        while(c);
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Parses a string in the format of "0,0" into integer values
/// ---------------------------------------------------------------------------
void prStringParseInts(const char* text, s32 &a, s32 &b)
{
    PRASSERT(text && *text);

    a = (s32)atoi(text);

    while(*text)
    {
        if (*text == ',')
        {
            b = (s32)atoi(text + 1);
            break;
        }

        text++;
    }
}


/// ---------------------------------------------------------------------------
/// Parses a string in the format of "0,0,0" into int values
/// ---------------------------------------------------------------------------
void prStringParseInts(const char* text, s32 &a, s32 &b, s32 &c)
{
    PRASSERT(text && *text);

    a = (s32)atoi(text);

    while(*text)
    {
        if (*text == ',')
        {
            text++;
            b = (s32)atoi(text);
            break;
        }

        text++;
    }

    while(*text)
    {
        if (*text == ',')
        {
            text++;
            c = (s32)atoi(text);
            break;
        }

        text++;
    }
}


/// ---------------------------------------------------------------------------
/// Parses a string in the format of "0,0" into float values
/// ---------------------------------------------------------------------------
void prStringParseFloats(const char* text, f32 &a, f32 &b)
{
    PRASSERT(text && *text);

    a = (f32)atof(text);

    while(*text)
    {
        if (*text == ',')
        {
            b = (f32)atof(text + 1);
            break;
        }

        text++;
    }
}


/// ---------------------------------------------------------------------------
/// Parses a string in the format of "0,0,0" into float values
/// ---------------------------------------------------------------------------
void prStringParseFloats(const char* text, f32 &a, f32 &b, f32 &c)
{
    PRASSERT(text && *text);

    a = (f32)atof(text);

    while(*text)
    {
        if (*text == ',')
        {
            text++;
            b = (f32)atof(text);
            break;
        }

        text++;
    }

    while(*text)
    {
        if (*text == ',')
        {
            text++;
            c = (f32)atof(text);
            break;
        }

        text++;
    }
}


/// ---------------------------------------------------------------------------
/// Finds if the specified character exists within the supplied string.
/// ---------------------------------------------------------------------------
bool prStringFind(const char* string, char character)
{
    if (string)
    {
        s32  index = 0;
        char c;

        do
        {
            c = string[index];
                
            if (c == character)
            {
                return true;
            }

            index++;
        }
        while(c);
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Decodes four bytes only as this allows for characters from the Greek,
/// Cyrillic, Coptic, Armenian, Hebrew, Arabic, Syriac and Tana alphabets.
/// And almost everything else.
///
/// http://www.codeguru.com/cpp/misc/misc/multi-lingualsupport/article.php/c10451
/// ---------------------------------------------------------------------------
s32 prStringExtractUtf8(const char *string, s32 &extracted)
{
    PRASSERT(string && *string);

    char first  = *string++;
    s32  adjust = 1;

    // Got marker byte?
    if ((first & MASK2BYTES) == MASK2BYTES)
    {
        char second = *string++;
        extracted = ((first & 0x1F) << 6) | (second & MASKBITS);
        adjust += 1;
    }
    else if ((first & MASK3BYTES) == MASK3BYTES)
    {
        char second = *string++;
        char third  = *string++;
        extracted = ((first & 0x0F) << 12) | ((second & MASKBITS) << 6) | (third & MASKBITS);
        adjust += 2;
    }
    else if ((first & MASK4BYTES) == MASK4BYTES)
    {
        char second = *string++;
        char third  = *string++;
        char fourth = *string++;
        extracted = ((first & 0x07) << 18) | ((second & MASKBITS) << 12)| ((third & MASKBITS) << 6) | (fourth & MASKBITS);
        adjust += 3;
    }
    else
    {
        prTrace("Failed to decode utf8 character\n");
    }

    return adjust;
}


/// ---------------------------------------------------------------------------
/// Parses controls in text docs into equivalent embedded control codes.
/// Current does;
/// - \n
/// - \t
/// ---------------------------------------------------------------------------
void prStringParseControls(const char *text, char *buffer)
{
    PRASSERT(text && *text);
    PRASSERT(buffer);

    char c;
    int in  = 0;
    int owt = 0;

    do
    {
        c = text[in++];

        if (c == '\\')
        {
            c = text[in++];

            switch(c)
            {
            case 'n':
                buffer[owt++] = '\n';
                break;

            case 't':
                buffer[owt++] = '\t';
                break;

            default:
                buffer[owt++] = '\\';
                buffer[owt++] = c;
                break;
            }
        }
        else
        {
            buffer[owt++] = c;
        }
    }
    while(c != '\0');
}


/// ---------------------------------------------------------------------------
/// Creates a formatted string.
/// ---------------------------------------------------------------------------
void prSprintf(char *buffer, const char* fmt, ...)
{
    PRASSERT(buffer);

    if (fmt && *fmt)
    {
        // Format the output.
        va_list args;        
        va_start(args, fmt);
    
    #if defined(PLATFORM_BADA)
        vsnprintf(buffer, 4096, fmt, args);
    #else
        vsprintf(buffer, fmt, args);
    #endif

        va_end(args);
    }
    else
    {
        buffer[0] = 0;
    }
}
