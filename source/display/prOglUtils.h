// File: prOglUtils.h
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


#ifndef __PROGLUTILS_H
#define __PROGLUTILS_H


// Forward declarations
class prTexture;


// Function: prOpenGLShowExtensions
//      Shows the opengl extensions available for the current platform.
void prOpenGLShowExtensions();

// Function: prDrawWaterMark
//      Draws the engine water mark.
//
// Parameters:
//      pTexture - A pointer to the watermark texture
void prDrawWaterMark(prTexture *pTexture);

// Function: prDrawWaterMark
//      Checks if the previous action caused an error, If so its displayed
//
// Parameters:
//      file - A char pointer to the file name
//      func - A char pointer to the functions name
//      line - Line number of the function that caused error
void prOpenGLErrorCheck(const char *file, const char *func, int line);


// Removeable calls
#if defined(_DEBUG) || defined(DEBUG)
    #define ERR_CHECK()     prOpenGLErrorCheck(__FILE__, __FUNCTION__, __LINE__)
#else
    #define ERR_CHECK()
#endif


#endif//__PROGLUTILS_H
