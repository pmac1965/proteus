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
 *
 */


#include "../prConfig.h"


#if defined(PLATFORM_LINUX)


#include <GL/glx.h>
#include <GL/gl.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
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
static long             eventMask    =  KeyPressMask         |
//                                        ExposureMask         |
                                        ButtonPressMask      |       // Pointer button down events wanted
                                        ButtonReleaseMask    |       // Pointer button up events wanted
                                        PointerMotionMask    |       // Pointer motion
                                        StructureNotifyMask;

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
PRBOOL prLinuxCreateDisplay(u32 width, u32 height)
{
    PRBOOL               result = PRFALSE;
    int                  dummy;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        prTrace("could not open display\n");
        return result;
    }

    // Make sure OpenGL's GLX extension supported
    if (!glXQueryExtension(display, &dummy, &dummy))
    {
        prTrace("X server has no OpenGL GLX extension\n");
        return result;
    }

    // Find an appropriate visual
    vi = glXChooseVisual(display, DefaultScreen(display), doubleBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(display, DefaultScreen(display), singleBuf);
        if (vi == NULL)
        {
            prTrace("no RGB visual with depth buffer\n");
            return result;
        }
        doubleBuffer = GL_FALSE;
    }

    // Test colour mode
    if (vi->class != TrueColor)
    {
        prTrace("TrueColor visual required for this program\n");
        return result;
    }

    // create an OpenGL rendering context
    cx = glXCreateContext(display, vi, None, GL_TRUE);
    if (cx == NULL)
    {
        prTrace("Could not create rendering context\n");
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

    XSetStandardProperties(display, win, "main", "main", None, (char **)argValues, argCount, NULL);

    // Bind the rendering context to the window
    glXMakeCurrent(display, win, cx);

    // request the X window to be displayed on the screen
    XMapWindow(display, win);

    // Okay, made it all the way through
    result = PRTRUE;

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
    XEvent event;

    //do
    if (XCheckWindowEvent(display, win, eventMask, &event))
    {
        //printf("Event %i\n", event.type);
        //prTrace("Event %i\n", event.type);

        //XNextEvent(display, &event);
        switch (event.type)
        {
            case KeyPress:
            {
                KeySym     keysym;
                //XKeyEvent *kevent;
                char       buffer[1];

                // It is necessary to convert the keycode to a keysym before checking if it is an escape
                //kevent = (XKeyEvent *)&event;
                if ((XLookupString((XKeyEvent*)&event, buffer, 1, &keysym,NULL) == 1) && (keysym == (KeySym)XK_Escape))
                {
                    printf("1: Quit app\n");
                    exit(0);
                }
                break;
            }

            // Mouse move
            case MotionNotify:
                prLinuxUpdateMouse(event.xbutton.x,event.xbutton.y, 0);
                    break;

            // Mouse buttons
            case ButtonRelease:
            case ButtonPress:
                prLinuxUpdateMouse(event.xbutton.x,event.xbutton.y, event.xbutton.button);
                    break;

            //case ConfigureNotify:
                //printf("ConfigureNotify - %i, %i\n", event.xconfigure.width, event.xconfigure.height);
                //glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                // fall through
                //break;

            //case Expose:
                //{
                    //static int c=0;
                    //printf("Expose %i\n", c++);
                //needRedraw = GL_TRUE;
                //}
                //break;
                }
    } //while(XPending(display)); /* loop to compress events */
//}

}

#endif
