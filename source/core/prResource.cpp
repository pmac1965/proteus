/**
 * prResource.cpp
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
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prResource::~prResource()
{
}
