/**
 * prMesh.cpp
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


#include "prMesh.h"
#include "prMeshLoader.h"
#include "prMeshLoader_OBJ.h"
#include "../core/prDefines.h"
#include "../core/prmacros.h"
#include "../core/prStringUtil.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prMesh::prMesh()
{
    pLoader = NULL;
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prMesh::~prMesh()
{
    PRSAFE_DELETE(pLoader);
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
bool prMesh::Load(const char *filename)
{
    PRASSERT(filename && *filename);

    s32  len    = prStringLength(filename);
    bool result = false;

    PRASSERT(len >= 4);
    prTrace("File type: %s\n", &filename[len - 4]);

    // Object files
    if (prStringCompareNoCase(&filename[len - 4], ".obj") == 0)
    {
        pLoader = new prMeshLoader_OBJ();
    }
    else
    {
        prTrace("Unsupported mesh format\n");
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prMesh::Update()
{
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prMesh::Draw()
{
}
