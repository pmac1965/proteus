// File: prRenderer.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
class  prTexture;
struct prVertex2D;


// Class: prRenderer
//      Renderer base class
//
// Notes:
//      All renderers will implement the basic core functionality
class prRenderer : public prCoreSystem
{
public:
    // Method: prRenderer
    //      Ctor
    prRenderer();

    // Method: ~prRenderer
    //      Dtor
    virtual ~prRenderer();

    // Method: SetWindow
    //      Sets the window that the renderer draws to.
    //
    // Parameters:
    //      window - A pointer to a window class
    void SetWindow(prWindow *window);

    // Method: GetWindow
    //      Gets the window thats drawn to.
    //
    //  Returns:
    //      A pointer to a window class, or null if not set
    prWindow *GetWindow() { return m_pWindow; }

    // Method: Init
    //      Inits the renderer with basic settings.
    virtual void Init() = 0;

    // Method: Destroy
    //      Destroys the renderer.
    virtual void Destroy() = 0;

    // Method: Begin
    //      Begins the image rendering cycle.
    virtual void Begin() = 0;

    // Method: End
    //      Ends the image rendering cycle.
    virtual void End() = 0;

    // Method: Present
    //      Shows the previously rendered image.
    virtual void Present() = 0;

    // Method: SetOrthographicView
    //      Set orthographic view in preparation for 2D rendering.
    virtual void SetOrthographicView() = 0;

    // Method: RestorePerspectiveView
    //      Restore perspective projection.
    virtual void RestorePerspectiveView() = 0;
    
    // Method: DrawPoint
    //      Draws a single point.
    //
    // Parameters:
    //      x - The x coordinate
    //      y - The y coordinate
    virtual void DrawPoint(f32 x, f32 y) = 0;
    
    // Method: DrawLine
    //      Draws a line.
    //
    // Parameters:
    //      x1 - The start x coordinate
    //      y1 - The start y coordinate
    //      x2 - The end x coordinate
    //      y2 - The end y coordinate
    virtual void DrawLine(f32 x1, f32 y1, f32 x2, f32 y2) = 0;

    // Method: DrawLine
    //      Draws a line 3D.
    //
    // Parameters:
    //      from - The start coordinate
    //      to   - The end coordinate
    virtual void DrawLine(prVector3 &from, prVector3 &to) = 0;
    
    // Method: DrawRect
    //      Draws a hollow rectangle.
    //
    // Parameters:
    //      x1 - The start x coordinate (Top left corner)
    //      y1 - The start y coordinate (Top left corner)
    //      x2 - The end x coordinate (Bottom right corner)
    //      y2 - The end y coordinate (Bottom right corner)
    virtual void DrawRect(f32 x1, f32 y1, f32 x2, f32 y2) = 0;
    
    // Method: DrawFilledRect
    //      Draws a filled rectangle.
    //
    // Parameters:
    //      x1 - The start x coordinate (Top left corner)
    //      y1 - The start y coordinate (Top left corner)
    //      x2 - The end x coordinate (Bottom right corner)
    //      y2 - The end y coordinate (Bottom right corner)
    virtual void DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2) = 0;
    
    // Method: DrawCircle
    //      Draws a hollow circle.
    //
    // Parameters:
    //      x      - The start x coordinate (center)
    //      y      - The start y coordinate (center)
    //      radius - The radius
    virtual void DrawCircle(f32 x, f32 y, f32 radius) = 0;

    // Method: DrawFilledCircle
    //      Draws a filled circle.
    //
    // Parameters:
    //      x      - The start x coordinate (center)
    //      y      - The start y coordinate (center)
    //      radius - The radius
    virtual void DrawFilledCircle(f32 x, f32 y, f32 radius) = 0;

    // Method: DrawPolygon
    //      Draws a hollow polygon.
    //
    // Parameters:
    //      vertices - A pointer to the vertex data
    //      count    - Number of vertices
    virtual void DrawPolygon(prVertex2D *vertices, s32 count) = 0;

    // Method: DrawFilledPolygon
    //      Draws a filled polygon.
    //
    // Parameters:
    //      vertices - A pointer to the vertex data
    //      count    - Number of vertices
    virtual void DrawFilledPolygon(prVertex2D *vertices, s32 count) = 0;
    
    // Method: DrawQuad
    //      Draws a textured quad
    virtual void DrawQuad() = 0;
    
    // Method: DrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    virtual void DrawQuad(float u0, float v0, float u1, float v1) = 0;

    // Method: DrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    //      c  - Colour
    virtual void DrawQuad(float u0, float v0, float u1, float v1, prColour c) = 0;

    // Method: BatchDrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    //      c  - Colour
    virtual void BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c) = 0;

    // Method: SetColour
    //      Sets the draw colour.
    //
    // Parameters:
    //      colour - Colour
    virtual void SetColour(const prColour &colour) = 0;

    // Method: SetClearColour
    //      Sets the clear colour.
    //
    // Parameters:
    //      colour - Colour
    virtual void SetClearColour(const prColour &colour) = 0;

    // Method: TexturesEnabled
    //      Enables/disables textures.
    //
    // Parameters:
    //      state - true or false
    virtual void TexturesEnabled(bool state) = 0;

    // Method: BlendEnabled
    //      Enables/disables blending.
    //
    // Parameters:
    //      state - true or false
    virtual void BlendEnabled(bool state) = 0;


protected:

    prWindow    *m_pWindow;
    prTexture   *m_pWatermark;
};


#endif//__PRRENDERER_H
