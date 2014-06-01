// File: prMesh_MD2.h
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


#ifndef __PRMESHMD2_H
#define __PRMESHMD2_H


#include "prMesh.h"
#include "prMD2.h"
#include "../core/prTypes.h"
#include "../file/prFileShared.h"


// Forward declarations
class prAnimation_MD2;


/** Base mesh class
 */
class prMesh_MD2 : public prMesh
{
public:

    prMesh_MD2();//const char *filename);
    ~prMesh_MD2();


    /** Renders the mesh.
     */
    void Draw();

    void Update();
    //{}


    /** Animates the mesh.
     */
    void Animate();


    /** Loads the mesh.
     */
    bool Load(const char *filename);


    void PlayA(const char *name);


private:

    /**
     */
    void RenderFrame(int frame);


private:
    prAnimation_MD2 *mpAnimation;
    prMD2Skin       *m_skins;
    prMD2TexCoord   *m_texCoords;
    prMD2Triangle   *m_triangles;
    prMD2Frame      *m_frames;
    int             *m_glcmds;

    int             m_numTriangles;
    int             m_numFrames;
    int             m_skinWidth;
    int             m_skinHeight;
    float           m_scale;
    int             m_frame;
};


#endif//__PRMESHMD2_H
