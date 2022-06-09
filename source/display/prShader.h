// File: prShader.h
//      A shader class to make usage simpler
/**
 * Copyright 2020 Paul Michael McNab
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


#include <string>
#include <glm/glm.hpp>
#include "../core/prTypes.h"


namespace Proteus { 
namespace Display {

    // Class: prShader
    //      A shader class
    class prShader
    {
    public:
        prShader();
        //~prShader() {}

        // Method: LoadFromMemory
        //      Loads a shader
        //
        // Parameters:
        //      vertexSrc   - A shader string
        //      fragmentSrc - A shader string
        bool LoadFromMemory(const char* vertexSrc, const char* fragmentSrc);

        //    void Attach();
            // use/activate the shader

            // Set this shader to be used.
        void Use();

        // utility uniform functions
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;

        void SetVec2(const std::string& name, const glm::vec2& value) const;
        void SetVec2(const std::string& name, float x, float y) const;

        void SetVec3(const std::string& name, const glm::vec3& value) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;

        void SetVec4(const std::string& name, const glm::vec4& value) const;
        void SetVec4(const std::string& name, float x, float y, float z, float w) const;

        void SetMat2(const std::string& name, const glm::mat2& mat) const;

        void SetMat3(const std::string& name, const glm::mat3& mat) const;

        void SetMat4(const std::string& name, const glm::mat4& mat) const;

        u32 GetID() const { return id; }

    private:
        // Sets the source for a memory based shader
        void SetSource(u32 shader, const char* src);

        // Shows the shader info log
        //void ShaderInfoLog(u32 shader);

        void /*prShader::*/CheckCompileErrors(u32 shader, std::string type);

        // Shows the program info log
        //void ProgramInfoLog(u32 shader);

    private:
        u32 id;     // Shaders id
    };

}}
