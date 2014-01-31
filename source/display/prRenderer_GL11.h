/**
 * @file       prRenderer_GL11.h
 * @brief      Contains a renderer for OpenGL 1.1
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


#ifndef __PRRENDERER_GL11_H
#define __PRRENDERER_GL11_H


#include "prRenderer.h"


/// @brief      Renderer for OpenGL 1.1
class prRenderer_GL11 : public prRenderer
{
public:

    /// @brief      Ctor
    prRenderer_GL11();

    /// @brief      Dtor
    ~prRenderer_GL11();

    /// @brief      Inits the renderer with basic settings.
    /// @note       Overrides pure virtual
    void Init();

    /// @brief      Destroys the renderer.
    /// @note       Overrides pure virtual
    void Destroy();

    /// @brief      Begins the image rendering cycle.
    /// @note       Overrides pure virtual
    void Begin();

    /// @brief      Ends the image rendering cycle.
    /// @note       Overrides pure virtual
    void End();

    /// @brief      Shows the previously rendered image.
    /// @note       Overrides pure virtual
    void Present();

    /// @brief      Set orthographic view in preparation for 2D rendering.
    /// @note       Overrides pure virtual
    void SetOrthographicView();

    /// @brief      Restore perspective projection.
    /// @note       Overrides pure virtual
    void RestorePerspectiveView();
    
    /// @brief      Draws a single point.
    /// @note       Overrides pure virtual
    /// @param      x - The x coordinate
    /// @param      y - The y coordinate
    void DrawPoint(f32 x, f32 y);
    
    /// @brief      Draws a line.
    /// @note       Overrides pure virtual
    /// @param      x1 - The start x coordinate
    /// @param      y1 - The start y coordinate
    /// @param      x2 - The end x coordinate
    /// @param      y2 - The end y coordinate
    void DrawLine(f32 x1, f32 y1, f32 x2, f32 y2);

    /// @brief      Draws a line 3D.
    /// @note       Overrides pure virtual
    /// @param      from - The start coordinate
    /// @param      to   - The end coordinate
    void DrawLine(prVector3 &from, prVector3 &to);
    
    /// @brief      Draws a hollow rectangle.
    /// @note       Overrides pure virtual
    /// @param      x1 - The start x coordinate (Top left corner)
    /// @param      y1 - The start y coordinate (Top left corner)
    /// @param      x2 - The end x coordinate (Bottom right corner)
    /// @param      y2 - The end y coordinate (Bottom right corner)
    void DrawRect(f32 x1, f32 y1, f32 x2, f32 y2);
    
    /// @brief      Draws a filled rectangle.
    /// @note       Overrides pure virtual
    /// @param      x1 - The start x coordinate (Top left corner)
    /// @param      y1 - The start y coordinate (Top left corner)
    /// @param      x2 - The end x coordinate (Bottom right corner)
    /// @param      y2 - The end y coordinate (Bottom right corner)
    void DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2);
    
    /// @brief      Draws a hollow circle.
    /// @note       Overrides pure virtual
    /// @param      x      - The start x coordinate (center)
    /// @param      y      - The start y coordinate (center)
    /// @param      radius - The radius
    void DrawCircle(f32 x, f32 y, f32 radius);

    /// @brief      Draws a filled circle.
    /// @note       Overrides pure virtual
    /// @param      x      - The start x coordinate (center)
    /// @param      y      - The start y coordinate (center)
    /// @param      radius - The radius
    void DrawFilledCircle(f32 x, f32 y, f32 radius);

    /// @brief      Draws a hollow polygon.
    /// @note       Overrides pure virtual
    /// @param      vertices - A pointer to the vertex data
    /// @param      count    - Number of vertices
    void DrawPolygon(prVertex2D *vertices, s32 count);

    /// @brief      Draws a filled polygon.
    /// @note       Overrides pure virtual
    /// @param      vertices - A pointer to the vertex data
    /// @param      count    - Number of vertices
    void DrawFilledPolygon(prVertex2D *vertices, s32 count);
    
    /// @brief      Draws a textured quad
    /// @note       Overrides pure virtual
    void DrawQuad();
    
    /// @brief      Draws a textured quad
    /// @note       Overrides pure virtual
    /// @param      u0 - Vertex coordinate
    /// @param      v0 - Vertex coordinate
    /// @param      u1 - Vertex coordinate
    /// @param      v1 - Vertex coordinate
    void DrawQuad(float u0, float v0, float u1, float v1);

    /// @brief      Draws a textured quad
    /// @note       Overrides pure virtual
    /// @param      u0 - Vertex coordinate
    /// @param      v0 - Vertex coordinate
    /// @param      u1 - Vertex coordinate
    /// @param      v1 - Vertex coordinate
    /// @param      c  - Colour
    void DrawQuad(float u0, float v0, float u1, float v1, prColour c);

    /// @brief      Draws a textured quad
    /// @note       Overrides pure virtual
    /// @param      u0 - Vertex coordinate
    /// @param      v0 - Vertex coordinate
    /// @param      u1 - Vertex coordinate
    /// @param      v1 - Vertex coordinate
    /// @param      c  - Colour
    void BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c);

    /// @brief      Sets the draw colour.
    /// @note       Overrides pure virtual
    /// @param      colour - Colour
    void SetColour(const prColour &colour);

    /// @brief      Sets the clear colour.
    /// @note       Overrides pure virtual
    /// @param      colour - Colour
    void SetClearColour(const prColour &colour);

    /// @brief      Enables/disables textures.
    /// @note       Overrides pure virtual
    /// @param      state - true or false
    void TexturesEnabled(bool state);
};


#endif//__PRRENDERER_GL11_H
