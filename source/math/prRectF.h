/**
 * @file       prRectF.h
 * @brief      Contains a floating point rectangle class.
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


#ifndef __PRRECTF_H
#define __PRRECTF_H


#include "../core/prTypes.h"
#include "prPoint.h"
#include "prMathsUtil.h"


// Forward declarations
class prRect;


/// Represents a simple rectangle.
class prRectF
{
public:
        
    /// Constructor.
    prRectF();

    /// Constructor.
    /// @param      top    - Rectangle coordinate 
    /// @param      left   - Rectangle coordinate 
    /// @param      bottom - Rectangle coordinate
    /// @param      right  - Rectangle coordinate
    prRectF(f32 top, f32 left, f32 bottom, f32 right);

    /// Constructor.
    /// @param      topLeft     - Rectangle coordinate 
    /// @param      bottomRight - Rectangle coordinate 
    prRectF(const prPoint3F& topLeft, const prPoint3F& bottomRight);

    /// Dtor.
    ~prRectF()
    {}
                    
    /// Determines if the passed coordinates are within the rectangle.
    /// @param      x    - X coordinate 
    /// @param      y    - Y coordinate 
    /// @return     true or false
    bool PointInside(f32 x, f32 y) const;

    /// Determines if the passed coordinates are within the rectangle.
    /// @param      point    - XY coordinates
    /// @return     true or false
    bool PointInside(const prPoint3& point) const;

    /// Determines if the passed coordinates are within the rectangle.
    /// @param      point    - XY coordinates
    /// @return     true or false
    bool PointInside(const prPoint3F& point) const;

    /// Resizes the rectangle.
    /// @param      top    - Adjustment value 
    /// @param      left   - Adjustment value
    /// @param      bottom - Adjustment value
    /// @param      right  - Adjustment value
    void Resize(f32 top, f32 left, f32 bottom, f32 right);                    

    /// Sets the size of the rectangle.
    /// @param      top    - Rectangle coordinate 
    /// @param      left   - Rectangle coordinate 
    /// @param      bottom - Rectangle coordinate
    /// @param      right  - Rectangle coordinate
    void Set(f32 top, f32 left, f32 bottom, f32 right);
                   
    /// Determines if the rectangle has zero width and height.
    /// @return     true or false
    bool IsEmpty() const { return (IS_ZERO(m_left - m_right) && IS_ZERO(m_top - m_bottom)); }
                    
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    f32 GetTop() const { return m_top; }            
    
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    f32 GetBottom() const { return m_bottom; }
    
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    f32 GetLeft() const { return m_left; }
        
    /// Gets rectangle coordinate.
    /// @return     Rectangle coordinate
    f32 GetRight() const { return m_right; }
        
    /// Gets rectangles width.
    /// @return     Rectangle width
    f32 GetWidth() const { return m_width; }
        
    /// Gets rectangles height.
    /// @return     Rectangle height
    f32 GetHeight() const { return m_height; }
                
    /// Determine whether the passed rectangle intersects this rectangle.
    /// @param      rect - A rectangle
    /// @return     true or false
    bool Intersect(const prRectF& rect) const;            

    /// Determine whether the passed rectangle intersects this rectangle.
    /// @param      rect - A rectangle
    /// @return     true or false
    bool Intersect(const prRect& rect) const;            
    
    /// Changes the position of the rectangle.
    /// @param      x    - Adjustment value
    /// @param      y    - Adjustment value
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
