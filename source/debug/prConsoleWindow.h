/**
 * @file       prConsoleWindow.h
 * @brief      Contains a class to represent a console window.
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


#ifndef __PRDEBUGCONSOLE_H
#define __PRDEBUGCONSOLE_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <stdio.h>


// Class to represent a console window.
//
class prConsoleWindow
{
public:

    // Creates the console window.
    //
    prConsoleWindow(const TCHAR *name, int width, int height);


    // Destructor.
    //
    ~prConsoleWindow();


    // Displays an exit message and waits for the user to press a key.
    //
    void DisplayCloseMessage();


private:

    // Closes all the consoles input/output handles.
    void CloseHandles();


private:

    HANDLE   scrbuf;
    FILE    *fstdout;
    FILE    *fstdin;
    FILE    *fstderr;
};


#endif//PLATFORM_PC


#endif//__PRDEBUGCONSOLE_H
