/**
 * prWindow_Linux.c
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
 */


#include "../prConfig.h"


#if defined(PLATFORM_LINUX)


#include <GL/glx.h>
#include <GL/gl.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../core/prTypes.h"
#include "../core/prDefines.h"
#include "../debug/prTrace.h"
#include "prLinuxInput.h"


// Locals
static int              argCount     = 0;
static const char     **argValues    = NULL;
static Display         *display      = NULL;
static XVisualInfo     *vi           = NULL;
static GLboolean        doubleBuffer = GL_TRUE;
static Window           win;
static long             eventMask    =  KeyPressMask         |		// Keys
                                        KeyReleaseMask       |		// Keys
                                        ButtonPressMask      |      // Pointer button down events wanted
                                        ButtonReleaseMask    |      // Pointer button up events wanted
                                        PointerMotionMask    |      // Pointer motion
                                        StructureNotifyMask;		// Events like destroy

// Local data
static int singleBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int doubleBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};


/// ---------------------------------------------------------------------------
/// Stores the args
/// ---------------------------------------------------------------------------
void prLinuxStoreArgs(int argc, const char *args[])
{
    argCount  = argc;
    argValues = args;
}


/// ---------------------------------------------------------------------------
/// Creates a linux window
/// ------------------------------ ---------------------------------------------
int prLinuxCreateDisplay(unsigned int width, unsigned int height)
{
    int                  result = 0;
    int                  dummy;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;
    XSizeHints			 sizeHints;

    // Set hints
    memset(&sizeHints, 0, sizeof(sizeHints));
    sizeHints.min_width  = width;
    sizeHints.max_width  = width;
    sizeHints.min_height = height;
    sizeHints.max_height = height;
    sizeHints.flags      = PMinSize | PMaxSize;

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        prTrace(LogError, "could not open display\n");
        return result;
    }

    // Make sure OpenGL's GLX extension supported
    if (!glXQueryExtension(display, &dummy, &dummy))
    {
        prTrace(LogError, "X server has no OpenGL GLX extension\n");
        return result;
    }

    // Find an appropriate visual
    vi = glXChooseVisual(display, DefaultScreen(display), doubleBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(display, DefaultScreen(display), singleBuf);
        if (vi == NULL)
        {
            prTrace(LogError, "no RGB visual with depth buffer\n");
            return result;
        }
        doubleBuffer = GL_FALSE;
    }

    // Test colour mode
    if (vi->class != TrueColor)
    {
        prTrace(LogError, "TrueColor visual required for this program\n");
        return result;
    }

    // create an OpenGL rendering context
    cx = glXCreateContext(display, vi, None, GL_TRUE);
    if (cx == NULL)
    {
        prTrace(LogError, "Could not create rendering context\n");
        return result;
    }

    // Create an X window with the selected visual
    cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    swa.colormap     = cmap;
    swa.border_pixel = 0;
    swa.event_mask   = eventMask;

    win = XCreateWindow(display,
                        RootWindow(display, vi->screen),
                        0,
                        0,
                        width,
                        height,
                        0,
                        vi->depth,
                        InputOutput,
                        vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask,
                        &swa);

    XSetStandardProperties(display, win, "main", "main", None, (char **)argValues, argCount, &sizeHints);

    // Bind the rendering context to the window
    glXMakeCurrent(display, win, cx);

    // request the X window to be displayed on the screen
    XMapWindow(display, win);

    // Okay, made it all the way through
    result = 1;

    return result;
}


/// ---------------------------------------------------------------------------
/// Displays the last buffer drawn
/// ---------------------------------------------------------------------------
void prLinuxSwapBuffers()
{
    if (doubleBuffer)
    {
        // buffer swap does implicit glFlush
        glXSwapBuffers(display, win);
    }
    else
    {
        // explicit flush for single buffered case
        glFlush();
    }
}


/// ---------------------------------------------------------------------------
/// The main linux loop
/// ---------------------------------------------------------------------------
void prLinuxLoop()
{
    // http://stackoverflow.com/questions/125806/capturing-input-in-linux
    XEvent event;
    while (XCheckWindowEvent(display, win, eventMask, &event))
    {
        //XNextEvent(display, &event);
        switch (event.type)
        {
            case KeyPress:
            	{
            		KeySym     keysym;
            		char       buffer[1];

            		// It is necessary to convert the keycode to a keysym before checking if it is an escape
            		//if ((XLookupString((XKeyEvent*)&event, buffer, 1, &keysym,NULL) == 1) && (keysym == (KeySym)XK_Escape))
            		//{
            		//    printf("1: Quit app\n");
            		//    exit(0);
            		//}
            		XLookupString((XKeyEvent*)&event, buffer, 1, &keysym, NULL);

  //          		char c = buffer[0];
  //          		printf("Press: %c - %i\n", c, c);

            		prLinuxUpdateKeyboard((unsigned int)buffer[0], TRUE);
            	}
				break;

            case KeyRelease:
/*				{
            		KeySym     keysym;
            		char       buffer[1];

            		XLookupString((XKeyEvent*)&event, buffer, 1, &keysym, NULL);

            		char c = buffer[0];
            		printf("Release: %c - %i\n", c, c);

            		prLinuxUpdateKeyboard((unsigned int)buffer[0], FALSE);
				}//*/
            	break;

            // Mouse move
            case MotionNotify:
                prLinuxUpdateMouse(event.xbutton.x,event.xbutton.y, 0, 0);
                break;

            // Mouse buttons
            case ButtonRelease:
                prLinuxUpdateMouse(event.xbutton.x,event.xbutton.y, event.xbutton.button, 0);
                break;

            case ButtonPress:
                prLinuxUpdateMouse(event.xbutton.x,event.xbutton.y, event.xbutton.button, 1);
                break;
        }
    }
}

#else

/// ---------------------------------------------------------------------------
/// Just to stop the translation unit being considered empty!
/// ---------------------------------------------------------------------------
static void Filler()
{
}

#endif
