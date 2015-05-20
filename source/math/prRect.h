// File: prRect.h
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
 */


#ifndef __PRRECT_H
#define __PRRECT_H


#include "../core/prTypes.h"
#include "prPoint.h"


// Namespaces
namespace Proteus {
namespace Math {


// class: prRect
//      Represents a simple rectangle.
class prRect
{
public:        
    // Method: prRect
    //      Constructor
    prRect();

    // Method: prRect
    //      Constructor
    //
    // Parameters:
    //      top    - Rectangle coordinate 
    //      left   - Rectangle coordinate 
    //      bottom - Rectangle coordinate
    //      right  - Rectangle coordinate
    prRect(Proteus::Core::s32 top, Proteus::Core::s32 left, Proteus::Core::s32 bottom, Proteus::Core::s32 right);

    // Method: prRect
    //      Constructor
    //
    // Parameters:
    //      top    - Rectangle coordinate 
    //      left   - Rectangle coordinate 
    //      bottom - Rectangle coordinate
    //      right  - Rectangle coordinate
    prRect(Proteus::Core::f32 top, Proteus::Core::f32 left, Proteus::Core::f32 bottom, Proteus::Core::f32 right);

    // Method: prRect
    //      Constructor
    //
    // Parameters:
    //      topLeft     - Rectangle coordinate 
    //      bottomRight - Rectangle coordinate 
    prRect(const prPoint3& topLeft, const prPoint3& bottomRight);

    // Method: ~prRect
    //      Destructor
    ~prRect()
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
    bool PointInside(Proteus::Core::s32 x, Proteus::Core::s32 y) const;
    
    // Method: PointInside
    //      Determines if the passed coordinates are within the rectangle.
    //
    // Parameters:
    //      point    - XY coordinates
    //
    // Returns:
    //      true or false
    bool PointInside(const prPoint3& point) const;

    // Method: Resize
    //      Resizes the rectangle.
    //
    // Parameters:
    //      top    - Adjustment value 
    //      left   - Adjustment value
    //      bottom - Adjustment value
    //      right  - Adjustment value
    void Resize(Proteus::Core::s32 top, Proteus::Core::s32 left, Proteus::Core::s32 bottom, Proteus::Core::s32 right);                    

    // Method: Set
    //      Sets the size of the rectangle.
    //
    // Parameters:
    //      top    - Rectangle coordinate 
    //      left   - Rectangle coordinate 
    //      bottom - Rectangle coordinate
    //      right  - Rectangle coordinate
    void Set(Proteus::Core::s32 top, Proteus::Core::s32 left, Proteus::Core::s32 bottom, Proteus::Core::s32 right);
                   
    // Method: IsEmpty
    //      Determines if the rectangle has zero width and height.
    //
    // Returns:
    //      true or false
    bool IsEmpty() const { return ((m_left - m_right) == 0  &&  (m_top - m_bottom) == 0); }
                    
    // Method: GetTop
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    Proteus::Core::s32 GetTop() const { return m_top; }            
    
    // Method: GetBottom
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    Proteus::Core::s32 GetBottom() const { return m_bottom; }
    
    // Method: GetLeft
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    Proteus::Core::s32 GetLeft() const { return m_left; }
        
    // Method: GetRight
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle coordinate
    Proteus::Core::s32 GetRight() const { return m_right; }
        
    // Method: GetWidth
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle width
    Proteus::Core::s32 GetWidth() const { return m_width; }
        
    // Method: GetHeight
    //      Gets rectangle coordinate.
    //
    // Returns:
    //      Rectangle height
    Proteus::Core::s32 GetHeight() const { return m_height; }
                
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
    void Move(Proteus::Core::s32 x, Proteus::Core::s32 y);
    

private:
    Proteus::Core::s32     m_top;
    Proteus::Core::s32     m_left;
    Proteus::Core::s32     m_bottom;
    Proteus::Core::s32     m_right;            
    Proteus::Core::s32     m_width;
    Proteus::Core::s32     m_height;


private:
    // This method ensures that left is less than right and top greater than botton.
    void VerifyCoords();    
};


}}// Namespaces


#endif//__PRRECT_H
