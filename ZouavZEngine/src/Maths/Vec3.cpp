#include "Maths/Vec4.hpp"
#include <cmath>
#include "Maths/Vec3.hpp"

const Vec3 Vec3::zero{ 0.0f, 0.0f, 0.0f };
const Vec3 Vec3::one{ 1.0f, 1.0f, 1.0f };
const Vec3 Vec3::forward{ 0.0f, 0.0f, 1.0f };
const Vec3 Vec3::backward{ 0.0f, 0.0f, -1.0f };
const Vec3 Vec3::right{ 1.0f, 0.0f, 0.0f };
const Vec3 Vec3::left{ -1.0f, 0.0f, 0.0f };
const Vec3 Vec3::up{ 0.0f, 1.0f, 0.0f };
const Vec3 Vec3::down{ 0.0f, -1.0f, 0.0f };

Vec3::Vec3()
    : x{ 0 }, y{ 0 }, z{ 0 }
{}

Vec3::Vec3(float _x, float _y, float _z)
    : x{ _x }, y{ _y }, z{ _z }
{}

Vec3::Vec3(const Vec3& _v)
    : x{ _v.x }, y{ _v.y }, z{ _v.z }
{}

Vec3::Vec3(const Vec4& _v)
    : x{ _v.x }, y{ _v.y }, z{ _v.z }
{
}

float Vec3::GetMagnitude() const
{
    return sqrt( x * x + y * y + z * z );
}

float Vec3::GetSquaredMagnitude() const
{
    return x * x + y * y + z * z;
}

void Vec3::Normalize()
{
    float length = GetMagnitude();
    x /= length;
    y /= length;
    z /= length;
}

Vec3 Vec3::Normalized() const
{
    float length = GetMagnitude();
    return { x / length, y / length, z / length };
}

std::string Vec3::ToString() const
{
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}

Vec3 Vec3::operator+(const Vec3& _v) const
{
    return { x + _v.x, y  + _v.y, z + _v.z };
}

Vec3 Vec3::operator-(const Vec3& _v) const
{
    return { x - _v.x, y - _v.y, z - _v.z };
}

Vec3 Vec3::operator*(const Vec3& _v) const
{
    return { x * _v.x, y * _v.y, z * _v.z };
}

Vec3 Vec3::operator*(float _value) const
{
    return { x * _value, y * _value, z * _value };
}

Vec3 Vec3::operator/(float _value) const
{
    return { x / _value, y / _value, z / _value };
}

Vec3 Vec3::operator+(float _value) const
{
    return { x + _value, y + _value, z + _value ,};
}

Vec3 Vec3::operator-(float _value) const
{
    return { x - _value, y - _value, z - _value , };
}

void Vec3::operator=(const Vec3& _v)
{
    x = _v.x;
    y = _v.y;
    z = _v.z;
} 

void Vec3::operator+=(const Vec3& _v)
{
    *this = *this + _v;
}

void Vec3::operator-=(const Vec3& _v)
{
    *this = *this - _v;
}

void Vec3::operator*=(const Vec3& _v)
{
    *this = *this * _v;
}

void Vec3::operator*=(float _value)
{
    *this = *this * _value;
}

void Vec3::operator/=(float _value)
{
    *this = *this / _value;
}

Vec3 Vec3::operator-() const
{
    return { -x, -y, -z };
}

Vec3 Vec3::Cross(const Vec3& _v) const
{
    return { y * _v.z - z * _v.y,
             z * _v.x - x * _v.z,
             x * _v.y - y * _v.x };
} 

Vec3 operator*(float _number, const Vec3& _rhs)
{
    return _rhs * _number;
}

float Vec3::Dot(const Vec3& _v) const
{
    return x * _v.x + y * _v.y + z * _v.z;
}

float Vec3::AngleToVector(const Vec3& _v) const
{
    return acos(Dot(_v) / (GetMagnitude() * _v.GetMagnitude()));
}

float Vec3::SignedAngleToVector(const Vec3& _v, const Vec3& _n) const
{
    return atan2(Cross(_v).Dot(_n), Dot(_v));
}

float Vec3::DistanceToVector(const Vec3& _v) const
{
    return sqrt( ( ( _v.x - x ) * ( _v.x - x ) ) + ( ( _v.y - y ) * ( _v.y - y ) ) + ( ( _v.z - z ) * ( _v.z - z ) ) );
}

bool Vec3::Colinear(const Vec3& _vector) const
{
    float colinearFactor = Cross(_vector).GetMagnitude();

    return (colinearFactor > -0.1 && colinearFactor < 0.1);
}