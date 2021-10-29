#pragma once

#include <string>
#include "Maths/Vec3.hpp"
#include "Maths/Vec4.hpp"
#include "Maths/Mat4.hpp"


class Quaternion
{
public:

	float w = 1.f;
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	Quaternion();
	Quaternion(const Vec3& _eulerAngles);
	Quaternion(const Vec4& _v);
	Quaternion(const Quaternion& _q);
	Quaternion(const Vec3& _v, float _f);
	Quaternion(float _w, float _x, float _y, float _z);

	static Quaternion AngleAxis(const Vec3& _v, float _f);

	void Mult(const Quaternion& _q2);
	Quaternion Mult(const Quaternion& _q2) const;

	static float DotProduct(const Quaternion& _q1, const Quaternion& _q2);

	void Add(const Quaternion& _q2);
	Quaternion Add(const Quaternion& _q2) const;

	void Neg(const Quaternion& _q2);
	Quaternion Neg(const Quaternion& _q2) const;

	void Scale(float);
	const Quaternion Scale(float) const;

	void Normalise();
	Quaternion Normalised() const;

	void Conjugate();
	const Quaternion Conjugated() const;

	float GetAngle() const;

	Vec3 GetAxis() const;

	Vec3 ToEuler() const;
	 
	Vec3 RotateVector(const Vec3& _vec) const;

	static Quaternion RotateFromTo(const Vec3&, const Vec3&);

	Mat4 GetRotationMatrix() const;

	float Norm() const;

	float Length() const;
	float SquaredLength() const;

	void Inverse();
	Quaternion Inversed() const;

    static Quaternion Lerp(const Quaternion& _q1, const Quaternion& _q2, float _t);
    static Quaternion SLerp(const Quaternion& _q1, const Quaternion& _q2, float _t);
    static Quaternion NLerp(const Quaternion& _q1, const Quaternion& _q2, float _t);

	std::string ToString() const;


	Quaternion operator*(const Quaternion _q2) const
	{
		return Mult(_q2);
	}
	Quaternion operator+(const Quaternion& _q2) const
	{
		return Add(_q2);
	}
	Quaternion operator*(const float _f) const
	{
		return Scale(_f);
	}
	Quaternion operator/(const float _f) const
	{
		return Quaternion(w / _f, x / _f, y / _f, z / _f);
	}
	Quaternion operator-() const
	{
		return Quaternion(-w, -x, -y, -z);
	}


	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(w, x, y, z);
	}
};

