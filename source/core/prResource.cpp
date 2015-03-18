/**
 * prResource.cpp
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


#include "prResource.h"
#include "prStringUtil.h"
#include "../debug/prAssert.h"
#include <string.h>


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prResource::prResource(const char *filename)
{
    PRASSERT(filename && *filename);
    PRASSERT(strlen(filename) < RESOURCE_FILENAME_SIZE);

    // Save filename.
    prStringCopySafe(m_filename, filename, sizeof(m_filename));
    prStringToLower(m_filename);
    prStringReplaceChar(m_filename, '\\', '/');

    // Hash name for quick searching.
    m_hash = prStringHash(m_filename);

    // Initial reference count.
    m_references = 1;
    m_locked     = false;
    m_size       = 0;
    m_exp0		 = false;
    m_exp1		 = false;
    m_exp2		 = false;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prResource::~prResource()
{
}
