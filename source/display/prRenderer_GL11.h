// File: prRenderer_GL11.h
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


#ifndef __PRRENDERER_GL11_H
#define __PRRENDERER_GL11_H


#include "prRenderer.h"


// Class: prRenderer_GL11
//      Renderer for OpenGL 1.1
class prRenderer_GL11 : public prRenderer
{
public:
    // Method: prRenderer_GL11
    //      Ctor
    prRenderer_GL11();

    // Method: ~prRenderer_GL11
    //      Dtor
    ~prRenderer_GL11();

    // Method: Init
    //      Inits the renderer with basic settings.
    void Init();

    // Method: Destroy
    //      Destroys the renderer.
    void Destroy();

    // Method: Begin
    //      Begins the image rendering cycle.
    void Begin();

    // Method: End
    //      Ends the image rendering cycle.
    void End();

    // Method: Present
    //      Shows the previously rendered image.
    void Present();

    // Method: SetOrthographicView
    //      Set orthographic view in preparation for 2D rendering.
    void SetOrthographicView();

    // Method: RestorePerspectiveView
    //      Restore perspective projection.
    void RestorePerspectiveView();
    
    // Method: DrawPoint
    //      Draws a single point.
    //
    // Parameters:
    //      x - The x coordinate
    //      y - The y coordinate
    void DrawPoint(f32 x, f32 y);
    
    // Method: DrawLine
    //      Draws a line.
    //
    // Parameters:
    //      x1 - The start x coordinate
    //      y1 - The start y coordinate
    //      x2 - The end x coordinate
    //      y2 - The end y coordinate
    void DrawLine(f32 x1, f32 y1, f32 x2, f32 y2);

    // Method: DrawLine
    //      Draws a line 3D.
    //
    // Parameters:
    //      from - The start coordinate
    //      to   - The end coordinate
    void DrawLine(prVector3 &from, prVector3 &to);
    
    // Method: DrawRect
    //      Draws a hollow rectangle.
    //
    // Parameters:
    //      x1 - The start x coordinate (Top left corner)
    //      y1 - The start y coordinate (Top left corner)
    //      x2 - The end x coordinate (Bottom right corner)
    //      y2 - The end y coordinate (Bottom right corner)
    void DrawRect(f32 x1, f32 y1, f32 x2, f32 y2);
    
    // Method: DrawFilledRect
    //      Draws a filled rectangle.
    //
    // Parameters:
    //      x1 - The start x coordinate (Top left corner)
    //      y1 - The start y coordinate (Top left corner)
    //      x2 - The end x coordinate (Bottom right corner)
    //      y2 - The end y coordinate (Bottom right corner)
    void DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2);
    
    // Method: DrawCircle
    //      Draws a hollow circle.
    //
    // Parameters:
    //      x      - The start x coordinate (center)
    //      y      - The start y coordinate (center)
    //      radius - The radius
    void DrawCircle(f32 x, f32 y, f32 radius);

    // Method: DrawFilledCircle
    //      Draws a filled circle.
    //
    // Parameters:
    //      x      - The start x coordinate (center)
    //      y      - The start y coordinate (center)
    //      radius - The radius
    void DrawFilledCircle(f32 x, f32 y, f32 radius);

    // Method: DrawPolygon
    //      Draws a hollow polygon.
    //
    // Parameters:
    //      vertices - A pointer to the vertex data
    //      count    - Number of vertices
    void DrawPolygon(prVertex2D *vertices, s32 count);

    // Method: DrawFilledPolygon
    //      Draws a filled polygon.
    //
    // Parameters:
    //      vertices - A pointer to the vertex data
    //      count    - Number of vertices
    void DrawFilledPolygon(prVertex2D *vertices, s32 count);
    
    // Method: DrawQuad
    //      Draws a textured quad
    void DrawQuad();
    
    // Method: DrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    void DrawQuad(float u0, float v0, float u1, float v1);

    // Method: DrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    //      c  - Colour
    void DrawQuad(float u0, float v0, float u1, float v1, prColour c);

    // Method: BatchDrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    //      c  - Colour
    void BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c);

    // Method: SetColour
    //      Sets the draw colour.
    //
    // Parameters:
    //      colour - Colour
    void SetColour(const prColour &colour);

    // Method: SetClearColour
    //      Sets the clear colour.
    //
    // Parameters:
    //      colour - Colour
    void SetClearColour(const prColour &colour);

    // Method: TexturesEnabled
    //      Enables/disables textures.
    //
    // Parameters:
    //      state - true or false
    void TexturesEnabled(bool state);

    // Method: BlendEnabled
    //      Enables/disables blending.
    //
    // Parameters:
    //      state - true or false
    void BlendEnabled(bool state);
};


#endif//__PRRENDERER_GL11_H
