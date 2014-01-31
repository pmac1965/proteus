/**
 * @file       prRenderer.h
 * @brief      Contains the renderer base class
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


#ifndef __PRRENDERER_H
#define __PRRENDERER_H


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations
class  prWindow;
class  prColour;
class  prVector3;
struct prVertex2D;


/// @brief      Renderer base classs
/// @n          All renderers will implement the basic core functionality
class prRenderer : public prCoreSystem
{
public:

    /// @brief      Ctor
    prRenderer();

    /// @brief      Dtor
    virtual ~prRenderer();

    /// @brief      Sets the window that the renderer draws to.
    /// @param      window - A pointer to a window class
    void SetWindow(prWindow *window);

    /// @brief      Gets the window thats drawn to.
    /// @return     A pointer to a window class, or null if not set
    prWindow *GetWindow() { return m_pWindow; }

    /// @brief      Inits the renderer with basic settings.
    /// @note       Pure virtual
    virtual void Init() = 0;

    /// @brief      Destroys the renderer.
    /// @note       Pure virtual
    virtual void Destroy() = 0;

    /// @brief      Begins the image rendering cycle.
    /// @note       Pure virtual
    virtual void Begin() = 0;

    /// @brief      Ends the image rendering cycle.
    /// @note       Pure virtual
    virtual void End() = 0;

    /// @brief      Shows the previously rendered image.
    /// @note       Pure virtual
    virtual void Present() = 0;

    /// @brief      Set orthographic view in preparation for 2D rendering.
    /// @note       Pure virtual
    virtual void SetOrthographicView() = 0;

    /// @brief      Restore perspective projection.
    /// @note       Pure virtual
    virtual void RestorePerspectiveView() = 0;
    
    /// @brief      Draws a single point.
    /// @note       Pure virtual
    /// @param      x - The x coordinate
    /// @param      y - The y coordinate
    virtual void DrawPoint(f32 x, f32 y) = 0;
    
    /// @brief      Draws a line.
    /// @note       Pure virtual
    /// @param      x1 - The start x coordinate
    /// @param      y1 - The start y coordinate
    /// @param      x2 - The end x coordinate
    /// @param      y2 - The end y coordinate
    virtual void DrawLine(f32 x1, f32 y1, f32 x2, f32 y2) = 0;

    /// @brief      Draws a line 3D.
    /// @note       Pure virtual
    /// @param      from - The start coordinate
    /// @param      to   - The end coordinate
    virtual void DrawLine(prVector3 &from, prVector3 &to) = 0;
    
    /// @brief      Draws a hollow rectangle.
    /// @note       Pure virtual
    /// @param      x1 - The start x coordinate (Top left corner)
    /// @param      y1 - The start y coordinate (Top left corner)
    /// @param      x2 - The end x coordinate (Bottom right corner)
    /// @param      y2 - The end y coordinate (Bottom right corner)
    virtual void DrawRect(f32 x1, f32 y1, f32 x2, f32 y2) = 0;
    
    /// @brief      Draws a filled rectangle.
    /// @note       Pure virtual
    /// @param      x1 - The start x coordinate (Top left corner)
    /// @param      y1 - The start y coordinate (Top left corner)
    /// @param      x2 - The end x coordinate (Bottom right corner)
    /// @param      y2 - The end y coordinate (Bottom right corner)
    virtual void DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2) = 0;
    
    /// @brief      Draws a hollow circle.
    /// @note       Pure virtual
    /// @param      x      - The start x coordinate (center)
    /// @param      y      - The start y coordinate (center)
    /// @param      radius - The radius
    virtual void DrawCircle(f32 x, f32 y, f32 radius) = 0;

    /// @brief      Draws a filled circle.
    /// @note       Pure virtual
    /// @param      x      - The start x coordinate (center)
    /// @param      y      - The start y coordinate (center)
    /// @param      radius - The radius
    virtual void DrawFilledCircle(f32 x, f32 y, f32 radius) = 0;

    /// @brief      Draws a hollow polygon.
    /// @note       Pure virtual
    /// @param      vertices - A pointer to the vertex data
    /// @param      count    - Number of vertices
    virtual void DrawPolygon(prVertex2D *vertices, s32 count) = 0;

    /// @brief      Draws a filled polygon.
    /// @note       Pure virtual
    /// @param      vertices - A pointer to the vertex data
    /// @param      count    - Number of vertices
    virtual void DrawFilledPolygon(prVertex2D *vertices, s32 count) = 0;
    
    /// @brief      Draws a textured quad
    /// @note       Pure virtual
    virtual void DrawQuad() = 0;
    
    /// @brief      Draws a textured quad
    /// @note       Pure virtual
    /// @param      u0 - Vertex coordinate
    /// @param      v0 - Vertex coordinate
    /// @param      u1 - Vertex coordinate
    /// @param      v1 - Vertex coordinate
    virtual void DrawQuad(float u0, float v0, float u1, float v1) = 0;

    /// @brief      Draws a textured quad
    /// @note       Pure virtual
    /// @param      u0 - Vertex coordinate
    /// @param      v0 - Vertex coordinate
    /// @param      u1 - Vertex coordinate
    /// @param      v1 - Vertex coordinate
    /// @param      c  - Colour
    virtual void DrawQuad(float u0, float v0, float u1, float v1, prColour c) = 0;

    /// @brief      Draws a textured quad
    /// @note       Pure virtual
    /// @param      u0 - Vertex coordinate
    /// @param      v0 - Vertex coordinate
    /// @param      u1 - Vertex coordinate
    /// @param      v1 - Vertex coordinate
    /// @param      c  - Colour
    virtual void BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c) = 0;

    /// @brief      Sets the draw colour.
    /// @note       Pure virtual
    /// @param      colour - Colour
    virtual void SetColour(const prColour &colour) = 0;

    /// @brief      Sets the clear colour.
    /// @note       Pure virtual
    /// @param      colour - Colour
    virtual void SetClearColour(const prColour &colour) = 0;

    /// @brief      Enables/disables textures.
    /// @note       Pure virtual
    /// @param      state - true or false
    virtual void TexturesEnabled(bool state) = 0;

protected:

    prWindow    *m_pWindow;
};


#endif//__PRRENDERER_H
