// ----------------------------------------------------------------------------
//
// File: prOnScreenLogger.h
//
//      Description     - Contains a helper class for platforms without easy
//                        access to IDE debug logging.
//      Author          - Paul Michael McNab.
//      Copyright       - Copyright Paul Michael McNab. All rights reserved.
//
// Disclaimer:
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//      TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//      PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//      CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//      EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//      PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//      PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//      LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//      NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ----------------------------------------------------------------------------


#ifndef __PRONSCREENLOGGER_H
#define __PRONSCREENLOGGER_H


#include <string>
#include <list>
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


//#include "../core/singleton.h"


// Forward declarations
class prFixedWidthFont;


// ----------------------------------------------------------------------------
// Simple on screen logging class.
// Useful when the debugger isn't very good.
// ----------------------------------------------------------------------------
//#if defined(PLATFORM_ANDROID)
//class OnScreenLogger
//#else
class prOnScreenLogger  : public prCoreSystem//: public Singleton<OnScreenLogger>
//#endif
{
public:

    prOnScreenLogger();
    ~prOnScreenLogger();


    // Adds a message.
    void Add(const char *message, ...);

    // Draw all the messages.
    void Draw(prFixedWidthFont *font, u32 xpos, u32 yOffset, bool right = false);

    // Remove all the messages.
    void Clear();

    //#if defined(PLATFORM_ANDROID)
    //static OnScreenLogger *GetInstance();
    //static void SingletonCreate();
    //static void SingletonDestroy();
    //static bool SingletonExists();
    //#endif

private:

    std::list<std::string>  m_messages;

//private:
    
    //MAKE_SINGLETON(OnScreenLogger)
};


#endif//__PRONSCREENLOGGER_H
