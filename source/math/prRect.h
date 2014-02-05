// File:
//      prRect.h
//
// Contents:
//      Contains a rectangle class.
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


#ifndef __PRRECT_H
#define __PRRECT_H


#include "../core/prTypes.h"
#include "prPoint.h"


// class: prRect
//      Represents a simple rectangle.
class prRect
{
public:
        
    // method: prRect
    //      Constructor.
    prRect();

    /// Constructor.
    /// @param      top    - Rectangle coordinate 
    /// @param      left   - Rectangle coordinate 
    /// @param      bottom - Rectangle coordinate
    /// @param      right  - Rectangle coordinate
    prRect(s32 top, s32 left, s32 bottom, s32 right);

    /// Constructor.
    /// @param      top    - Rectangle coordinate 
    /// @param      left   - Rectangle coordinate 
    /// @param      bottom - Rectangle coordinate
    /// @param      right  - Rectangle coordinate
    prRect(f32 top, f32 left, f32 bottom, f32 right);

    /// Constructor.
    /// @param      topLeft     - Rectangle coordinate 
    /// @param      bottomRight - Rectangle coordinate 
    prRect(const prPoint3& topLeft, const prPoint3& bottomRight);

    /// Dtor.
    ~prRect()
    {}
                    
    /// Determines if the passed coordinates are within the rectangle.
    /// @param      x    - X coordinate 
    /// @param      y    - Y coordinate 
    /// @return     true or false
    bool PointInside(s32 x, s32 y) const;

    /// Determines if the passed coordinates are within the rectangle.
    /// @param      point    - XY coordinates
    /// @return     true or false
    bool PointInside(const prPoint3& point) const;

    /// Resizes the rectangle.
    /// @param      top    - Adjustment value 
    /// @param      left   - Adjustment value
    /// @param      bottom - Adjustment value
    /// @param      right  - Adjustment value
    void Resize(s32 top, s32 left, s32 bottom, s32 right);                    

    /// Sets the size of the rectangle.
    /// @param      top    - Rectangle coordinate 
    /// @param      left   - Rectangle coordinate 
    /// @param      bottom - Rectangle coordinate
    /// @param      right  - Rectangle coordinate
    void Set(s32 top, s32 left, s32 bottom, s32 right);
                   
    /// Determines if the rectangle has zero width and height.
    /// @return     true or false
    bool IsEmpty() const { return ((m_left - m_right) == 0  &&  (m_top - m_bottom) == 0); }
                    
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    s32 GetTop() const { return m_top; }            
    
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    s32 GetBottom() const { return m_bottom; }
    
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    s32 GetLeft() const { return m_left; }
        
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    s32 GetRight() const { return m_right; }
        
    /// Gets rectangles width.
    /// @return     Rectangle coordinate
    s32 GetWidth() const { return m_width; }
        
    /// Gets rectangles height.
    /// @return     Rectangle coordinate
    s32 GetHeight() const { return m_height; }
                
    /// Determine whether the passed rectangle intersects this rectangle.
    /// @param      rect - A rectangle
    /// @return     true or false
    bool Intersect(const prRect& rect) const;            
    
    /// Changes the position of the rectangle.
    /// @param      x    - Adjustment value
    /// @param      y    - Adjustment value
    void Move(s32 x, s32 y);
    

private:

    s32     m_top;
    s32     m_left;
    s32     m_bottom;
    s32     m_right;            
    s32     m_width;
    s32     m_height;


private:

    /// This method ensures that left is less than right and top greater than botton.
    void VerifyCoords();    
};


#endif//__PRRECT_H
