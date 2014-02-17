/**
 * prOnScreenLogger.cpp
 */


#include "prOnScreenLogger.h"
#include "prDebugFont.h"
#include "prTrace.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prResourceManager.h"
#include "../core/prRegistry.h"
#include "../display/prTexture.h"
#include "../debug/prDebug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prOnScreenLogger::prOnScreenLogger() : prCoreSystem(PRSYSTEM_ONSCREENLOGGER, "prOnScreenLogger")
{
    m_pTexture        = NULL;
    m_pFixedWidthFont = NULL;
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prOnScreenLogger::~prOnScreenLogger()
{
    // Delete the font
    PRSAFE_DELETE(m_pFixedWidthFont);

    // Release the fonts texture
    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
    if (pRM && m_pTexture)
    {
        pRM->Unload(m_pTexture);
        m_pTexture = NULL;
    }

    // And clear the strings
    Clear();
}


/// ----------------------------------------------------------------------------
/// Adds a message.
/// ----------------------------------------------------------------------------
void prOnScreenLogger::Add(const char *message, ...)
{
    if (message && *message)
    {
        char buffer[512];

		// Format the message.
        va_list args;
        va_start(args, message);        
        vsprintf(buffer, message, args);
        va_end(args);
        TODO("Make safer");

        // Store
        m_messages.push_front(strdup(buffer));

        // Release old
        if (m_messages.size() >= MAX_MESSAGE_COUNT)
        {
            char *p = m_messages.back();
            PRSAFE_DELETE(p);
            m_messages.pop_back();
        }
    }
}


/// ----------------------------------------------------------------------------
/// Draw all the messages.
/// ----------------------------------------------------------------------------
void prOnScreenLogger::Draw(u32 xpos, u32 ypos, prFixedWidthFont::prFixedWidthAlignment alignment)
{
    if (m_pFixedWidthFont == NULL)
    {
        // Create a texture
        prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
        PRASSERT(pRM)

        m_pTexture = pRM->LoadFromMemory<prTexture>("debugfont", debugFont, sizeof(debugFont));
        PRASSERT(m_pTexture);

        // Create the font
        m_pFixedWidthFont = new prFixedWidthFont(m_pTexture, 16, 16, 10);
        PRASSERT(m_pFixedWidthFont)
    }


    if (m_pFixedWidthFont)
    {
        // Set alignment
        switch (alignment)
        {
        case prFixedWidthFont::FW_ALIGN_LEFT:
        case prFixedWidthFont::FW_ALIGN_RIGHT:
        case prFixedWidthFont::FW_ALIGN_CENTER:
            m_pFixedWidthFont->SetAlignment(alignment);
            break;

        default:
            prTrace("prOnScreenLogger - Invalid alignment");
            m_pFixedWidthFont->SetAlignment(prFixedWidthFont::FW_ALIGN_LEFT);
            break;
        }

        // Get screen height
        prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        PRASSERT(pReg)        
        u32 scrnHeight = (u32)atoi(pReg->GetValue("ScreenHeight"));

        // Draw.
        std::list<char *>::iterator itr = m_messages.begin();
        std::list<char *>::iterator end = m_messages.end();
        for (;itr != end; ++itr)
        {
            m_pFixedWidthFont->Draw((f32)xpos, (f32)ypos, (*itr));            
            ypos += m_pFixedWidthFont->GetFrameHeight();

            // Early exit?
            if (ypos >= scrnHeight)
                break;
        }
    }
}


/// ----------------------------------------------------------------------------
/// Remove all the messages.
/// ----------------------------------------------------------------------------
void prOnScreenLogger::Clear()
{
    while (!m_messages.empty())
    {
        char *p = m_messages.back();
        PRSAFE_DELETE(p);
        m_messages.pop_back();
    }
}


/// ----------------------------------------------------------------------------
/// Returns the number of messages.
/// ----------------------------------------------------------------------------
s32 prOnScreenLogger::Count()
{
    return m_messages.size();
}