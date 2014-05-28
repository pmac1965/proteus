// File: prMeshLoader_OBJ.h
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


#ifndef __PRMESHLOADEROBJ_H
#define __PRMESHLOADEROBJ_H


#include "prMeshLoader.h"


// Method: prMeshLoader_OBJ
//      A 3D mesh loader class for wavefront obj files
class prMeshLoader_OBJ : public prMeshLoader
{
public:
    // Method: prMeshLoader_OBJ
    //      Ctor
    prMeshLoader_OBJ();

    // Method: prMeshLoader_OBJ
    //      Dtor
    ~prMeshLoader_OBJ();

    // Method: Load
    //      Overridden load method
    bool Load(const char *filename);
};


#endif//__PRMESHLOADEROBJ_H
