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


#pragma once


// Forward declarations
class prTexture;


// Class: prMesh
//      A 3D mesh class base class
class prMesh
{
public:
    // Method: prMesh
    //      Ctor
    prMesh();

    // Method: ~prMesh
    //      Dtor
    virtual ~prMesh();

    // Method: Load
    //      Loads a 3D mesh
    virtual bool Load(const char *filename) = 0;

    // Method: Update
    //      Updates a 3D meshes animation if it is animated
    virtual void Update() = 0;

    // Method: Draw
    //      Draws a 3D mesh
    virtual void Draw() = 0;
    
    // Method: SetWireframe
    //      Sets the mesh to render as a wireframe.
    void SetWireframe(bool state) { mWireframe = state; }

    // Method: SetWireframe
    //      Gets the meshes wireframe state.
    bool GetWireframe() const { return mWireframe; }


protected:

    prTexture  *mpTexture;

    bool        mLoaded;
    bool        mWireframe;
};
