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


#pragma once


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
    void Init() override;

    // Method: Destroy
    //      Destroys the renderer.
    void Destroy() override;

    // Method: Begin
    //      Begins the image rendering cycle.
    void Begin() override;

    // Method: End
    //      Ends the image rendering cycle.
    void End() override;

    // Method: Present
    //      Shows the previously rendered image.
    void Present() override;

    // Method: SetOrthographicView
    //      Set orthographic view in preparation for 2D rendering.
    void SetOrthographicView() override;

    // Method: RestorePerspectiveView
    //      Restore perspective projection.
    void RestorePerspectiveView() override;
    
    // Method: DrawPoint
    //      Draws a single point.
    //
    // Parameters:
    //      x - The x coordinate
    //      y - The y coordinate
    void DrawPoint(Proteus::Core::f32 x, Proteus::Core::f32 y) override;
    
    // Method: DrawLine
    //      Draws a line.
    //
    // Parameters:
    //      x1 - The start x coordinate
    //      y1 - The start y coordinate
    //      x2 - The end x coordinate
    //      y2 - The end y coordinate
    void DrawLine(Proteus::Core::f32 x1, Proteus::Core::f32 y1, Proteus::Core::f32 x2, Proteus::Core::f32 y2) override;

    // Method: DrawLine
    //      Draws a line 3D.
    //
    // Parameters:
    //      from - The start coordinate
    //      to   - The end coordinate
    void DrawLine(Proteus::Math::prVector3 &from, Proteus::Math::prVector3 &to) override;

    // Method: DrawLine
    //      Draws a line 3D.
    //
    // Parameters:
    //      from - The start coordinate
    //      to   - The end coordinate
    void DrawLine(const Proteus::Math::prVector3 &from, const Proteus::Math::prVector3 &to) override;
    
    // Method: DrawRect
    //      Draws a hollow rectangle.
    //
    // Parameters:
    //      x1 - The start x coordinate (Top left corner)
    //      y1 - The start y coordinate (Top left corner)
    //      x2 - The end x coordinate (Bottom right corner)
    //      y2 - The end y coordinate (Bottom right corner)
    void DrawRect(Proteus::Core::f32 x1, Proteus::Core::f32 y1, Proteus::Core::f32 x2, Proteus::Core::f32 y2) override;
    
    // Method: DrawFilledRect
    //      Draws a filled rectangle.
    //
    // Parameters:
    //      x1 - The start x coordinate (Top left corner)
    //      y1 - The start y coordinate (Top left corner)
    //      x2 - The end x coordinate (Bottom right corner)
    //      y2 - The end y coordinate (Bottom right corner)
    void DrawFilledRect(Proteus::Core::f32 x1, Proteus::Core::f32 y1, Proteus::Core::f32 x2, Proteus::Core::f32 y2) override;
    
    // Method: DrawCircle
    //      Draws a hollow circle.
    //
    // Parameters:
    //      x      - The start x coordinate (center)
    //      y      - The start y coordinate (center)
    //      radius - The radius
    void DrawCircle(Proteus::Core::f32 x, Proteus::Core::f32 y, Proteus::Core::f32 radius) override;

    // Method: DrawFilledCircle
    //      Draws a filled circle.
    //
    // Parameters:
    //      x      - The start x coordinate (center)
    //      y      - The start y coordinate (center)
    //      radius - The radius
    void DrawFilledCircle(Proteus::Core::f32 x, Proteus::Core::f32 y, Proteus::Core::f32 radius) override;

    // Method: DrawPolygon
    //      Draws a hollow polygon.
    //
    // Parameters:
    //      vertices - A pointer to the vertex data
    //      count    - Number of vertices
    void DrawPolygon(prVertex2D *vertices, Proteus::Core::s32 count) override;

    // Method: DrawFilledPolygon
    //      Draws a filled polygon.
    //
    // Parameters:
    //      vertices - A pointer to the vertex data
    //      count    - Number of vertices
    void DrawFilledPolygon(prVertex2D *vertices, Proteus::Core::s32 count) override;
    
    // Method: DrawQuad
    //      Draws a textured quad
    void DrawQuad() override;
    
    // Method: DrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    void DrawQuad(float u0, float v0, float u1, float v1) override;

    // Method: DrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    //      c  - Colour
    void DrawQuad(float u0, float v0, float u1, float v1, prColour c) override;

    // Method: BatchDrawQuad
    //      Draws a textured quad
    //
    // Parameters:
    //      u0 - Vertex coordinate
    //      v0 - Vertex coordinate
    //      u1 - Vertex coordinate
    //      v1 - Vertex coordinate
    //      c  - Colour
    void BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c) override;

    // Method: SetColour
    //      Sets the draw colour.
    //
    // Parameters:
    //      colour - Colour
    void SetColour(const prColour &colour) override;

    // Method: SetClearColour
    //      Sets the clear colour.
    //
    // Parameters:
    //      colour - Colour
    void SetClearColour(const prColour &colour) override;

    // Method: TexturesEnabled
    //      Enables/disables textures.
    //
    // Parameters:
    //      state - true or false
    void TexturesEnabled(bool state) override;

    // Method: BlendEnabled
    //      Enables/disables blending.
    //
    // Parameters:
    //      state - true or false
    void BlendEnabled(bool state) override;

    // Method: DrawGrid
    //      Draws a positioning grid
    //
    // Parameters:
    //      size - The number of squares in either direction.
    //      E.g 2 will give a 4x4 grid
    void DrawGrid(Proteus::Core::s32 size) override;
};
