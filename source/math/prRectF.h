// File: prRectF.h
/*
    Copyright 2014 Paul Michael McNab

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef __PRRECTF_H
#define __PRRECTF_H


#include "../core/prTypes.h"
#include "prPoint.h"
#include "prMathsUtil.h"


// Forward declarations
class prRect;


// class: prRectF
//      Represents a simple rectangle.
class prRectF
{
public:        
    // Method: prRectF
    //      Constructor
    prRectF();

    // Method: prRectF
    //      Constructor
    //
    // Parameters:
    //      top    - Rectangle coordinate 
    //      left   - Rectangle coordinate 
    //      bottom - Rectangle coordinate
    //      right  - Rectangle coordinate
    prRectF(f32 top, f32 left, f32 bottom, f32 right);

    // Method: prRectF
    //      Constructor
    //
    // Parameters:
    //      topLeft     - Rectangle coordinate 
    //      bottomRight - Rectangle coordinate 
    prRectF(const prPoint3F& topLeft, const prPoint3F& bottomRight);

    // Method: ~prRectF
    //      Destructor
    ~prRectF()
    {}
                    
    // Method: PointInside
    //       Determines if the passed coordinates are within the rectangle.
    //
    // Parameters:
    //      x    - X coordinate 
    //      y    - Y coordinate 
    //
    // Returns:
    //      true or false
    bool PointInside(f32 x, f32 y) const;

    // Method: PointInside
    //      Determines if the passed coordinates are within the rectangle.
    //
    // Parameters:
    //      point    - XY coordinates. Z is ignored.
    //
    // Returns:
    //      true or false
    bool PointInside(const prPoint3& point) const;

    // Method: PointInside
    //      Determines if the passed coordinates are within the rectangle.
    //
    // Parameters:
    //      point    - XY coordinates. Z is ignored.
    //
    // Returns:
    //      true or false
    bool PointInside(const prPoint3F& point) const;

    // Method: Resize
    //      Resizes the rectangle.
    //
    // Parameters:
    //      top    - Adjustment value 
    //      left   - Adjustment value
    //      bottom - Adjustment value
    //      right  - Adjustment value
    void Resize(f32 top, f32 left, f32 bottom, f32 right);                    

    // Method: Set
    //      Sets the size of the rectangle.
    //
    // Parameters:
    //      top    - Rectangle coordinate 
    //      left   - Rectangle coordinate 
    //      bottom - Rectangle coordinate
    //      right  - Rectangle coordinate
    void Set(f32 top, f32 left, f32 bottom, f32 right);
                   
    // Method: IsEmpty
    //      Determines if the rectangle has zero width and height.
    //
    // Returns:
    //      true or false
    bool IsEmpty() const { return (IS_ZERO(m_left - m_right) && IS_ZERO(m_top - m_bottom)); }
                    
    // Method: GetTop
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    f32 GetTop() const { return m_top; }            
    
    // Method: GetBottom
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    f32 GetBottom() const { return m_bottom; }
    
    // Method: GetLeft
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    f32 GetLeft() const { return m_left; }
        
    // Method: GetRight
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    f32 GetRight() const { return m_right; }
        
    // Method: GetWidth
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle width
    f32 GetWidth() const { return m_width; }
        
    // Method: GetHeight
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle height
    f32 GetHeight() const { return m_height; }
                
    // Method: Intersect
    //      Determine whether the passed rectangle intersects this rectangle.
    //
    // Parameters:
    //      rect - A rectangle
    //
    // Returns:
    //      true or false
    bool Intersect(const prRectF& rect) const;            

    // Method: Intersect
    //      Determine whether the passed rectangle intersects this rectangle.
    //
    // Parameters:
    //      rect - A rectangle
    //
    // Returns:
    //      true or false
    bool Intersect(const prRect& rect) const;            
    
    // Method: Move
    //      Changes the position of the rectangle.
    //
    // Parameters:
    //      x    - Adjustment value
    //      y    - Adjustment value
    void Move(f32 x, f32 y);
    

private:

    f32     m_top;
    f32     m_left;
    f32     m_bottom;
    f32     m_right;            
    f32     m_width;
    f32     m_height;


private:

    // This method ensures that left is less than right and top greater than botton.
    void VerifyCoords();    
};


#endif//__PRRECTF_H
