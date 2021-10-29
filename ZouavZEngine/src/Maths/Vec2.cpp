#include "Maths/Vec2.hpp"
#include <cmath>

const Vec2 Vec2::zero = { 0.0f, 0.0f };
const Vec2 Vec2::one = { 1.0f, 1.0f };
const Vec2 Vec2::up = { 0.0f, 1.0f };
const Vec2 Vec2::down = { 0.0f, -1.0f };
const Vec2 Vec2::right = { 1.0f, 0.0f };
const Vec2 Vec2::left = { -1.0f, 0.0f };

Vec2::Vec2()
    : x{ 0 }, y{ 0 }
{}

Vec2::Vec2(float _x, float _y)
    : x{ _x }, y{ _y }
{}

Vec2::Vec2(const Vec2& _v)
    : x{ _v.x }, y{ _v.y }
{}

float Vec2::DotProduct(const Vec2& _v)
{
    return x * _v.x + y * _v.y;
}

float Vec2::GetMagnitude() const
{
    return sqrt( x * x + y * y );
}

float Vec2::GetSquaredMagnitude() const
{
    return x * x + y * y;
}

void Vec2::Normalize()
{
    float length = GetMagnitude();
    x /= length;
    y /= length;
}

Vec2 Vec2::Normalized() const
{
    float length = GetMagnitude();
    return { x / length, y / length };
}

float Vec2::Cross(const Vec2& v) const
{
    return x * v.y - v.x * y;
}

std::string Vec2::ToString() const
{
    return std::to_string(x) + ", " + std::to_string(y);
}

Vec2 Vec2::operator+(const Vec2& _v) const
{
    return { x + _v.x, y + _v.y };
}

Vec2 Vec2::operator-(const Vec2& _v) const
{
    return { x - _v.x, y - _v.y };
}

Vec2 Vec2::operator-() const
{
    return { -x, -y };
}

Vec2 Vec2::operator*(float _value) const
{
    return { x * _value, y * _value };
}

Vec2 Vec2::operator/(float _value) const
{
    return { x / _value, y / _value };
}

Vec2 Vec2::operator+(float _value) const
{
    return { x + _value, y + _value };
}

void Vec2::operator=(const Vec2& _v)
{
    x = _v.x;
    y = _v.y;
}

void Vec2::operator+=(const Vec2& _v)
{
    *this = *this + _v;
}

void Vec2::operator-=(const Vec2& _v)
{
    *this = *this - _v;
}

void Vec2::operator*=(float _value)
{
    *this = *this * _value;
}

void Vec2::operator/=(float _value)
{
    *this = *this / _value;
}

bool Vec2::operator==(const Vec2& v) const
{
    float dx = x - v.x;
    float dy = y - v.y;
    return (dx * dx + dy * dy) < 0.000001f;
}

bool Vec2::operator!=(const Vec2& v) const
{
    return !operator==(v);
}