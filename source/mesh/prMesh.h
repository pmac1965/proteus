// File: prMesh.h
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


#ifndef __PRMESH_H
#define __PRMESH_H


// Forward declarations
class prMeshLoader;


// Class: prMesh
//      A 3D mesh class
class prMesh
{
public:
    // Method: prMesh
    //      Ctor
    prMesh();

    // Method: ~prMesh
    //      Dtor
    ~prMesh();

    // Method: Load
    //      Loads a 3D mesh
    bool Load(const char *filename);

    // Method: Update
    //      Updates a 3D meshes animation if it is animated
    void Update();

    // Method: Draw
    //      Draws a 3D mesh
    void Draw();


private:

    prMeshLoader    *pLoader;
};


#endif//__PRMESH_H
