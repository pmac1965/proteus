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
#include "../core/prDefines.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../core/prVertex.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prMesh::prMesh()
{
    mpTexture   = NULL;
    mLoaded     = false;
    mWireframe  = false;
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prMesh::~prMesh()
{
}
