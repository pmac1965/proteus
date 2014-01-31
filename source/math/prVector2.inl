// ------------------------------------------------------------------------------------------------
//
// prVector2.inl
//
// Disclaimer:
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//      TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//      PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//      CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//      EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//      PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//      PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//      LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//      NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ------------------------------------------------------------------------------------------------


#include <math.h>


// ------------------------------------------------------------------------------------------------
// Calculates the magnitude of a vector.
// ------------------------------------------------------------------------------------------------
inline float prVector2::Magnitude() const
{
    return sqrt((x * x) + (y * y));
}


// ------------------------------------------------------------------------------------------------
// Calculates the magnitude of a vector squared.
// ------------------------------------------------------------------------------------------------
inline float prVector2::MagnitudeSquared() const
{
    return ((x * x) + (y * y));
}


// ------------------------------------------------------------------------------------------------
// Normalise this vector.
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::Normalize()
{
    float mag = Magnitude();

    if (mag > 0.0f)
    {
        x = x / mag;
        y = y / mag;
    }

    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator +
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator + (const prVector2& rhs) const
{
    return prVector2(x + rhs.x, y + rhs.y);
}

// Operator +
inline prVector2 prVector2::operator + (const float& rhs) const
{
    return prVector2(x + rhs, y + rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator -
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator - (const prVector2& rhs) const
{
    return prVector2(x - rhs.x, y - rhs.y);
}

// Operator -
inline prVector2 prVector2::operator - (const float& rhs) const
{
    return prVector2(x - rhs, y - rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator *
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator * (const prVector2& rhs) const
{
    return prVector2(x * rhs.x, y * rhs.y);
}

// Operator *
inline prVector2 prVector2::operator * (const float& rhs) const
{
    return prVector2(x * rhs, y * rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator /
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator / (const prVector2& rhs) const
{
    return prVector2(x / rhs.x, y / rhs.y);
}

// Operator /
inline prVector2 prVector2::operator / (const float& rhs) const
{
    return prVector2(x / rhs, y / rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator +=
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::operator += (const prVector2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    
    return *this;       
}


// ------------------------------------------------------------------------------------------------
// Operator -=
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::operator -= (const prVector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    
    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator *=
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::operator *= (const float val)
{
    x = x * val;
    y = y * val;

    return *this;
}
