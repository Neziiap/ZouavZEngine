#define _USE_MATH_DEFINES
#include <cmath>
#include "Maths/Quaternion.hpp"
using namespace std;

Quaternion::Quaternion()
{
	w = 1.0f;
	x = y = z = 0.0f;
}

Quaternion::Quaternion(const Vec3& _eulerAngles)
{
	Vec3 angles = _eulerAngles * 0.5f * (M_PI / 180.0f);

	double cy = cos(angles.z);
	double sy = sin(angles.z);
	double cp = cos(angles.y);
	double sp = sin(angles.y);
	double cr = cos(angles.x);
	double sr = sin(angles.x);
										 
	w = cr * cp * cy + sr * sp * sy;
	x = sr * cp * cy - cr * sp * sy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
}

Quaternion::Quaternion(const Vec4& _v)
{
	w = _v.w;
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

Quaternion::Quaternion(const Quaternion& _q)
{
	w = _q.w;
	x = _q.x;
	y = _q.y;
	z = _q.z;
}

Quaternion::Quaternion(const Vec3& _v, float _f)
{
	w = _f;
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z)
{
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

Quaternion Quaternion::AngleAxis(const Vec3& _v, float _f)
{
	Quaternion toReturn;

	toReturn.x = _v.x * sinf(_f / 2.f);
	toReturn.y = _v.y * sinf(_f / 2.f);
	toReturn.z = _v.z * sinf(_f / 2.f);
	toReturn.w = cosf(_f / 2.f);

	return toReturn;
}

void Quaternion::Mult(const Quaternion& _q2)
{
	float tempw = w;
	float tempx = x;
	float tempy = y;
	float tempz = z;

	w = -tempx * _q2.x - tempy * _q2.y - tempz * _q2.z + tempw * _q2.w;
	x = tempx * _q2.w + tempy * _q2.z - tempz * _q2.y + tempw * _q2.x;
	y = -tempx * _q2.z + tempy * _q2.w + tempz * _q2.x + tempw * _q2.y;
	z = tempx * _q2.y - tempy * _q2.x + tempz * _q2.w + tempw * _q2.z;
}

Quaternion Quaternion::Mult(const Quaternion& _q2) const
{
	return Quaternion(	-x * _q2.x - y * _q2.y - z * _q2.z + w * _q2.w,
						x * _q2.w + y * _q2.z - z * _q2.y + w * _q2.x,
						-x * _q2.z + y * _q2.w + z * _q2.x + w * _q2.y,
						x * _q2.y - y * _q2.x + z * _q2.w + w * _q2.z);
}

void Quaternion::Add(const Quaternion& _q2)
{
	x = x + _q2.x;
	y = y + _q2.y;
	z = z + _q2.z;
	w = w + _q2.w;
}

Quaternion Quaternion::Add(const Quaternion& _q2) const
{
	return Quaternion(w + _q2.w, x + _q2.x, y + _q2.y, z + _q2.z);
}

float Quaternion::DotProduct(const Quaternion& _q1, const Quaternion& _q2)
{
    return _q1.w * _q2.w + _q1.x * _q2.x + _q1.y * _q2.y + _q1.z * _q2.z;
}

void Quaternion::Neg(const Quaternion& _q2)
{
	x = x - _q2.x;
	y = y - _q2.y;
	z = z - _q2.z;
	w = w - _q2.w;
}

Quaternion Quaternion::Neg(const Quaternion& _q2) const
{
	return Quaternion(w - _q2.w, x - _q2.x, y - _q2.y, z - _q2.z);
}

void Quaternion::Scale(float s)
{
	w *= s;
	x *= s;
	y *= s;
	z *= s;
}

const Quaternion Quaternion::Scale(float _s) const
{
	return Quaternion(w * _s, x * _s, y * _s, z * _s);
}

void Quaternion::Normalise()
{
	float u = sqrtf(x * x + y * y + z * z + w * w);
	x /= u;
	y /= u;
	z /= u;
	w /= u;
}

Quaternion Quaternion::Normalised() const
{
	float u = sqrtf(x * x + y * y + z * z + w * w);
	return Quaternion(w / u, x / u, y / u,	z / u);
}

void Quaternion::Conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

const Quaternion Quaternion::Conjugated() const
{
	return Quaternion(w, -x, -y, -z);
}

float Quaternion::Norm() const
{
	return w * w + x * x + y * y + z * z;
}

float Quaternion::Length() const
{
	return sqrtf(Norm());
}

float Quaternion::SquaredLength() const
{
	return Norm();
}

void Quaternion::Inverse()
{
	Conjugate();
	Scale(1 / Norm());
}

Quaternion Quaternion::Inversed() const
{
	return Conjugated().Scale(1 / Norm());
}

float Quaternion::GetAngle() const
{
    return acosf(w) * 2.f;
}

Vec3 Quaternion::GetAxis() const
{
    const Vec3 xyz(x, y, z);
    return  xyz / sinf(GetAngle() / 2.f);
}

Vec3 Quaternion::ToEuler() const
{
	Vec3 angles;

	float sinr_cosp = 2.0f * (w * x + y * z);
	float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
	angles.x = std::atan2f(sinr_cosp, cosr_cosp);

	float sinp = 2.0f * (w * y - z * x);
	if (std::abs(sinp) >= 1.0f)
		angles.y = std::copysignf(M_PI / 2, sinp); 
	else
		angles.y = std::asinf(sinp);

	float siny_cosp = 2.0f * (w * z + x * y);
	float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
	angles.z = std::atan2f(siny_cosp, cosy_cosp);

	return angles * (180.0f / M_PI);
}

Vec3 Quaternion::RotateVector(const Vec3& _vec) const
{
    Vec3 u(x, y, z);

	float s = w;

	return 2.0f * u.Dot(_vec) * u + (s * s - u.Dot(u)) * _vec + 2.0f * s * u.Cross(_vec);
}

Quaternion Quaternion::RotateFromTo(const Vec3& _vec1, const Vec3& _vec2)
{
	float dot = _vec1.Dot(_vec2);
	Vec3 temp;
	if (dot < -0.999999f)
	{
		temp = Vec3::right.Cross(_vec1);
		if (temp.GetMagnitude() < 0.000001f)
			temp = Vec3::up.Cross(_vec1);
		temp.Normalize();
		return Quaternion::AngleAxis(temp, M_PI);
	}
	else if (dot > 0.999999f) 
		return Quaternion();
	else 
	{
		temp = _vec1.Cross(_vec2);
		return Quaternion(1 + dot, temp.x, temp.y, temp.z).Normalised();
	}
}

Mat4 Quaternion::GetRotationMatrix() const
{
    const float twoXX = 2.f * x * x;
    const float twoXY = 2.f * x * y;
    const float twoXZ = 2.f * x * z;
    const float twoXW = 2.f * x * w;
    const float twoYY = 2.f * y * y;
    const float twoYZ = 2.f * y * z;
    const float twoYW = 2.f * y * w;
    const float twoZZ = 2.f * z * z;
    const float twoZW = 2.f * z * w;

    const Vec3 vec1(1.f - twoYY - twoZZ, twoXY + twoZW, twoXZ - twoYW);
    const Vec3 vec2(twoXY - twoZW, 1.f - twoXX - twoZZ, twoYZ + twoXW);
    const Vec3 vec3(twoXZ + twoYW, twoYZ - twoXW, 1.f - twoXX - twoYY);

    Mat4 rst({	vec1.x, vec1.y, vec1.z, 0.0f,
				vec2.x, vec2.y, vec2.z, 0.0f,
				vec3.x, vec3.y, vec3.z, 0.0f,
				0.0f,	0.0f,	0.0f,	1.0f});

    return rst /*/ SquaredLength()*/;
}

Quaternion Quaternion::Lerp(const Quaternion& _q1, const Quaternion& _q2, float _t)
{
    return _q1 * (1.0f - _t) + _q2 * _t;
}

Quaternion Quaternion::SLerp(const Quaternion& _q1, const Quaternion& _q2, float _t)
{
	float dot = DotProduct(_q1, _q2);

    Quaternion tempQ1 = _q1;

    if (dot < 0.f)
        tempQ1 = -tempQ1;

	if (abs(dot) >= 1.f)
		return _q2;

    float omega = acosf(abs(dot));

	if (abs(sinf(omega)) < 0.00001f)
		return _q2;
	
	Quaternion toreturn = (tempQ1 * sinf((1.f - _t) * omega) + _q2 * sinf(_t * omega)) / sinf(omega);

	//std::cout << dot << "	" << abs(dot) << " " << omega << " " << sinf(omega) << " " << toreturn.ToString() << std::endl;

    return toreturn;
}

Quaternion Quaternion::NLerp(const Quaternion& _q1, const Quaternion& _q2, float _t)
{
    return Quaternion::Lerp(_q1, _q2, _t).Normalised();
}

string Quaternion::ToString() const
{
	return "[ " + to_string(w) + " ; " + to_string(x) + " ; " + to_string(y) + " ; " + to_string(z) + " ]";
}