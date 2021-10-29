#include "Maths/Vec4.hpp"
#include <cmath>

const Vec4 Vec4::zero{ 0.0f, 0.0f, 0.0f, 0.0f };
const Vec4 Vec4::one{ 0.0f, 0.0f, 0.0f, 0.0f };

Vec4::Vec4()
    : w{ 1 }, x{ 0 }, y{ 0 }, z{ 0 }
{}

Vec4::Vec4(float _x, float _y, float _z)
    : w{ 1 }, x{ _x }, y{ _y }, z{ _z }
{}

Vec4::Vec4(float _x, float _y, float _z, float _w)
    : w{ _w }, x{ _x }, y{ _y }, z{ _z }
{}

Vec4::Vec4(const Vec3& v, float _w)
    : w{ _w }, x{ v.x }, y{ v.y }, z{ v.z }
{}

Vec4::Vec4(const Vec4& v)
    : w{ v.w }, x{ v.x }, y{ v.y }, z{ v.z }
{}

Vec4 Vec4::Normalized() const
{
    float magnitude = GetMagnitude();

    return { w / magnitude, x / magnitude, y / magnitude, z / magnitude };
}

void Vec4::Normalize()
{
    float magnitude = GetMagnitude();

    w /= magnitude;
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

float Vec4::GetMagnitude() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

float Vec4::GetSquaredMagnitude() const
{
    return w * w + x * x + y * y + z * z;
}

void Vec4::Lerp(const Vec4& _v2, float _t)
{
    *this = *this + (_v2 - *this) * _t;

}

Vec4 Vec4::Lerp(const Vec4& _v1, const Vec4& _v2, float _t)
{
    return _v1 + (_v2 - _v1) * _t;
}

Vec4 Vec4::operator+(const Vec4& _v) const
{
    return {x + _v.x, y + _v.y, z + _v.z, w + _v.w};
}

Vec4 Vec4::operator-(const Vec4& _v) const
{
    return {x - _v.x, y - _v.y, z - _v.z, w - _v.w };
}

Vec4 Vec4::operator*(const Vec4& _v) const
{
    return {x * _v.x, y * _v.y, z * _v.z, w * _v.w };
}

Vec4 Vec4::operator*(float _value) const
{
    return {x * _value, y * _value, z * _value, w * _value };
}

Vec4 Vec4::operator/(float _value) const
{
    return {x / _value, y / _value, z / _value, w / _value };
}

void Vec4::operator=(const Vec4& _v)
{
    w = _v.w;
    x = _v.x;
    y = _v.y;
    z = _v.z;
} 

void Vec4::operator+=(const Vec4& _v)
{
    *this = *this + _v;
}

void Vec4::operator-=(const Vec4& _v)
{
    *this = *this - _v;
}

void Vec4::operator*=(const Vec4& _v)
{
    *this = *this * _v;
}

void Vec4::operator*=(float _value)
{
    *this = *this * _value;
}

void Vec4::operator/=(float _value)
{
    *this = *this / _value;
}

Vec4 Vec4::operator-() const
{
    return {-w, -x, -y, -z };
}
