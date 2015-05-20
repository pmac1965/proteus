// File: prMD2.h
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


#ifndef __PRMD2_H
#define __PRMD2_H


#include "../core/prTypes.h"
#include "../core/prDefines.h"
#include "../core/prMacros.h"
#include "../file/prFileShared.h"


/** MD2 file header.
 */
typedef struct
{
    Proteus::Core::s32     ident;              // Magic number. must be equal to "IDP2"
    Proteus::Core::s32     version;            // Md2 version. must be equal to 8

    Proteus::Core::s32     skinWidth;          // Width of the texture
    Proteus::Core::s32     skinHeight;         // Height of the texture
    Proteus::Core::s32     frameSize;          // Size of one frame in bytes

    Proteus::Core::s32     num_skins;          // Number of textures
    Proteus::Core::s32     num_vertices;       // Number of vertices
    Proteus::Core::s32     num_st;             // Number of texture coordinates
    Proteus::Core::s32     num_tris;           // Number of triangles
    Proteus::Core::s32     num_glcmds;         // Number of opengl commands
    Proteus::Core::s32     num_frames;         // Total number of frames

    Proteus::Core::s32     offset_skins;       // Offset to skin names (64 bytes each)
    Proteus::Core::s32     offset_st;          // Offset to s-t texture coordinates
    Proteus::Core::s32     offset_tris;        // Offset to triangles
    Proteus::Core::s32     offset_frames;      // Offset to frame data
    Proteus::Core::s32     offset_glcmds;      // Offset to opengl commands
    Proteus::Core::s32     offset_end;         // Offset to end of file

} prMD2Header;


/** MD2 Skin data.
 */
typedef struct
{
  char name[64];                // Texture's filename

} prMD2Skin;


/** MD2 Texture coords.
 */
typedef struct
{
  Proteus::Core::s16 s;                        // Coord
  Proteus::Core::s16 t;                        // Coord

} prMD2TexCoord;


/** MD2 Triangle data.
 */
typedef struct
{
  Proteus::Core::u16 vertex[3];                // Triangle's vertex indices
  Proteus::Core::u16 st[3];                    // Texture coord indices

} prMD2Triangle;


/** MD2 Vertex data.
 */
typedef struct
{
  Proteus::Core::u8 v[3];                      // Compressed vertex position
  Proteus::Core::u8 normalIndex;               // Normal vector index

} prMD2Vertex;


// OpenGL vector types.
typedef Proteus::Core::f32 prMD2Vec2[2];
typedef Proteus::Core::f32 prMD2Vec3[3];


/** MD2 Frame data.
 */
typedef struct prMD2Frame
{
    // Constructor.
    prMD2Frame()
    {
        scale[0]     = 0.0f;
        scale[1]     = 0.0f;
        scale[2]     = 0.0f;
        translate[0] = 0.0f;
        translate[1] = 0.0f;
        translate[2] = 0.0f;        
        name[0]      = 0;
        verts        = NULL;
    }

    // Destructor
    ~prMD2Frame() 
    {
        PRSAFE_DELETE_ARRAY(verts); 
    }

    prMD2Vec3    scale;           // Scale factors
    prMD2Vec3    translate;       // Translation vector
    char         name[16];        // Frame name
    prMD2Vertex *verts;           // Frames's vertex list

} prMD2Frame;


/** OpenGL command packet
 */
typedef struct
{
    Proteus::Core::f32 s;                      // S texture coord.
    Proteus::Core::f32 t;                      // T texture coord.
    Proteus::Core::s32 index;                  // Vertex index

} prMD2GLCMD;


/** MD2 Animation information
 */
typedef struct
{
    char                   name[16];           // Frame name
    Proteus::Core::u32     hash;               // Frame hash
    Proteus::Core::s32     start;              // First frame index
    Proteus::Core::s32     end;                // Last frame index

} prMD2Anim;


// Defines
#define MD2_MAGIC               PRMAKE4('2', 'P', 'D', 'I')
#define MD2_VERSION             8
#define MD2_MAX_FRAMES          199


#endif//__PRMD2_H
