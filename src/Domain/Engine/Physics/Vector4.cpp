#include "Vector4.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"
#include "Matrix4.h"
#include "Quaternion.h"

const Vector4 Vector4::up      = Vector4( 0,  1,  0, 0);
const Vector4 Vector4::down    = Vector4( 0, -1,  0, 0);
const Vector4 Vector4::right   = Vector4( 1,  0,  0, 0);
const Vector4 Vector4::left    = Vector4(-1,  0,  0, 0);
const Vector4 Vector4::forward = Vector4( 0,  0, -1, 0);
const Vector4 Vector4::back    = Vector4( 0,  0,  1, 0);
const Vector4 Vector4::zero    = Vector4( 0,  0,  0, 0);
const Vector4 Vector4::one     = Vector4( 1,  1,  1, 1);

Vector4::Vector4() : Vector4(0.0f)
{
}

Vector4::Vector4(const glm::vec4 &v) : Vector4(v.x, v.y, v.z, v.w)
{
}

Vector4::Vector4(float a) : Vector4(a, a, a, a)
{
}

Vector4::Vector4(float x, float y, float z, float w) : glm::vec4(x,y,z,w)
{
}

Vector4::Vector4(const Color &c) : Vector4(c.r, c.g, c.b, c.a)
{
}

Vector4::Vector4(const Vector3 &v, float w) : Vector4(v.x, v.y, v.z, w)
{
}

Vector4::Vector4(const Vector2 &v, float z, float w) : Vector4(v.x, v.y, z, w)
{
}

float Vector4::Length() const
{
    return float(glm::sqrt(x*x + y*y + z*z + w*w));
}

void Vector4::Normalize()
{
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
}

Vector4 Vector4::Normalized() const
{
    Vector4 v(*this);
    v.Normalize();
    return v;
}

Vector4 Vector4::ToDegrees() const
{
    return Vector4(glm::degrees(glm::vec4(*this)));
}

Vector4 Vector4::ToRadians() const
{
    return Vector4(glm::radians(glm::vec4(*this)));
}

glm::vec4 Vector4::ToGlmVec4() const
{
    return glm::vec4(x,y,z,w);
}

float Vector4::Distance(const Vector4 &p) const
{
    return Vector4::Distance(*this, p);
}

Vector4 Vector4::Lerp(const Vector4 &v1,
                      const Vector4 &v2,
                      float progression)
{
    return Vector4(glm::mix(glm::vec4(v1), glm::vec4(v2), progression));
}

Vector4 Vector4::Abs() const
{
    return Vector4(glm::abs(x), glm::abs(y), glm::abs(z), glm::abs(w));
}

Vector4 Vector4::Abs(const Vector4 &v)
{
    return v.Abs();
}

float Vector4::Dot(const Vector4 &v1, const Vector4 &v2)
{
    return glm::dot(glm::vec4(v1), glm::vec4(v2));
}

float Vector4::Distance(const Vector4 &v1, const Vector4 &v2)
{
    return (v1 - v2).Length();
}

Vector2 Vector4::xy() const
{
    return Vector2(x, y);
}

Vector3 Vector4::xyz() const
{
    return Vector3(x, y, z);
}










Vector4 operator*(Quaternion q, const Vector4 &rhs)
{
    return Vector4(q * glm::vec4(rhs.x, rhs.y, rhs.z, rhs.w));
}


Vector4 operator+(const Vector4 & v1, const Vector4 &v2)
{
    return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w * v2.w);
}


Vector4 operator*(const Vector4 &v1, const Vector4 &v2)
{
    return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}


Vector4 operator-(const Vector4 &v)
{
    return Vector4(-v.x, -v.y, -v.z, -v.w);
}


Vector4 operator*(float a, const Vector4 &v)
{
    return Vector4(a * v.x, a * v.y, a * v.z, a * v.w);
}


Vector4 operator*(const Vector4 &v, float a)
{
    return a * v;
}


Vector4 operator/(float a, const Vector4 &v)
{
    return Vector4(a / v.x, a / v.y, a / v.z, a / v.w);
}


Vector4 operator/(const Vector4 &v, float a)
{
    return Vector4(v.x / a, v.y / a, v.z / a, v.w / a);
}


Vector4 operator/(const Vector4 &v1, const Vector4 &v2)
{
    return Vector4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}


Vector4 &operator+=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}


Vector4 &operator-=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

Vector4 &operator*=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    lhs.w *= rhs.w;
    return lhs;
}


Vector4 &operator/=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    lhs.w /= rhs.w;
    return lhs;
}


Vector4 operator+(float a, const Vector4 &v)
{
    return Vector4(a + v.x, a + v.y, a + v.z, a + v.w);
}


Vector4 operator+(const Vector4 &v, float a)
{
    return a + v;
}


Vector4 operator-(const Vector4 &v1, const Vector4 &v2)
{
    return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}


Vector4 operator*(const Matrix4 &m, const Vector4 &v)
{
    return Vector4(glm::mat4(m) * glm::vec4(v));
}


Vector4 operator-(float a, const Vector4 &v)
{
    return Vector4(a - v.x, a - v.y, a - v.z, a - v.w);
}


Vector4 operator-(const Vector4 &v, float a)
{
    return Vector4(v.x - a, v.y - a, v.z - a, v.w - a);
}

Vector4 &operator+=(Vector4 &lhs, float a)
{
    lhs.x += a;
    lhs.y += a;
    lhs.z += a;
    lhs.w += a;
    return lhs;
}


Vector4 &operator-=(Vector4 &lhs, float a)
{
    lhs.x -= a;
    lhs.y -= a;
    lhs.z -= a;
    lhs.w -= a;
    return lhs;
}


Vector4 &operator*=(Vector4 &lhs, float a)
{
    lhs.x *= a;
    lhs.y *= a;
    lhs.z *= a;
    lhs.w *= a;
    return lhs;
}


Vector4 &operator/=(Vector4 &lhs, float a)
{
    lhs.x /= a;
    lhs.y /= a;
    lhs.z /= a;
    lhs.w /= a;
    return lhs;
}