/**
 * prMesh_MD2.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include "prMesh_MD2.h"
#include "prNormals_MD2.h"
#include "prAnimation_MD2.h"
#include "../core/prStringUtil.h"
#include "../core/prCore.h"
#include "../core/prResourceManager.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../display/prTexture.h"
#include "../file/prFileShared.h"
#include <stdio.h>
#include <fstream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


//using namespace Proteus::Core;


// Debug
#define MD2_DEBUG


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prMesh_MD2::prMesh_MD2() : prMesh()
{
    mpAnimation     = NULL;
    m_skins         = NULL;
    m_texCoords     = NULL;
    m_triangles     = NULL;
    m_frames        = NULL;
    m_glcmds        = NULL;
    m_numTriangles  = 0;
    m_skinWidth     = 0;
    m_skinHeight    = 0;
    m_scale         = 1.0f;
    m_frame         = 0;
    m_numFrames     = 0;
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prMesh_MD2::~prMesh_MD2()
{
    PRSAFE_DELETE_ARRAY(m_skins);
    PRSAFE_DELETE_ARRAY(m_texCoords);
    PRSAFE_DELETE_ARRAY(m_triangles);
    PRSAFE_DELETE_ARRAY(m_frames);
    PRSAFE_DELETE_ARRAY(m_glcmds);
    PRSAFE_DELETE(mpAnimation);
}


/// ---------------------------------------------------------------------------
/// Renders the mesh.
/// ---------------------------------------------------------------------------
void prMesh_MD2::Draw()
{
    if (mLoaded)
    {
        glPushMatrix();

            // Axis rotation - Makes test mesh upright!
            glRotatef(270.0f, 1.0f, 0.0f, 0.0f);


            // Wireframe?
            if (mWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }


            // Render
            glPushAttrib(GL_POLYGON_BIT);

                // Set polyons to clockwise (For back face culling)
                glFrontFace(GL_CW);

                RenderFrame(m_frame);

                glFrontFace(GL_CCW);

            glPopAttrib();

        glPopMatrix();
    }
}


/// ---------------------------------------------------------------------------
/// Renders the mesh.
/// ---------------------------------------------------------------------------
void prMesh_MD2::Update()
{
    Animate();
}


// Animates the mesh.
//
void prMesh_MD2::Animate()
{
    if (mLoaded)
    {
        if (mpAnimation)
        {
            //mpAnimation->
            mpAnimation->Update(16.0f/1000.0f);

            m_frame = static_cast<prAnimation_MD2*>(mpAnimation)->GetFrame();
            prTrace(prLogLevel::LogError, "Frame %i\n", m_frame);
        }
    }
}


// Loads the mesh.
//
bool prMesh_MD2::Load(const char *filename)
{
    // Loaded?
    if (!mLoaded)
    {
        // Open the file
        std::ifstream ifs(filename, std::ios::binary);

        if (ifs.fail())
        {
            PRWARN("Failed to open model file %s", filename);
            return false;
        }


        // Is the file too small?
        ifs.seekg(0, std::ios::end);
        int size = (int)ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        if (size <= sizeof(prMD2Header))
        {
            PRWARN("File '%s' is not an MD2 file. It is too small", filename);
            ifs.close();
            return false;
        }


        // Load the header.
        prMD2Header header;

        if (ifs.read(reinterpret_cast<char*>(&header), sizeof(header)))
        {
            if (header.ident != MD2_MAGIC || header.version != MD2_VERSION)
            {
                PRWARN("File '%s' is not an MD2 file.", filename);
                ifs.close();
                return false;
            }


            // Display file details
            #ifdef MD2_DEBUG
            prTrace(prLogLevel::LogError, "-----------------------------------------\n");
            prTrace(prLogLevel::LogError, "File       :     %s\n", filename            );
            prTrace(prLogLevel::LogError, "-----------------------------------------\n");
            prTrace(prLogLevel::LogError, "Ident      :     %x\n", header.ident        );
            prTrace(prLogLevel::LogError, "Version    :     %i\n", header.version      );
            prTrace(prLogLevel::LogError, "Frames     :     %i\n", header.num_frames   );
            prTrace(prLogLevel::LogError, "commands   :     %i\n", header.num_glcmds   );
            prTrace(prLogLevel::LogError, "Triangles  :     %i\n", header.num_tris     );
            prTrace(prLogLevel::LogError, "Tex coords :     %i\n", header.num_st       );
            prTrace(prLogLevel::LogError, "Vertices   :     %i\n", header.num_vertices );
            prTrace(prLogLevel::LogError, "Textures   :     %i\n", header.num_skins    );
            prTrace(prLogLevel::LogError, "Tex width  :     %i\n", header.skinWidth    );
            prTrace(prLogLevel::LogError, "Tex height :     %i\n", header.skinHeight   );
            prTrace(prLogLevel::LogError, "Frame size :     %i\n", header.frameSize    );
            prTrace(prLogLevel::LogError, "-----------------------------------------\n");
            #endif


            // Memory allocation for model data
            m_texCoords = new prMD2TexCoord [header.num_st];
            m_triangles = new prMD2Triangle [header.num_tris];
            m_frames    = new prMD2Frame    [header.num_frames];
            m_glcmds    = new int           [header.num_glcmds];


            // Read skin names
            if (header.num_skins > 0)
            {
                m_skins = new prMD2Skin [header.num_skins];

                ifs.seekg(header.offset_skins, std::ios::beg);
                ifs.read(reinterpret_cast<char*>(m_skins), sizeof (prMD2Skin) * header.num_skins);

                #ifdef MD2_DEBUG
                for (int i=0; i<header.num_skins; i++)
                {
                    prTrace(prLogLevel::LogError, "Skin: %s\n", m_skins[i].name);

                    char buffer[FILE_MAX_FILENAME_SIZE];

                    prStringCopySafe(buffer, m_skins[i].name, sizeof(buffer));
                    prTrace(prLogLevel::LogError, "Skin 1: %s\n", buffer);

                    prStringToLower(buffer);
                    prTrace(prLogLevel::LogError, "Skin 2: %s\n", buffer);

                    prStringReplaceChar(buffer, '\\', '/');
                    prTrace(prLogLevel::LogError, "Skin 3: %s\n", buffer);

                    s32 index = prStringFindLastIndex(buffer, '/');
                    if (index > 0)
                    {
                        prStringCopySafe(buffer, &buffer[index + 1], sizeof(buffer));
                    }

                    //prStringReplaceChar(buffer, '\\', '/');
                    prTrace(prLogLevel::LogError, "Skin 4: %s\n", buffer);

                    index = prStringFindLastIndex(buffer, '.');
                    if (index > 0)
                    {
                        buffer[index] = 0;
                    //    prStringCopySafe(buffer, m_skins[i].name, sizeof(buffer));
                    }
                    prTrace(prLogLevel::LogError, "Skin 5: %s\n", buffer);
                    //else
                    //{
                    //}

                    prStringAddString(".pvr", buffer);
                    prTrace(prLogLevel::LogError, "Skin 6: %s\n", buffer);

                    char fullpath[FILE_MAX_FILENAME_SIZE];
                    sprintf(fullpath, "data/textures/%s", buffer);

                    prTrace(prLogLevel::LogError, "Texture: '%s'\n", fullpath);
                    
                    // Load the texture
                    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
                    if (pRM)
                    {
                        mpTexture = pRM->Load<prTexture>(fullpath);

                        //pRM->Load(fullpath);//  >Unload((*it).texture);
                    }            

                }
                #endif
            }
            else
            {
                prTrace(prLogLevel::LogError, "Model %s has no associated textures\n", filename);
            }


            // Read texture coords.
            ifs.seekg(header.offset_st, std::ios::beg);
            ifs.read(reinterpret_cast<char*>(m_texCoords), sizeof(prMD2TexCoord) * header.num_st);

            // Read triangle data
            ifs.seekg(header.offset_tris, std::ios::beg);
            ifs.read(reinterpret_cast<char*>(m_triangles), sizeof(prMD2Triangle) * header.num_tris);

            // Read frames
            ifs.seekg(header.offset_frames, std::ios::beg);
            
            for (int i=0; i<header.num_frames; i++)
            {
                // Memory allocation for the vertices of this frame
                m_frames[i].verts = new prMD2Vertex[header.num_vertices];

                // Read frame data
                ifs.read(reinterpret_cast<char*>(&m_frames[i].scale),    sizeof(prMD2Vec3));
                ifs.read(reinterpret_cast<char*>(&m_frames[i].translate),sizeof(prMD2Vec3));
                ifs.read(reinterpret_cast<char*>(&m_frames[i].name),     sizeof(char) * 16);
                ifs.read(reinterpret_cast<char*>( m_frames[i].verts),    sizeof(prMD2Vertex) * header.num_vertices);

                #ifdef MD2_DEBUG
                prTrace(prLogLevel::LogError, "Name: %s\n", &m_frames[i].name);
                #endif
            }

            // Read OpenGL commands
            ifs.seekg(header.offset_glcmds, std::ios::beg);
            ifs.read(reinterpret_cast<char*>(m_glcmds), sizeof(int) * header.num_glcmds);


            // Store some of the header data.
            m_numTriangles  = header.num_tris;
            m_skinWidth     = header.skinWidth;
            m_skinHeight    = header.skinHeight;
            m_numFrames     = header.num_frames;


            // Done.
            mLoaded = true;
            ifs.close();


            // Create the animation data
            if (m_numFrames > 0)
            {
                mpAnimation = new prAnimation_MD2(m_frames, m_numFrames);
            }
        }
        else
        {
            PRWARN("Failed to load mesh '%s'.", filename);
        }
    }

    return true;
}


// Renders the mesh.
//
void prMesh_MD2::RenderFrame(int frame)
{
    // Valid frame?
    if (!PRBETWEEN(frame, 0, m_numFrames - 1))
    {
        prTrace(prLogLevel::LogError, "prMesh_MD2 - Invalid frame index\n");
        return;
    }

    
    // Bind texture
    if (mpTexture)
    {
        //glEnable(GL_TEXTURE_2D);
        mpTexture->Bind();
    }

    glBegin (GL_TRIANGLES);
    {
        // Draw each triangle
        for (int i = 0; i < m_numTriangles; i++)
        {
            // Draw each vertex of this triangle
            for (int j = 0; j < 3; j++)
            {
                prMD2Frame    *pFrame     = &m_frames[frame];
                prMD2Vertex   *pVert      = &pFrame->verts[m_triangles[i].vertex[j]];
                prMD2TexCoord *pTexCoords = &m_texCoords[m_triangles[i].st[j]];

                // Compute final texture coords.
                float s = static_cast<float>(pTexCoords->s) / m_skinWidth;
                float t = static_cast<float>(pTexCoords->t) / m_skinHeight;

                glTexCoord2f (s, 1.0f - t);
                //OGL_ErrorCheck();

                // Send normal vector to OpenGL
                glNormal3fv(md2Normals[pVert->normalIndex]);
                //OGL_ErrorCheck();

                // Uncompress vertex position and scale it
                prMD2Vec3 vector;

                vector[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * m_scale;
                vector[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * m_scale;
                vector[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * m_scale;

                glVertex3fv(vector);                
                //OGL_ErrorCheck();
            }
        }
    }
    glEnd();
}


void prMesh_MD2::PlayA(const char *name)
{
    if (mpAnimation)
    {
        mpAnimation->Play(name);
    }
}


#endif
