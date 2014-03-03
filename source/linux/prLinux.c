/**
 * prWindow_Linux.cpp
 */


#include "../prConfig.h"


#if defined(PLATFORM_LINUX)


#include <GL/glx.h>
#include <GL/gl.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include "../core/prTypes.h"
#include "../core/prDefines.h"
#include "../debug/prTrace.h"


// Locals
static int	            argCount     = 0;
static const char     **argValues    = NULL;
static Display         *display      = NULL;
static XVisualInfo     *vi           = NULL;
static GLboolean        doubleBuffer = GL_TRUE;
static Window           win;


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
    swa.event_mask   = KeyPressMask    |
                       ExposureMask    |
                       ButtonPressMask |
                       StructureNotifyMask;

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

    do
    {
        XNextEvent(display, &event);
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
                    exit(0);
                }
                break;
            }

            case ButtonPress:
                //recalcModelView = GL_TRUE;
                switch (event.xbutton.button)
                {
                case 1:// xAngle += 10.0;
                    break;
                case 2: //yAngle += 10.0;
                    break;
                case 3: //zAngle += 10.0;
                    break;
                }
            break;//*/

            case ConfigureNotify:
                prTrace("ConfigureNotify - %i, %i\n", event.xconfigure.width, event.xconfigure.height);
                glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                // fall through
                break;

            case Expose:
                {
                    static int c=0;
                    prTrace("Expose %i\n", c++);
                //needRedraw = GL_TRUE;
                }
                break;
        }

    } while(XPending(display)); /* loop to compress events */
//}

}

#endif
