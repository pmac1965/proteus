/**
 * prSpritePointerPool.cpp
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
 *
 */


#include "prSpritePointerPool.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../core/prDefines.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../display/prSprite.h"
#include "../display/prSpriteManager.h"


#define MAX_PATH_SIZE   260


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSpritePointerPool::prSpritePointerPool()
{
    Init();
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSpritePointerPool::~prSpritePointerPool()
{
}


/// ---------------------------------------------------------------------------
/// Adds a pre-created sprite to the pool of reusable sprites.
/// ---------------------------------------------------------------------------
void prSpritePointerPool::Add(const char *filename, prSprite* pSprite)
{
    PRASSERT(filename && *filename);
    PRASSERT(pSprite);

    // Create a safe hash
    char buffer[MAX_PATH_SIZE];
    prStringCopySafe(buffer, filename, MAX_PATH_SIZE);
    prStringToLower(buffer);
    u32 hash  = prStringHash(buffer);
    s32 index = Find(hash);

    bool retry;
    do
    {
        retry = false;

        // Found index?
        if (PRBETWEEN(index, 0, POOL_MAX_TYPES - 1))
        {
            // Store in pool
            spriteList [index] [spriteCount[index]] = pSprite;

            // Store backup
            spriteBack [index] [spriteCount[index]] = pSprite;

            // Increment count
            spriteCount[index]++;

            // Add to count
            count++;

            // Clear index.
            pSprite->pool = -1;
        }
        else
        {
            // Nope, then get an index.
            for (s32 i=0; i<POOL_MAX_TYPES; i++)
            {
                // Found? Use retry to add.
                if (arrayHashes[i] == 0xFFFFFFFF)
                {
                    arrayHashes[i] = hash;
                    index          = i;
                    retry          = true;
                    break;
                }
            }

            if (index == -1)
            {
                prTrace("prSpritePointerPool: Failed to add sprite pointer\n");
                return;
            }
        }
    }
    while(retry);
}


/// ---------------------------------------------------------------------------
/// Fetches a pre-created sprite from the pool of reusable sprites.
/// ---------------------------------------------------------------------------
prSprite* prSpritePointerPool::Fetch(const char *filename)
{
    PRASSERT(filename && *filename);

    // Create a safe hash
    char buffer[MAX_PATH_SIZE];
    prStringCopySafe(buffer, filename, MAX_PATH_SIZE);
    prStringToLower(buffer);
    u32 hash  = prStringHash(buffer);
    s32 index = Find(hash);

    prSprite* sprite = NULL;

    if (PRBETWEEN(index, 0, POOL_MAX_TYPES - 1))
    {
        if (spriteCount[index] > 0)
        {
            spriteCount[index]--;
            sprite = spriteList [index] [spriteCount[index]];
            PRASSERT(sprite);
            sprite->pool = index;
        }
    }

    return sprite;
}


/// ---------------------------------------------------------------------------
/// Returns a pre-created sprite to the pool of reusable sprites.
/// ---------------------------------------------------------------------------
void prSpritePointerPool::Return(prSprite *pSprite)
{
    PRASSERT(pSprite);
    if (pSprite)
    {
        s32 index = pSprite->pool;
            
        PRASSERT(PRBETWEEN(index, 0, POOL_MAX_TYPES - 1));
        if (PRBETWEEN(index, 0, POOL_MAX_TYPES - 1))
        {
            spriteList [index] [spriteCount[index]] = pSprite;
            spriteCount[index]++;
            pSprite->pool = -1;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Returns all sprites to the pool.
/// All active sprites from the pool should be considered invalid after this call.
/// ---------------------------------------------------------------------------
void prSpritePointerPool::ReturnAll()
{
    for (int j=0; j<POOL_MAX_TYPES; j++)
    {
        spriteCount[j] = 0;

        for (int k=0; k<POOL_MAX_SPRITES; k++)
        {
            spriteList[j][k] = spriteBack[j][k];

            if (spriteList[j][k] != NULL)
            {
                spriteCount[j]++;
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Gets the number of sprites remaining in a specified pool.
/// ---------------------------------------------------------------------------
s32 prSpritePointerPool::Count(const char *filename) const
{
    PRASSERT(filename && *filename);

    // Create a safe hash
    char buffer[MAX_PATH_SIZE];
    prStringCopySafe(buffer, filename, MAX_PATH_SIZE);
    prStringToLower(buffer);
    u32 hash  = prStringHash(buffer);
    s32 index = Find(hash);
    s32 num   = -1;

    if (PRBETWEEN(index, 0, POOL_MAX_TYPES - 1))
    {
        num = spriteCount[index];
    }

    return num;
}


/// ---------------------------------------------------------------------------
/// Gets the number of sprites remaining in a specified pool.
/// ---------------------------------------------------------------------------
s32 prSpritePointerPool::Count(s32 index) const
{
    s32 num = -1;

    if (PRBETWEEN(index, 0, POOL_MAX_TYPES - 1))
    {
        num = spriteCount[index];
    }

    return num;
}


/// ---------------------------------------------------------------------------
/// Releases all the sprite pointers and resets the pool back to its default state.
/// ---------------------------------------------------------------------------
void prSpritePointerPool::Release(prSpriteManager &spriteManager)
{
    // Release all sprites
    for (int j=0; j<POOL_MAX_TYPES; j++)
    {
        for (int k=0; k<POOL_MAX_SPRITES; k++)
        {
            if (spriteBack[j][k] != NULL)
            {
                spriteManager.Release(spriteBack[j][k]);
                spriteBack[j][k] = NULL;
            }
        }
    }

    Init();
}


/// ---------------------------------------------------------------------------
/// Finds the specified index for the types hash
/// ---------------------------------------------------------------------------
s32 prSpritePointerPool::Find(u32 hash) const
{
    s32 index = -1;
        
    for (int i=0; i<POOL_MAX_TYPES; i++)
    {
        if (arrayHashes[i] == hash)
        {
            index = i;
            break;
        }
    }

    return index;
}


/// ---------------------------------------------------------------------------
/// Internal setup.
/// ---------------------------------------------------------------------------
void prSpritePointerPool::Init()
{
    // Clear the pools
    for (int j=0; j<POOL_MAX_TYPES; j++)
    {
        spriteCount[j] = 0;
        arrayHashes[j] = 0xFFFFFFFF;

        for (int k=0; k<POOL_MAX_SPRITES; k++)
        {
            spriteList[j][k] = NULL;
            spriteBack[j][k] = NULL;
        }
    }

    // Reset totals.
    count = 0;
}
