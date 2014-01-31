/**
 * @file       prString.h
 * @brief      Contains basic fixed size string class.
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


#ifndef __PRSTRING_H
#define __PRSTRING_H


#include "prTypes.h"
#include "prStringShared.h"


// Defines
#define STRING_BUFFER_SIZE  256


/// @brief      Fixed size string class. This class is designed not to
/// @n          allocate memory during usage. Use where you know string size
/// @n          will be limited.
class prString
{
public:

    /// @brief      Ctor
    prString();

    /// @brief      Ctor
    /// @param      text - The string to set
    prString(const char *text);

    /// @brief      Copy ctor
    /// @param      str - String to copy
    prString(const prString &str);

    /// @brief      Sets the string as empty, but not NULL
    void Clear();

    /// @brief      Sets the strings text
    /// @param      text - The string to set
    /// @note       The passed text can be NULL or empty ""
    void Set(const char *text);

    /// @brief      Adds to the string.
    /// @param      str - String to add
    void Append(const prString &str);

    /// @brief      Adds to the string.
    /// @param      text - Text to add
    /// @note       The passed text can be NULL or empty ""
    void Append(const char *text);

    /// @brief      Removes any leading spaces.
    void TrimFront();

    /// @brief      Removes any trailing spaces.
    void TrimBack();

    /// @brief      Removes any leading/trailing spaces.
    void Trim();

    /// @brief      Compares string for equality.
    /// @param      text - Text to compare
    /// @return     CMP_ERROR
    /// @return     CMP_LESSTHAN
    /// @return     CMP_EQUALTO
    /// @return     CMP_GREATERTHAN
    /// @see        prStringResult
    prStringResult Compare(const char *text);

    /// @brief      Compares string for equality.
    /// @param      str - String to compare
    /// @return     CMP_ERROR
    /// @return     CMP_LESSTHAN
    /// @return     CMP_EQUALTO
    /// @return     CMP_GREATERTHAN
    /// @see        prStringResult
    prStringResult Compare(const prString &str);

    /// @brief      Replaces all occurences of 'findChar' with 'replaceChar'
    void Replace(char findChar, char replaceChar);

    /// @brief      Makes the string uppercase
    void ToUpper();

    /// @brief      Makes the string lowercase
    void ToLower();

    /// @brief      Creates a string using a standard C format string
    /// @param      fmt - The format string
    /// @param      ... - Optional parameters
    void Sprintf(const char *fmt, ...);

    /// @brief      Gets the length of the string
    s32 Length() const { return m_length; }

    /// @brief      Gets the strings text
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


#endif//_PRSTRING_H
