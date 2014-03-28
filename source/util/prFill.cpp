#include "prFill.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"
#include <cstring>


#define DEBUG_FILL


namespace
{
    typedef struct Coords
    {
        u32 y;
        u32 x;
        u32 dir;

    } Coords;

    u8     *grid        = NULL;
    void   *pData       = NULL;
    Coords *pCoords     = NULL;
    u32     gridWidth   = 0;
    u32     gridHeight  = 0;
    s32     coord       = 0;
    bool    gridInit    = false;

    #if defined(DEBUG_FILL)
    u8  *dgrid      = NULL;
    #endif

    enum
    {
        F_LEFT,
        F_UP,
        F_DOWN,
        F_RIGHT,
        F_DONE
    };
}


void prFillInit(const u32 width, const u32 height, void *ptr)
{
    PRASSERT(width > 0);
    PRASSERT(height > 0);

    PRSAFE_DELETE_ARRAY(grid);
    PRSAFE_DELETE_ARRAY(pCoords);

    #if defined(DEBUG_FILL)
    PRSAFE_DELETE_ARRAY(dgrid);
    #endif

    grid        = new u8 [height * width];
    pCoords     = new Coords [(height * width) * 4];        // Allow 4 per cell
    pData       = ptr;
    gridWidth   = width;
    gridHeight  = height;
    coord       = 0;

    #if defined(DEBUG_FILL)
    dgrid       = new u8 [height * width];
    memset(dgrid, 0, sizeof(u8) * (height * width));
    #endif

    memset(grid, 0, sizeof(u8) * (height * width));
    gridInit = true;
}


void prFillStart(u32 y, u32 x, s32 value, prFillCallback cb)
{
    PRASSERT(y < gridHeight);
    PRASSERT(x < (gridWidth * gridHeight));
    PRASSERT(gridInit);
    PRASSERT(cb);

    for(;;)
    {
        // If match?
        if (cb(y, x, value, pData))
        {
            prTrace("Success at %i, %i\n", y, x);

            pCoords[coord].x = x;
            pCoords[coord].y = y;
            coord++;

            #if defined(DEBUG_FILL)
            dgrid[(y * gridWidth) + x] = 1;
            #endif

            u8 data = grid[(y * gridWidth) + x];

            // Set new direction
            if (data == F_LEFT)
            {
                if (x > 0)
                {
                    //prTrace("Left\n");
                    grid[(y * gridWidth) + x] ++;
                    x--;
                }
                else
                {
                    //prTrace("Dir next\n");
                    grid[(y * gridWidth) + x] ++;
                    coord--;
                }
            }
            else if (data == F_UP)
            {
                if (y > 0)
                {
                    //prTrace("Up\n");
                    grid[(y * gridWidth) + x] ++;
                    y--;
                }
                else
                {
                    //prTrace("Dir next\n");
                    grid[(y * gridWidth) + x] ++;
                    coord--;
                }
            }
            else if (data == F_DOWN)
            {
                if (y < (gridHeight - 1))
                {
                    //prTrace("Down\n");
                    grid[(y * gridWidth) + x] ++;
                    y++;
                }
                else
                {
                    //prTrace("Dir next\n");
                    grid[(y * gridWidth) + x] ++;
                    coord--;
                }
            }
            else if (data == F_RIGHT)
            {
                if (x < (gridWidth - 1))
                {
                    //prTrace("Right\n");
                    grid[(y * gridWidth) + x] ++;
                    x++;
                }
                else
                {
                    //prTrace("Dir next\n");
                    grid[(y * gridWidth) + x] ++;
                    coord--;
                }
            }
            else
            {
                prTrace("Done 2\n");
                break;
            }
        }
        else
        {
            if (--coord > -1)
            {
                x = pCoords[coord].x;
                y = pCoords[coord].y;
            }
            else
            {
                prTrace("Done 1\n");
            }
        }
    }

#if defined(DEBUG_FILL)
    prTraceRemoveDuplicates(false);
    for (u32 yy = 0; yy < gridHeight; yy++)
    {
        for (u32 xx = 0; xx < gridWidth; xx++)
        {
            prTrace("%i", dgrid[(yy * gridWidth) + xx]);
        }
        prTrace("\n");
    }
    prTrace("\n");
    prTraceRemoveDuplicates(true);
#endif
}
