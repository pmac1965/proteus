/**
 * @file       prStringShared.h
 * @brief      Contains shared enums for strings.
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


#ifndef __PRSTRINGSHARED_H
#define __PRSTRINGSHARED_H


/// @enum   prStringResult
/// @brief  String comparison results
/// @see    StringResult prStringCompare(const char *firstString, const char *secondString);
/// @see    StringResult prStringCompareNoCase(const char *firstString, const char *secondString);
/// @see    StringResult prStringCompareNoCase(const char *firstString, const char *secondString, int max);
enum prStringResult
{
    CMP_ERROR       = -2,       ///< An error occurred
    CMP_LESSTHAN    = -1,       ///< Less than
    CMP_EQUALTO     =  0,       ///< Equal to
    CMP_GREATERTHAN =  1        ///< Greater than
};


#endif//__PRSTRINGSHARED_H
