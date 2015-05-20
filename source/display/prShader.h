// File: prShader.h
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


#ifndef __PRSHADER_H
#define __PRSHADER_H


#include "../core/prTypes.h"


// Class: prShader
//      A shader class
class prShader
{
public:
    prShader();
    ~prShader();

    // Method: LoadFromMemory
    //      Loads a shader
    //
    // Parameters:
    //      vertexSrc   - 
    //      fragmentSrc - 
    bool LoadFromMemory(const char *vertexSrc, const char *fragmentSrc);

    void Attach();

private:
    // Sets the source for a memory based shader
    void SetSource(Proteus::Core::u32 shader, const char *src);

    // Shows the shader info log
    void ShaderInfoLog(Proteus::Core::u32 shader);

    // Shows the program info log
    void ProgramInfoLog(Proteus::Core::u32 shader);
};


#endif//__PRSHADER_H
