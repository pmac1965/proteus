/**
 * prString.cpp
 * Contains basic fixed size string class.
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "prString.h"
#include "prDefines.h"
#include "prStringUtil.h"
#include "../debug/prDebug.h"
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prString::prString()
{
    Set(NULL);
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prString::prString(const char *text)
{
    Set(text);
}


/// ---------------------------------------------------------------------------
/// Copy ctor
/// ---------------------------------------------------------------------------
prString::prString(const prString &str)
{
    Set(str.Text());
}


/// ---------------------------------------------------------------------------
/// Sets the string as empty, but not NULL
/// ---------------------------------------------------------------------------
void prString::Clear()
{
    m_buffer[0] = 0;
    m_length    = 0;
}


/// ---------------------------------------------------------------------------
/// Sets the strings text.
/// ---------------------------------------------------------------------------
void prString::Set(const char *text)
{
    // NULL?
    if (text == NULL)
    {
        m_hash = 0xFFFFFFFF;
        m_text = NULL;
        Clear();
        return;
    }

    // Empty?
    if (*text == (char)NULL)
    {
        m_hash = 0xFFFFFFFF;
        m_text = m_buffer;
        Clear();
        return;
    }

    // Copy the string.
    m_length = prStringCopySafe(m_buffer, text, sizeof(m_buffer));
    m_text   = m_buffer;
    m_hash   = 0xFFFFFFFF;
}


/// ---------------------------------------------------------------------------
/// Adds to the string.
/// ---------------------------------------------------------------------------
void prString::Append(const prString &str)
{
    Append(str.Text());
}


/// ---------------------------------------------------------------------------
/// Adds to the string.
/// ---------------------------------------------------------------------------
void prString::Append(const char *text)
{
    if (text && *text)
    {
        while(*text && m_length < (s32)(sizeof(m_buffer) - 1))
        {
            m_buffer[m_length++] = *text++;
        }

        m_buffer[m_length] = '\0';
    }

    TODO("Add hash generation")
}


/// ---------------------------------------------------------------------------
/// Removes any leading spaces.
/// ---------------------------------------------------------------------------
void prString::TrimFront()
{
    if (m_length > 0)
    {
        if (m_buffer[0] == ' ')
        {
            char *s = &m_buffer[1];

            while(*s == ' ')
            {
                s++;
            }

            Set(s);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Removes any trailing spaces.
/// ---------------------------------------------------------------------------
void prString::TrimBack()
{
    while(m_length > 0 && m_buffer[m_length - 1] == ' ')
    {
        m_buffer[--m_length] = '\0';
    }
}


/// ---------------------------------------------------------------------------
/// Removes any leading/trailing spaces.
/// ---------------------------------------------------------------------------
void prString::Trim()
{
    TrimFront();
    TrimBack();
}


/// ---------------------------------------------------------------------------
/// Compares string for equality.
/// ---------------------------------------------------------------------------
prStringResult prString::Compare(const char *text)
{
    return prStringCompare(m_buffer, text);
}


/// ---------------------------------------------------------------------------
/// Compares string for equality.
/// ---------------------------------------------------------------------------
prStringResult prString::Compare(const prString &str)
{
    return prStringCompare(m_buffer, str.Text());
}


/// ---------------------------------------------------------------------------
/// Replaces all occurences of 'findChar' with 'replaceChar'
/// ---------------------------------------------------------------------------
void prString::Replace(char findChar, char replaceChar)
{
    if (m_length > 0)
    {
        char *string = m_buffer;

        while(*string)
        {
            if (*string == findChar)
                *string = replaceChar;

            string++;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Makes the string uppercase
/// ---------------------------------------------------------------------------
void prString::ToUpper()
{
    s32 index = 0;

    while(m_buffer[index])
    {
        m_buffer[index] = (char)toupper(m_buffer[index]);
        index++;
    }
}


/// ---------------------------------------------------------------------------
/// Makes the string lowercase
/// ---------------------------------------------------------------------------
void prString::ToLower()
{
    s32 index = 0;

    while(m_buffer[index])
    {
        m_buffer[index] = (char)tolower(m_buffer[index]);
        index++;
    }
}


/// ---------------------------------------------------------------------------
/// Creates a string using a standard C format string
/// ---------------------------------------------------------------------------
void prString::Sprintf(const char *fmt, ...)
{
    // No change
    if (!fmt)
        return;

    // Got at least 1 character.
    if (*fmt)
    {
        char buffer[STRING_BUFFER_SIZE];

        // Format the output.
        va_list args;        
        va_start(args, fmt);  

#if defined(PLATFORM_PC)
        vsprintf_s(buffer, sizeof(buffer), fmt, args);
#elif defined(PLATFORM_BADA)
        vsnprintf(buffer, sizeof(buffer), fmt, args);
#else
        vsprintf(buffer, fmt, args);
#endif

        va_end(args);
        Set(buffer);
    }

    // Empty string
    else
    {
        Clear();
    }
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const prString& rhs) const
{
    prString s(*this);
    s.Append(rhs);
    return s;
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const char rhs) const
{
    prString s(*this);
    char buffer[32];
#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%c", rhs);
#else
    sprintf(buffer, "%c", rhs);
#endif
    s.Append(buffer);
    return s;
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const s8 rhs) const
{
    prString s(*this);
    char buffer[32];
#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%i", rhs);
#else
    sprintf(buffer, "%i", rhs);
#endif
    s.Append(buffer);
    return s;
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const s16 rhs) const
{
    prString s(*this);
    char buffer[32];
#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%hi", rhs);
#else
    sprintf(buffer, "%hi", rhs);
    s.Append(buffer);
#endif
    return s;
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const s32 rhs) const
{
    prString s(*this);
    char buffer[32];
#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%i", rhs);
#else
    sprintf(buffer, "%i", rhs);
#endif
    s.Append(buffer);
    return s;
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const f32 rhs) const
{
    prString s(*this);
    char buffer[32];
#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%f", rhs);
#else
    sprintf(buffer, "%f", rhs);
#endif
    s.Append(buffer);
    return s;
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const s64 rhs) const
{
    prString s(*this);
    char buffer[32];
#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%lli", rhs);
#else
    sprintf(buffer, "%lli", rhs);
#endif
    s.Append(buffer);
    return s;
}


// ----------------------------------------------------------------------------
// Operator +
// ----------------------------------------------------------------------------
prString prString::operator + (const f64 rhs) const
{
    prString s(*this);
    char buffer[32];
#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%lf", rhs);
#else
    sprintf(buffer, "%lf", rhs);
#endif
    s.Append(buffer);
    return s;
}


// ----------------------------------------------------------------------------
// Operator [] - Array access.
// ----------------------------------------------------------------------------
char& prString::operator [] (unsigned int index)
{
    PRASSERT(index < (unsigned int)m_length);
    return m_buffer[index];
}
