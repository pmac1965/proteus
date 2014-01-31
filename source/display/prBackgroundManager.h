/**
 * @file       prBackgroundManager.h
 * @brief      Contains the 2D background manager class.
 * @note       This class is used to create and destroy backgrounds.
 * @n          You must use this class to create backgrounds, as they
 * @n          cannot be created without it.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRBACKGROUNDMANAGER_H
#define __PRBACKGROUNDMANAGER_H


#include "../core/prCoreSystem.h"


// Forward declarations.
class prBackground;


#define MAX_BACKGROUNDS     8


/// @brief      Background management class
/// @note       This class is used to create and destroy backgrounds.
/// @n          You must use this class to create backgrounds, as they
/// @n          cannot be created without it.
class prBackgroundManager : public prCoreSystem
{
public:

    /// @brief      Constructs the background manager.
    prBackgroundManager();

    /// @brief      Destructor.
    ~prBackgroundManager();

    /// @brief      Creates a background.
    prBackground *Create(const char *filename);

    /// @brief      Releases a background and any asssociated assets.
    void Release(prBackground *bg);
    
    /// @brief      Releases all backgrounds and their asssociated assets. 
    void ReleaseAll();

    /// @brief      Shows all the assets being used by the background manager.
    void DisplayUsage();


private:

    // Stops passing by value and assignment.
    prBackgroundManager(const prBackgroundManager&);
    const prBackgroundManager& operator = (const prBackgroundManager&);


private:

    prBackground   *backgrounds[MAX_BACKGROUNDS];
};


#endif//__PRBACKGROUNDMANAGER_H
