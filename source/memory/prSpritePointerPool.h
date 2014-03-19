// File: prSpritePointerPool.h
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


#ifndef __PRSPRITEPOINTERPOOL_H
#define __PRSPRITEPOINTERPOOL_H


#include "../core/prTypes.h"


// Forward declarations
class prSprite;
class prSpriteManager;


// Defines
#define POOL_MAX_SPRITES    32
#define POOL_MAX_TYPES      32


// Class: prSpritePointerPool
//      Class designed to allow reuseable pools of sprites.
//
// Notes:
//      This class does not release the sprite pointers by default.
//
//      Release() releases all the sprites and resets the pool to its default state.
//
// Warning:
//      Do not use a sprite pointer after calling Release()
class prSpritePointerPool
{
public:
    // Method: prSpritePointerPool
    //      Ctor
    prSpritePointerPool();

    // Method: ~prSpritePointerPool
    //      Dtor
    ~prSpritePointerPool();

    // Method: Add
    //      Adds a pre-created sprite to the pool of reusable sprites.
    //
    // Parameters:
    //      name      - A name for the pool type.
    //      pSprite   - A sprite pointer to store in the pool.
    void Add(const char *name, prSprite* pSprite);

    // Method: Fetch
    //      Fetches a pre-created sprite from the pool of reusable sprites.
    //
    // Parameters:
    //      name      - The name of the pool type.
    //
    // Returns:
    //      A pointer to a sprite, or NULL if the pool name was invalid or the pool is empty.
    prSprite* Fetch(const char *name);

    // Method: Return
    //      Returns a pre-created sprite to the pool of reusable sprites.
    //
    // Parameters:
    //      pSprite   - A sprite pointer to be returned to the pool.
    void Return(prSprite *pSprite);

    // Method: ReturnAll
    //      Returns all sprites to the pool.
    // 
    // Notes:
    //      All active sprites from the pool should be considered invalid after this call.
    void ReturnAll();

    // Method: Count
    //      Gets the number of sprites remaining in a specified pool.
    //
    // Parameters:
    //      name      - The name of the pool type.
    //
    // Returns:
    //      Count or -1 if pool name is invalid
    //
    // Notes:
    //      Please remember this is *NOT* the total number of sprites.
    s32 Count(const char *name) const;

    // Method: Count
    //      Gets the number of sprites remaining in a specified pool.
    //
    // Parameters:
    //      index     - The index of the pool type.
    //
    // Returns:
    //      Count or -1 if pool index is invalid
    //
    // Notes:
    //      Please remember this is *NOT* the total number of sprites.
    s32 Count(s32 index) const;

    // Method: Total
    //      Gets the total number of sprites in all the pools.
    //
    // Notes:
    //      This is regardless of whether they have been fetched or are still in their pool.
    s32 Total() const { return count; }

    // Method: Release
    //      Releases all the sprite pointers and resets the pool back to its default state.
    //
    // Parameters:
    //      spriteManager - A reference to the SpriteManager managing the sprites.
    //
    // Notes:
    //      Any previously contained sprite pointers used after this call will result in undefined behaviour,
    //      which usually means a crash.
    //
    // See Also:
    //      <prSpriteManager>
    void Release(prSpriteManager &spriteManager);


private:

    // Finds the specified index for the types hash
    s32 Find(u32 hash) const;

    // Internal setup.
    void Init();


private:

    prSprite* spriteList [POOL_MAX_TYPES][POOL_MAX_SPRITES];              // The sprite pools
    prSprite* spriteBack [POOL_MAX_TYPES][POOL_MAX_SPRITES];              // Backup of the pool to allow release and return of the pointers.
    s32       spriteCount[POOL_MAX_TYPES];                                // Contains the current count of sprites in each pool
    u32       arrayHashes[POOL_MAX_TYPES];                                // Hashes for each pool type.
    s32       count;                                                      // Total sprites in pool.
};


#endif//__PRSPRITEPOINTERPOOL_H
