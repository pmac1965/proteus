// File: prMesh_OBJ.h
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


#ifndef __PRMESH_OBJ_H
#define __PRMESH_OBJ_H


#include "prMesh.h"
#include "../core/prTypes.h"
#include "../core/prVertex.h"
#include <vector>
#include <list>


// Tri face
typedef struct
{
    Proteus::Core::s32 a, b, c;

} prFace;


// Method: prMeshLoader_OBJ
//      A 3D mesh loader class for wavefront obj files
class prMesh_OBJ : public prMesh
{
public:
    // Method: prMesh_OBJ
    //      Ctor
    prMesh_OBJ();

    // Method: prMesh_OBJ
    //      Dtor
    ~prMesh_OBJ();

    // Method: Load
    //      Overridden load method
    bool Load(const char *filename);

    // Method: Update
    //      Updates a 3D meshes animation if it is animated
    void Update() {}

    // Method: Draw
    //      Draws a 3D mesh
    void Draw();


private:

    void Parse(char *buffer);
    bool ReadLine(char *buffer, Proteus::Core::s32 &size);
    void ParseLine(char *buffer);
    

private:

    std::vector<prVertex3D> verts;
    std::list<prFace>       faces;
    prVertex3D             *pVerts;
    Proteus::Core::s32      count;
};


#endif//__PRMESH_OBJ_H
