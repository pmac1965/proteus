/**
 * prMesh_OBJ.cpp
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


#include <stdlib.h>
#include "prMesh_OBJ.h"
#include "../file/prFile.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prMesh_OBJ::prMesh_OBJ() : prMesh()
{
    pVerts = NULL;
    count  = 0;
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prMesh_OBJ::~prMesh_OBJ()
{
    PRSAFE_DELETE_ARRAY(pVerts);
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
bool prMesh_OBJ::Load(const char *filename)
{
    PRASSERT(filename && *filename);

    prFile *file = new prFile(filename);
    if (file)
    {
        if (file->Open())
        {
            u32 size = file->Size();
            if (size > 0)
            {
                // Create buffer with + 1 for terminating null
                char *buffer = new char [size + 1];

                // Read file
                file->Read(buffer, size);
                file->Close();
                delete file;

                // Terminate read data
                buffer[size] = 0;

                // Parse
                Parse(buffer);
                PRSAFE_DELETE_ARRAY(buffer);

                count = (s32)(faces.size() * 3);

                pVerts = new prVertex3D[count];

                std::list<prFace>::iterator it  = faces.begin();
                std::list<prFace>::iterator end = faces.end();


                prVertex3D *pTempVerts = pVerts;

                for (;it != end; ++it)
                {
                    s32 a = (*it).a;
                    s32 b = (*it).b;
                    s32 c = (*it).c;

                    // Tri 1
                    pTempVerts->x = verts[a].x;
                    pTempVerts->y = verts[a].y;
                    pTempVerts->z = verts[a].z;
                    pTempVerts++;

                    // Tri 2
                    pTempVerts->x = verts[b].x;
                    pTempVerts->y = verts[b].y;
                    pTempVerts->z = verts[b].z;
                    pTempVerts++;

                    // Tri 3
                    pTempVerts->x = verts[c].x;
                    pTempVerts->y = verts[c].y;
                    pTempVerts->z = verts[c].z;
                    pTempVerts++;

                    //prTrace(LogError, "%f, %f, %f, ",  verts[a].x, verts[a].y, verts[a].z);
                    //prTrace(LogError, "%f, %f, %f, ",  verts[b].x, verts[b].y, verts[b].z);
                    //prTrace(LogError, "%f, %f, %f,\n", verts[c].x, verts[c].y, verts[c].z);
                }

                verts.clear();
                faces.clear();
            }
        }
    }

    return false;
}


void prMesh_OBJ::Draw()
{
    glPushMatrix();

    // Render
    glPushAttrib(GL_POLYGON_BIT);

    // Set polyons to clockwise (For back face culling)
    glFrontFace(GL_CCW);

    glBegin (GL_TRIANGLES);
    {
        // Draw each triangle
        for (int i = 0; i < count; i++)
        {
            glVertex3f(pVerts[i].x, pVerts[i].y, pVerts[i].z);           
        }
    }
    glEnd();

    glPopAttrib();

    glPopMatrix();
}


void prMesh_OBJ::Parse(char *buffer)
{
    s32 size;
    //s32 i=0;

    while(ReadLine(buffer, size))
    {
        // Terminate the line
        char c = buffer[size];
        buffer[size] = 0;

        // Parse each line
        ParseLine(buffer);

        // Restore buffer
        buffer[size] = c;

        // Next line start
        buffer += size;
    }
}


bool prMesh_OBJ::ReadLine(char *buffer, s32 &size)
{
    size = 0;

    char *working = buffer;

    //char
    if (working)
    {
        bool loop = true;

        do
        {
            char c = *working++;

            // File ended?
            if (c == '\0')
            {
                size = (s32)(working - buffer);
                return false;
            }

            // Line end?
            if (c == '\r')
            {
                //prTrace(LogError, "CR\n");
                continue; // Until we get '\n'
            }

            // Line end?
            if (c == '\n')
            {
                size = (s32)(working - buffer);
                return true;
            }
        }
        while(loop);
    }

    return false;
}


void prMesh_OBJ::ParseLine(char *buffer)
{
    if (buffer)
    {
        s32 len = prStringLength(buffer);//strlen(buffer);

        //prTrace(LogError, "Size %03i: %s", len, buffer);

        if (len >= 2)
        {
            char c1 = *buffer ++;
            char c2 = *buffer ++;

            // Vertex data
            if (c1 == 'v' && c2 == ' ')
            {
                buffer = prStringSkipWhitespace(buffer);        // Skip leading whitespace
                f32 x  = (f32)atof(buffer);                     // Converts
                buffer = prStringSkipToWhitespace(buffer);      // Skip to next whitespace


                buffer = prStringSkipWhitespace(buffer);
                f32 y  = (f32)atof(buffer);
                buffer = prStringSkipToWhitespace(buffer);

                buffer = prStringSkipWhitespace(buffer);
                f32 z  = (f32)atof(buffer);

                //prTrace(LogError, "v %f, %f, %f\n", x,y,z);

                prVertex3D v;
                v.x = x;
                v.y = y;
                v.z = z;

                verts.push_back(v);
            }
            
            // Face data
            if (c1 == 'f' && c2 == ' ')
            {
                buffer = prStringSkipWhitespace(buffer);        // Skip leading whitespace
                s32 a  = atoi(buffer);                          // Converts
                buffer = prStringSkipToWhitespace(buffer);      // Skip to next whitespace


                buffer = prStringSkipWhitespace(buffer);
                s32 b  = atoi(buffer);
                buffer = prStringSkipToWhitespace(buffer);

                buffer = prStringSkipWhitespace(buffer);
                s32 c  = atoi(buffer);

                //prTrace(LogError, "f %i, %i, %i\n", a, b, c);
                prFace f;
                f.a = a - 1;
                f.b = b - 1;
                f.c = c - 1;

                faces.push_back(f);
            }
        }
    }
}


#endif
