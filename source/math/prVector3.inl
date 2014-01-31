// ------------------------------------------------------------------------------------------------
//
// prVector3.inl
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
inline float prVector3::Magnitude() const
{
    return sqrt((x * x) + (y * y) + (z * z));
}


// ------------------------------------------------------------------------------------------------
// Calculates the magnitude of a vector squared.
// ------------------------------------------------------------------------------------------------
inline float prVector3::MagnitudeSquared() const
{
    return ((x * x) + (y * y) + (z * z));
}


// ------------------------------------------------------------------------------------------------
// Normalise this vector.
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::Normalize()
{
    float mag = Magnitude();

    if (mag > 0.0f)
    {
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }

    return *this;
}


// ------------------------------------------------------------------------------------------------
// Returns the dot product.
// ------------------------------------------------------------------------------------------------
inline float prVector3::DotProduct(const prVector3 &v) const
{
    return ((x * v.x) +
            (y * v.y) +
            (z * v.z));
}


// ------------------------------------------------------------------------------------------------
// Returns the cross product.
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::CrossProduct(const prVector3 &v) const
{
    return prVector3(y * v.z - z * v.y,
                     z * v.x - x * v.z,
                     x * v.y - y * v.x);
}


// ------------------------------------------------------------------------------------------------
// Operator +
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator + (const prVector3& rhs) const
{
    return prVector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

// Operator +
inline prVector3 prVector3::operator + (const float& rhs) const
{
    return prVector3(x + rhs, y + rhs, z + rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator -
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator - (const prVector3& rhs) const
{
    return prVector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

// Operator -
inline prVector3 prVector3::operator - (const float& rhs) const
{
    return prVector3(x - rhs, y - rhs, z - rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator *
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator * (const prVector3& rhs) const
{
    return prVector3(x * rhs.x, y * rhs.y, z * rhs.z);
}

// Operator *
inline prVector3 prVector3::operator * (const float& rhs) const
{
    return prVector3(x * rhs, y * rhs, z * rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator /
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator / (const prVector3& rhs) const
{
    return prVector3(x / rhs.x, y / rhs.y, z / rhs.z);
}

// Operator /
inline prVector3 prVector3::operator / (const float& rhs) const
{
    return prVector3(x / rhs, y / rhs, z / rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator +=
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::operator += (const prVector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    
    return *this;       
}


// ------------------------------------------------------------------------------------------------
// Operator -=
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::operator -= (const prVector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    
    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator *=
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::operator *= (const float val)
{
    x = x * val;
    y = y * val;
    z = z * val;

    return *this;    
}
