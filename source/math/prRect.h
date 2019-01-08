// File: prRect.h
//      Represents a simple rectangle.
//
// Notes:
//      Another coder attempted to improve this code by adding a floating point
//      point version of the integer code. That code was removed and the
//      original code has been templated.
/**
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
 *  PMAC - Converted to template class
 */


#pragma once


#include "prPoint.h"


// Namespaces
namespace Proteus {
namespace Math {


// class: prRect
//      Represents a simple rectangle.
template<typename T>
class prRect
{
public:        
    // Method: prRect
    //      Constructor
    //
    // Parameters:
    //      top    - Rectangle corner coordinate 
    //      left   - Rectangle corner coordinate 
    //      bottom - Rectangle corner coordinate
    //      right  - Rectangle corner coordinate
    prRect(T top, T left, T bottom, T right)
        : m_top    (top)
        , m_left   (left)
        , m_bottom (bottom)
        , m_right  (right)
    {
        VerifyCoords();
    }   

    // Method: prRect
    //      Constructor
    //
    // Parameters:
    //      topLeft     - Rectangle coordinate 
    //      bottomRight - Rectangle coordinate 
    prRect(const prPoint3& topLeft, const prPoint3& bottomRight)
        : m_top    (topLeft.y)
        , m_left   (topLeft.x)
        , m_bottom (bottomRight.y)
        , m_right  (bottomRight.x)
    {
    }
                    
    // Method: PointInside
    //       Determines if the passed coordinates are within the rectangle.
    //
    // Parameters:
    //      x    - X coordinate 
    //      y    - Y coordinate 
    //
    // Returns:
    //      true or false
    bool PointInside(T x, T y) const
    {
        if (IsEmpty() == false)
        {
            return 
            ( 
                x >= m_left    &&
                x <= m_right   &&
                y <= m_bottom  &&
                y >= m_top
            );
        }
    
        return false;           
    }
    
    // Method: PointInside
    //      Determines if the passed coordinates are within the rectangle.
    //
    // Parameters:
    //      point    - XY coordinates
    //
    // Returns:
    //      true or false
    bool PointInside(const prPoint3& point) const
    {
        if (IsEmpty() == false)
        {
            return 
            ( 
                point.x >= m_left    &&
                point.x <= m_right   &&
                point.y <= m_bottom  &&
                point.y >= m_top
            );
        }
    
        return false;           
    }

    // Method: Resize
    //      Resizes the rectangle.
    //
    // Parameters:
    //      top    - Adjustment value 
    //      left   - Adjustment value
    //      bottom - Adjustment value
    //      right  - Adjustment value
    void Resize(T top, T left, T bottom, T right)
    {
        m_top    += top;
        m_left   += left;
        m_bottom += bottom;
        m_right  += right;

        VerifyCoords();
    }

    // Method: Set
    //      Sets the size of the rectangle.
    //
    // Parameters:
    //      top    - Rectangle coordinate 
    //      left   - Rectangle coordinate 
    //      bottom - Rectangle coordinate
    //      right  - Rectangle coordinate
    void Set(T top, T left, T bottom, T right)
    {
        m_top    = top;
        m_left   = left;
        m_bottom = bottom;
        m_right  = right;
                
        VerifyCoords();
    }
                   
    // Method: IsEmpty
    //      Determines if the rectangle has zero width and height.
    //
    // Returns:
    //      true or false
    // TODO("Add template specialization");
    bool IsEmpty() const { return (IS_ZERO(m_left - m_right) && IS_ZERO(m_top - m_bottom)); }
                    
    // Method: GetTop
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    T GetTop() const { return m_top; }            
    
    // Method: GetBottom
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    T GetBottom() const { return m_bottom; }
    
    // Method: GetLeft
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    T GetLeft() const { return m_left; }
        
    // Method: GetRight
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    T GetRight() const { return m_right; }
        
    // Method: GetWidth
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle width
    T GetWidth() const { return m_width; }
        
    // Method: GetHeight
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle height
    T GetHeight() const { return m_height; }
                
    // Method: Intersect
    //      Determine whether the passed rectangle intersects this rectangle.
    //
    // Parameters:
    //      rect - A rectangle
    //
    // Returns:
    //      true or false
    bool Intersect(const prRect& rect) const
    {
        return (m_right   >= rect.GetLeft()   &&
                m_left    <= rect.GetRight()  &&
                m_bottom  >= rect.GetTop()    &&
                m_top     <= rect.GetBottom());
    }
    
    // Method: Move
    //      Changes the position of the rectangle.
    //
    // Parameters:
    //      x    - Adjustment value
    //      y    - Adjustment value
    void Move(T x, T y)
    {
        m_right  += x;
        m_left   += x;
        m_bottom += y;
        m_top    += y;
    }
    

private:
    T     m_top;
    T     m_left;
    T     m_bottom;
    T     m_right;            
    T     m_width;
    T     m_height;


private:
    // This method ensures that left is less than right and top higher than botton.
    void VerifyCoords()
    {
        if (m_right < m_left)
        {
            prSwap(m_left, m_right);
        }
    
        if (m_top > m_bottom)
        {
            prSwap(m_top, m_bottom);
        }
    
        m_width  = m_right  - m_left;
        m_height = m_bottom - m_top;
    }
};


}}// Namespaces
