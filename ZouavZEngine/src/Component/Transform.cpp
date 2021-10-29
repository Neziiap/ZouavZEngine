#include "Component/Transform.hpp"

Transform::Transform(const Vec3& _position, const Vec3& _rotation, const Vec3& _scale)
	 : localPosition(_position), localRotation(_rotation), localScale(_scale)
{
}

Transform::Transform(const Vec3& _position, const Quaternion& _rotation, const Vec3& _scale)
	: localPosition(_position), localRotation(_rotation), localScale(_scale)
{
}

Transform Transform::InitTransform()
{
	return Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
}

void Transform::UpdateWorldPos(const Vec3& _position, const Quaternion& _rotation, const Vec3& _scale)
{
	worldPosition = Mat4::CreateTRSMatrix(_position, _rotation, _scale) * localPosition;
	worldRotation = _rotation * localRotation;
	worldScale = _scale * localScale;
}

const Vec3& Transform::WorldPosition() const
{
	return worldPosition;
}

const Quaternion& Transform::WorldRotation() const
{
	return worldRotation;
}

const Vec3& Transform::WorldScale() const
{
	return worldScale;
}

Mat4 Transform::GetTRSMatrix() const
{
	return Mat4::CreateTRSMatrix(WorldPosition(), WorldRotation(), WorldScale());
}

void Transform::TranslateX(float _x)
{
	localPosition.x += _x;
}

void Transform::TranslateY(float _y)
{
	localPosition.y += _y;
}

void Transform::TranslateZ(float _z)
{
	localPosition.z += _z;
}

void Transform::Translate(float _x, float _y, float _z)
{
	localPosition += {_x, _y, _z};
}

void Transform::Translate(const Vec3& _direction)
{
	localPosition += _direction;
}

void Transform::RotateX(float _angle)
{
	localRotation = localRotation * Quaternion(Vec3{ _angle, 0.0f, 0.0f });
}

void Transform::RotateY(float _angle)
{
	localRotation = localRotation * Quaternion(Vec3{ 0.0f, _angle, 0.0f });
}

void Transform::RotateZ(float _angle)
{
	localRotation = localRotation * Quaternion(Vec3{ 0.0f, 0.0f, _angle });
}

void Transform::Rotate(const Vec3& _angles)
{
	localRotation = localRotation * Quaternion(_angles);
}

void Transform::Rotate(const Quaternion& _rotToAdd)
{
	localRotation = localRotation * _rotToAdd;
}

void Transform::AddToScale(const Vec3& _toAdd)
{
	localScale += _toAdd;
}

void Transform::MultiplyScaleBy(const Vec3& _coeffs)
{
	localScale *= _coeffs;
}

void Transform::MultiplyScaleBy(float _coeff)
{
	localScale *= _coeff;
}

void Transform::MultiplyScaleXBy(float _coeff)
{
	localScale.x *= _coeff;
}

void Transform::MultiplyScaleYBy(float _coeff)
{
	localScale.y *= _coeff;
}

void Transform::MultiplyScaleZBy(float _coeff)
{
	localScale.z *= _coeff;
}

Vec3 Transform::Right()
{
	return localRotation.RotateVector(Vec3::right);
}

Vec3 Transform::Up()
{
	return localRotation.RotateVector(Vec3::up);
}

Vec3 Transform::Forward()
{
	return localRotation.RotateVector(Vec3::forward);
}