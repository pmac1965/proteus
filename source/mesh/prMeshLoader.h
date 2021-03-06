// File: prMeshLoader.h
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


#ifndef __PRMESHLOADER_H
#define __PRMESHLOADER_H


// Supported loader types
enum
{
    LOADER_TYPE_OBJ,
};


// Method: prMeshLoader
//      A 3D mesh loader base class
class prMeshLoader
{
public:
    // Method: prMeshLoader
    //      Ctor
    prMeshLoader();

    // Method: prMeshLoader
    //      Dtor
    virtual ~prMeshLoader();

    // Method: Load
    //      This methid needs to be overridden.
    virtual bool Load(const char *filename) = 0;
};


#endif//__PRMESHLOADER_H
