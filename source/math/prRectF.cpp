/**
 * prRectF.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"
#include "prRectF.h"
#include "prRect.h"
#include "prPoint.h"


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prRectF::prRectF()
: m_top    (0)
, m_left   (0)
, m_bottom (0)
, m_right  (0) 
, m_width  (0) 
, m_height (0) 
{
}


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prRectF::prRectF(f32 top, f32 left, f32 bottom, f32 right)
: m_top    (top)
, m_left   (left)
, m_bottom (bottom)
, m_right  (right)
{
    VerifyCoords();
}   
    

/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prRectF::prRectF(const prPoint3F& topLeft, const prPoint3F& bottomRight)
: m_top    (topLeft.y)
, m_left   (topLeft.x)
, m_bottom (bottomRight.y)
, m_right  (bottomRight.x)
{
    VerifyCoords();
}


/// ---------------------------------------------------------------------------
/// Determines if the passed coordinates are within the rectangle.
/// ---------------------------------------------------------------------------
bool prRectF::PointInside(f32 x, f32 y) const
{
    if (IsEmpty() == false)
    {
        return 
        ( 
            x >= m_left     &&
            x <= m_right    &&
            y <= m_bottom   &&
            y >= m_top
        );
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Determines if the passed coordinates are within the rectangle.
/// ---------------------------------------------------------------------------
bool prRectF::PointInside(const prPoint3& point) const
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


/// ---------------------------------------------------------------------------
/// Determines if the passed coordinates are within the rectangle.
/// ---------------------------------------------------------------------------
bool prRectF::PointInside(const prPoint3F& point) const
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


/// ---------------------------------------------------------------------------
/// Resizes the rectangle.
/// ---------------------------------------------------------------------------
void prRectF::Resize(f32 top, f32 left, f32 bottom, f32 right)
{
    m_top    += top;
    m_left   += left;
    m_bottom += bottom;
    m_right  += right;

    VerifyCoords();
}


/// ---------------------------------------------------------------------------
/// Sets the size of the rectangle.
/// ---------------------------------------------------------------------------
void prRectF::Set(f32 top, f32 left, f32 bottom, f32 right)
{
    m_top    = top;
    m_left   = left;
    m_bottom = bottom;
    m_right  = right;
                
    VerifyCoords();
}


/// ---------------------------------------------------------------------------
/// Ensures that the coordinates make sense.
/// ---------------------------------------------------------------------------
void prRectF::VerifyCoords()
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
    
    
/// ---------------------------------------------------------------------------
/// Determine whether two rectangles intersect.
/// ---------------------------------------------------------------------------
bool prRectF::Intersect(const prRect& rect) const
{
    return (
                m_right   >= rect.GetLeft()   &&
                m_left    <= rect.GetRight()  &&
                m_bottom  >= rect.GetTop()    &&
                m_top     <= rect.GetBottom()
           );
}


/// ---------------------------------------------------------------------------
/// Determine whether two rectangles intersect.
/// ---------------------------------------------------------------------------
bool prRectF::Intersect(const prRectF& rect) const
{
    return (
                m_right   >= rect.GetLeft()   &&
                m_left    <= rect.GetRight()  &&
                m_bottom  >= rect.GetTop()    &&
                m_top     <= rect.GetBottom()
           );
}


/// ---------------------------------------------------------------------------
/// Changes the position of the rectangle.
/// ---------------------------------------------------------------------------
void prRectF::Move(f32 x, f32 y)
{
    m_right  += x;
    m_left   += x;
    m_bottom += y;
    m_top    += y;
}
