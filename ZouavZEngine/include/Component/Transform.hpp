#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Quaternion.hpp"
#include <vector>

class Transform
{
protected:
    Vec3       worldPosition{ 0.0f, 0.0f, 0.0f };
    Quaternion worldRotation{ 1.0f, 0.0f, 0.0f, 0.0f };
    Vec3       worldScale{ 1.0f, 1.0f, 1.0f };

public:
    Vec3       localPosition{0.0f, 0.0f, 0.0f};
    Quaternion localRotation{1.0f, 0.0f, 0.0f, 0.0f};
    Vec3       localScale{1.0f, 1.0f, 1.0f};

    Transform() = default;
    Transform(const Vec3& _position, const Vec3& _rotation, const Vec3& _scale);
    Transform(const Vec3& _position, const Quaternion& _rotation, const Vec3& _scale);
    ~Transform() = default;

    static Transform InitTransform();

    void UpdateWorldPos(const Vec3&, const Quaternion&, const Vec3&);

    const Vec3& WorldPosition() const;
    const Quaternion& WorldRotation() const;
    const Vec3& WorldScale() const;

    Mat4 GetTRSMatrix() const;

    void TranslateX(float _x);
    void TranslateY(float _y);
    void TranslateZ(float _z);
    void Translate(float _x, float _y, float _z);
    void Translate(const Vec3& _direction);

    void RotateX(float _angle);
    void RotateY(float _angle);
    void RotateZ(float _angle);
    void Rotate(const Vec3& _angles);
    void Rotate(const Quaternion& _rotToAdd);

    void AddToScale(const Vec3& _toAdd);
    void MultiplyScaleBy(const Vec3& _coeffs);
    void MultiplyScaleBy(float _coeff);
    void MultiplyScaleXBy(float _coeff);
    void MultiplyScaleYBy(float _coeff);
    void MultiplyScaleZBy(float _coeff);

    Vec3 Right();
    Vec3 Up();
    Vec3 Forward();
};