// ----------------------------------------------------------------------------
//
// File: prOnScreenLogger.cpp
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


#include "prOnScreenLogger.h"
#include "../core/prCore.h"
#include "../display/prFixedWidthFont.h"
#include <stdarg.h>
#include <stdio.h>


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prOnScreenLogger::prOnScreenLogger() : prCoreSystem(PRSYSTEM_ONSCREENLOGGER, "prOnScreenLogger")
{
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prOnScreenLogger::~prOnScreenLogger()
{
}


// ----------------------------------------------------------------------------
// Adds a message.
// ----------------------------------------------------------------------------
void prOnScreenLogger::Add(const char *message, ...)
{
    if (message && *message)
    {
        char buffer[256];

		// Format the message.
        va_list args;
        va_start(args, message);        
        vsprintf(buffer, message, args);
        va_end(args);

        m_messages.push_front(std::string(buffer));

        if (m_messages.size() > 64)
        {
            m_messages.pop_back();
        }
    }
}


// ----------------------------------------------------------------------------
// Draw all the messages.
// ----------------------------------------------------------------------------
void prOnScreenLogger::Draw(prFixedWidthFont *font, u32 xpos, u32 yOffset, bool right)
{
    if (font)
    {
        std::list<std::string>::iterator itr = m_messages.begin();
        std::list<std::string>::iterator end = m_messages.end();

        f32 ypos = 0;

        // Right aligned?
        if (right)
        {
            font->SetAlignment(prFixedWidthFont::FW_ALIGN_RIGHT);
        }

        // Draw.
        for (;itr != end;++itr)
        {
            font->Draw((f32)xpos, ypos, (*itr).c_str());            
            ypos += yOffset;
        }

        // Restore alignment.
        font->SetAlignment(prFixedWidthFont::FW_ALIGN_LEFT);
    }
}


// ----------------------------------------------------------------------------
// Remove all the messages.
// ----------------------------------------------------------------------------
void prOnScreenLogger::Clear()
{
    m_messages.clear();
}

//
//#if defined(PLATFORM_ANDROID)
//namespace
//{
//    prOnScreenLogger *instance = NULL;
//}
//
//prOnScreenLogger *prOnScreenLogger::GetInstance()
//{
//    if (instance == NULL)
//    {
//        instance = new prOnScreenLogger();
//    }
//
//    return instance;
//}
//
//void prOnScreenLogger::SingletonCreate()
//{
//    if (instance == NULL)
//    {
//        instance = new prOnScreenLogger();
//    }
//}
//
//void prOnScreenLogger::SingletonDestroy()
//{
//    SAFE_DELETE(instance);
//}
//
//bool prOnScreenLogger::SingletonExists()
//{
//    return (instance != NULL);
//}
//#endif
