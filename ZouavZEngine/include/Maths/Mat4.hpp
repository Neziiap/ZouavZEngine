#ifndef _MAT4_HPP_
#define _MAT4_HPP_

#include "Matrix.hpp"
#include <initializer_list>
#include <assimp/matrix4x4.h>

class Vec3;
class Vec4;
class Quaternion;

class Mat4 : public Matrix
{
    public:
        Mat4();
        Mat4(std::initializer_list<float> matrix);
        Mat4(const Matrix& m);
        ~Mat4() = default;

        static const Mat4 identity;
        static const Mat4 zero;

        static Mat4 CreateScaleMatrix(const Vec3& v);
        static Mat4 CreateTranslationMatrix(const Vec3& v);
        static Mat4 CreateXRotationMatrix(float angle);
        static Mat4 CreateYRotationMatrix(float angle);
        static Mat4 CreateZRotationMatrix(float angle);
        static Mat4 CreateFixedAngleEulerRotationMatrix(const Vec3& v);
        static Mat4 CreateTRSMatrix(const Vec3& translation, const Vec3& rotation, const Vec3& scale);
        static Mat4 CreateTRSMatrix(const Vec3& translation, const Quaternion& rotate, const Vec3& scale);
        
        static Mat4 CreateUnitVecRotMatrix(const Vec3& v, float anle);
        static Mat4 CreateProjMatrix(float d);
        static Mat4 CreateOrthographicProjectionMatrix(float width, float height, float near, float far);
        static Mat4 CreatePerspectiveProjectionMatrix(float width, float height, float near, float far, float fov);
        
        static Mat4 ConvertAssimpMatrixToMat4(aiMatrix4x4t<ai_real> _matrix);

        Vec3 operator*(const Vec3& _v) const;
        Vec4 operator*(const Vec4& _v) const;
        inline Mat4 operator*(const Mat4& _m) const
        {
            Mat4 result = zero;

            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 16; j += 4)
                    for (int k = 0; k < 4; ++k)
                        result.matrix[i + j] += matrix[i + k * 4] * _m.matrix[k + j];

            return result;
        }
        Mat4 operator*(float _value) const;
        Mat4 operator/(float _value) const;

        void operator=(const Mat4& _m);
        void operator+=(const Mat4& _m);
        void operator-=(const Mat4& _m);
        void operator*=(const Mat4& _m);
        void operator*=(float _value);
        void operator/=(float _value);
};

#endif