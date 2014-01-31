/**
 * @file       prOglUtils.h
 * @brief      Contains OpenGL helper functions.
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


#ifndef __PROGLUTILS_H
#define __PROGLUTILS_H


// Forward declarations
class Texture;


/// @brief      Shows the opengl extensions available for the current platform.
void prOpenGLShowExtensions();

/// @brief      Draws the engine water mark.
/// @param      pTexture - A pointer to the watermark texture
void prDrawWaterMark(Texture *pTexture);

/// @brief      Checks if the previous action caused an error, If so its displayed
/// @param      file - A char pointer to the file name
/// @param      func - A char pointer to the functions name
/// @param      line - Line number of the function that caused error
void prOpenGLErrorCheck(const char *file, const char *func, int line);


// Removeable calls
#if defined(_DEBUG) || defined(DEBUG)
    #define ERR_CHECK() prOpenGLErrorCheck(__FILE__, __FUNCTION__, __LINE__)
#else
    #define ERR_CHECK()
#endif


#endif//__PROGLUTILS_H
