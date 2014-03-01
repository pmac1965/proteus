/**
 * prApplication_Linux.cpp
 */


#include "../prConfig.h"


#if defined(PLATFORM_LINUX)

 
#include "prApplication_Linux.h"
#include "prWindow_Linux.h"
#include "prWindowProcedure.h"
#include "prCore.h"
#include "prRegistry.h"
#include "prVersion.h"
#include "prMacros.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../debug/prConsoleWindow.h"
#include "../debug/prOnScreenLogger.h"
#include "../input/prMouse.h"
#include "../display/prRenderer.h"
#include "../core/prStringUtil.h"
#include "../linux/prLinux.h"


// ----------------------------------------------------------------------------
// Static data
// ----------------------------------------------------------------------------
static const char *embedded[] = 
{
    "The Proteus Game Engine. ",
    "Author Paul Michael McNab. ",
    "Copyright Paul Michael McNab. All rights reserved.",
};


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prApplication_Linux::prApplication_Linux() : prApplication()
{
#if defined(_DEBUG) || defined(DEBUG)
    // Write startup info.
    prRegistry *reg = (prRegistry *)prCoreGetComponent(PRSYSTEM_REGISTRY);
    if (reg)
    {
        if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
        {
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Engine version : %s\n", prGetVersionAsString());
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Components:\n");
            prTrace("Lua            : 5.2.3 - Copyright (C) 1994-2013 Lua.org, PUC-Rio.\n");
            prTrace("Box2D          : 2.3.0 - Copyright (c) 2006-2013 Erin Catto\n");
            prTrace("TinyXML        : 2.6.2 - Copyright (c) 2000-2006 Lee Thomason.\n");
            prTrace("Zlib           : 1.2.8 - Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler.\n");
            prTrace("Ogg            : 1.2.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
            prTrace("Vorbis         : 1.3.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
            prTrace("AntTweakBar    : 1.16  - Copyright (C) 2005-2013 Philippe Decaudin.\n");        
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Config         : %s - Linux - Debug\n", BuildType());
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Options        : Verbose      %s\n", reg->GetValue("Verbose"));
            prTrace("               : Use archives %s\n", reg->GetValue("UseArchives"));
            prTrace("               : Log to file  %s\n", reg->GetValue("LogToFile"));
            prTrace("               : Help         %s\n", reg->GetValue("Help"));
            prTrace("-------------------------------------------------------------------------------\n");
        }
    }
#endif


    //prTrace("-------------------------------------------------------------------------------\n");
    //prTrace("Build number %0.4f\n", BUILD_NUMBER);
    //prTrace("-------------------------------------------------------------------------------\n");


    // Access the embedded data, so it'll stay linked into the game.
    for (u32 j=0; j<PRARRAY_SIZE(embedded); j++)
    {
        const char *text = embedded[j];

        // Just compare the first 10 characters.
        for (int k=0; k<10; k++)
        {
            if (text[k] == 0)
            {
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prApplication_Linux::~prApplication_Linux()
{
}


/// ---------------------------------------------------------------------------
/// Creates the application display.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_Linux::DisplayCreate(u32 width, u32 height, const char *pWindowName)
{
    PRBOOL result = prCreateLinuxDisplay(width, height);
    if (result)
    {
        m_running = PRTRUE;
        result    = PRTRUE;

        // Set window.
        prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
        if (pRenderer)
        {
            //pRenderer->SetWindow(m_pWindow);
            //pRenderer->Init();
        }

        // Set registry
        prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        if (reg)
        {
            // Set title here
            static_cast<prWindow_Linux *>(m_pWindow)->SetTitle(pWindowName);

            reg->SetValue("WindowName", pWindowName);
            reg->SetValue("ScreenWidth", width);
            reg->SetValue("ScreenHeight", height);

            // Show startup entries
            prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (reg)
            {
                if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
                {
                   reg->ShowKeyValuePairs();
                }
            }
        }
    }
    else
    {
        // Exit app.
        m_running = PRFALSE;
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Run the game.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_Linux::Run()
{
	  while (1)
	  {
		  prLinuxLoop();

/*	    if (recalcModelView)
	    {
	      glMatrixMode(GL_MODELVIEW);

	      // reset modelview matrix to the identity matrix
	      glLoadIdentity();

	      // move the camera back three units
	      glTranslatef(0.0, 0.0, -3.0);

	      // rotate by X, Y, and Z angles
	      glRotatef(xAngle, 0.1, 0.0, 0.0);
	      glRotatef(yAngle, 0.0, 0.1, 0.0);
	      glRotatef(zAngle, 0.0, 0.0, 1.0);

	      recalcModelView = GL_FALSE;
	      needRedraw = GL_TRUE;
	    }
	    if (needRedraw)
	    {
	      redraw();
	      needRedraw = GL_FALSE;
	    }//*/
	  }

    return PRFALSE;
}


/// ---------------------------------------------------------------------------
/// Returns the build
/// ---------------------------------------------------------------------------
const char *prApplication_Linux::BuildType()
{
    #if defined(PROTEUS_TOOL)
        return "Tool";
    #else
        return "Game";
    #endif
}


#endif//PLATFORM_PC
