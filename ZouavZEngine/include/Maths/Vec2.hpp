#ifndef _VEC2_HPP_
#define _VEC2_HPP_

#include <string>

class Vec2
{
public:
    float x;
    float y;

    const static Vec2 zero;
    const static Vec2 one;
    const static Vec2 up;
    const static Vec2 down;
    const static Vec2 right;
    const static Vec2 left;

    Vec2();
    Vec2(float _x, float _y);
    Vec2(const Vec2& _v);
    ~Vec2() = default;

    float DotProduct(const Vec2& _v);
    float GetMagnitude() const;
    float GetSquaredMagnitude() const;
    Vec2 Normalized() const;
    float Cross(const Vec2& _v) const;

    void Normalize();

    std::string ToString() const;

    Vec2 operator+(const Vec2& _v) const;
    Vec2 operator-(const Vec2& _v) const;  
    Vec2 operator-() const;
    Vec2 operator*(float _value) const;
    Vec2 operator/(float _value) const;
    Vec2 operator+(float _value) const;

    void operator=(const Vec2& _v);
    void operator+=(const Vec2& _v);
    void operator-=(const Vec2& _v);
    void operator*=(float _value);
    void operator/=(float _value);

    bool operator==(const Vec2& v) const;
    bool operator!=(const Vec2& v) const;

    template <class Archive>
    void serialize(Archive& _ar)
    {
        _ar(x, y);
    }
};

#endif