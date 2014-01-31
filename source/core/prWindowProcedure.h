/**
 * @file       prWindowProcedure.h
 * @brief      Contains the default windows message handler.
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


#ifndef __PRWINDOWPROCEDURE_H
#define __PRWINDOWPROCEDURE_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


// ----------------------------------------------------------------------------
// Exclude MFC
// ----------------------------------------------------------------------------
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WIN32_EXTRA_LEAN
#define WIN32_EXTRA_LEAN
#endif


#include <windows.h>


// ----------------------------------------------------------------------------
//  Forward declarations
// ----------------------------------------------------------------------------
class prApplication;


/// @brief      Pointer to allow game to receive specific messages like Activate and Deactivate.
/// @param      app - A pointer to the current application
void prSetApplicationForWindowProcedure(prApplication *app);


/// @brief      The default window message handler.
/// @param      hwnd    - Handle to the application window
/// @param      msg     - A windows message
/// @param      wParam  - Message data
/// @param      lParam  - Message data
/// @return     Returns zero if a message is handled, otherwise the function returns the value returned
/// @return     by the default window procedure
LRESULT CALLBACK prWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif//PLATFORM_PC


#endif//__PRWINDOWPROCEDURE_H
