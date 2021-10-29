#include "Maths/Vec3.hpp"
#include "Maths/Quaternion.hpp"
#include <math.h>
#include <cassert>


#include "Maths/Mat4.hpp"

const Mat4 Mat4::identity = Matrix::Identity(4);
const Mat4 Mat4::zero = Matrix::Zero(4, 4);

Mat4::Mat4() 
    : Matrix(4, 4)
{
    for (int i = 0; i < 16; i++)
        matrix[i] = 0;        
}

Mat4::Mat4(std::initializer_list<float> matrix)
    : Matrix(4, 4)
{
    assert(matrix.size() == 16);

    int i = 0;
    for (float f : matrix)
        this->matrix[i++] = f;
}

Mat4::Mat4(const Matrix& m) 
    : Matrix(m)
{}

Mat4 Mat4::CreateScaleMatrix(const Vec3& scale)
{
    Mat4 scaleMat;

    scaleMat.Accessor(0, 0) = scale.x;
    scaleMat.Accessor(1, 1) = scale.y;
    scaleMat.Accessor(2, 2) = scale.z;
    scaleMat.Accessor(3, 3) = 1;

    return scaleMat;
}

Mat4 Mat4::CreateTranslationMatrix(const Vec3& translation)
{
    Mat4 translationMat = Mat4::identity;

    translationMat.matrix[12] = translation.x;
    translationMat.matrix[13] = translation.y;
    translationMat.matrix[14] = translation.z;

    return translationMat;
}

Mat4 Mat4::CreateXRotationMatrix(float angle)
{
    Mat4 rotationMat;

    rotationMat.Accessor(0, 0) = 1;
    rotationMat.Accessor(3, 3) = 1;

    rotationMat.Accessor(1, 1) = cosf(angle);
    rotationMat.Accessor(1, 2) = -sinf(angle);
    rotationMat.Accessor(2, 1) = sinf(angle);
    rotationMat.Accessor(2, 2) = cosf(angle);

    return rotationMat;    
}

Mat4 Mat4::CreateYRotationMatrix(float angle)
{
    Mat4 rotationMat;

    rotationMat.Accessor(1, 1) = 1;
    rotationMat.Accessor(3, 3) = 1;

    rotationMat.Accessor(0, 0) = cosf(angle);
    rotationMat.Accessor(0, 2) = sinf(angle);
    rotationMat.Accessor(2, 0) = -sinf(angle);
    rotationMat.Accessor(2, 2) = cosf(angle);

    return rotationMat;    
}

Mat4 Mat4::CreateZRotationMatrix(float angle)
{
    Mat4 rotationMat;

    rotationMat.Accessor(2, 2) = 1;
    rotationMat.Accessor(3, 3) = 1;

    rotationMat.Accessor(0, 0) = cosf(angle);
    rotationMat.Accessor(0, 1) = -sinf(angle);
    rotationMat.Accessor(1, 0) = sinf(angle);
    rotationMat.Accessor(1, 1) = cosf(angle);

    return rotationMat;    
}

Mat4 Mat4::CreateFixedAngleEulerRotationMatrix(const Vec3& angle)
{
    return Mat4::CreateYRotationMatrix(angle.y) * 
        Mat4::CreateXRotationMatrix(angle.x) * 
        Mat4::CreateZRotationMatrix(angle.z);    
}

Mat4 Mat4::CreateTRSMatrix(const Vec3& translation, const Vec3& rotate, const Vec3& scale)
{
    return Mat4::CreateTranslationMatrix(translation) * 
        Mat4::CreateFixedAngleEulerRotationMatrix(rotate) * 
        Mat4::CreateScaleMatrix(scale);    
}

Mat4 Mat4::CreateTRSMatrix(const Vec3& translation, const Quaternion& rotate, const Vec3& scale)
{
    return Mat4::CreateTranslationMatrix(translation) *
        rotate.GetRotationMatrix() *
        Mat4::CreateScaleMatrix(scale);
}

Mat4 Mat4::CreateUnitVecRotMatrix(const Vec3& v, float angle)
{
    Mat4 rotationMat = identity;

    float c = cosf(angle);
    float s = sinf(angle);
    float t = 1 - cosf(angle);

    rotationMat.Accessor(0, 0) = (t * v.x) * (t * v.x) + c;
    rotationMat.Accessor(1, 1) = (t * v.y) * (t * v.y) + c;
    rotationMat.Accessor(2, 2) = (t * v.z) * (t * v.z) + c;

    rotationMat.Accessor(0, 1) = t * v.x * v.y - s * v.z;
    rotationMat.Accessor(1, 2) = t * v.y * v.z - s * v.x;
    rotationMat.Accessor(2, 0) = t * v.z * v.x - s * v.y;
        
    rotationMat.Accessor(0, 2) = t * v.x * v.z + s * v.y;
    rotationMat.Accessor(1, 0) = t * v.x * v.y + s * v.z;
    rotationMat.Accessor(2, 1) = t * v.y * v.z + s * v.x;

    return rotationMat;
}

Mat4 Mat4::CreateProjMatrix(float d)
{
    Mat4 projectionMat;

    projectionMat.Accessor(0, 0) = 1;
    projectionMat.Accessor(1, 1) = 1;
    projectionMat.Accessor(2, 2) = 1;
    projectionMat.Accessor(3, 2) = 1 / d;

    return projectionMat;
}

Mat4 Mat4::CreatePerspectiveProjectionMatrix(float width, float height, float near, float far, float fov)
{
    Mat4 perspective;
    float temp, temp2, temp3, temp4;
    {
        float ymax, xmax;
        ymax = near * tanf(fov * 3.1415926f / 360.0f);
        xmax = ymax * (width / height);
        temp = 2 * near;
        temp2 = 2 * xmax;
        temp3 = 2 * ymax;
    }
    temp4 = far - near;
    perspective.matrix[0]  = temp / temp2;
    perspective.matrix[5]  = temp / temp3;
    perspective.matrix[10] = (-far - near) / temp4;
    perspective.matrix[11] = -1.0;
    perspective.matrix[14] = (-temp * far) / temp4;

    return perspective;
}

Mat4 Mat4::CreateOrthographicProjectionMatrix(float width, float height, float near, float far)
{
    Mat4 orthographic = identity;
        
    float aspect = (float)width / height;

    orthographic.matrix[0]  = 1.f;
    orthographic.matrix[5]  = 1.f / aspect;
    orthographic.matrix[10] = -2.f / (far - near);
    orthographic.matrix[12] = -1.f;
    orthographic.matrix[13] = -1.f;
    orthographic.matrix[14] = -(far + near) / (far - near);

    return orthographic;
}

Vec3 Mat4::operator*(const Vec3& _v) const
{
    return {matrix[0] * _v.x + matrix[4] * _v.y + matrix[8] * _v.z + matrix[12],
            matrix[1] * _v.x + matrix[5] * _v.y + matrix[9] * _v.z + matrix[13],
            matrix[2] * _v.x + matrix[6] * _v.y + matrix[10] * _v.z + matrix[14]};
}

Vec4 Mat4::operator*(const Vec4& _v) const
{
    return {matrix[0] * _v.x + matrix[4] * _v.y + matrix[8] * _v.z + matrix[12] * _v.w,
            matrix[1] * _v.x + matrix[5] * _v.y + matrix[9] * _v.z + matrix[13] * _v.w,
            matrix[2] * _v.x + matrix[6] * _v.y + matrix[10] * _v.z + matrix[14] * _v.w,
            matrix[3] * _v.x + matrix[7] * _v.y + matrix[11] * _v.z + matrix[15] * _v.w };
}

Mat4 Mat4::ConvertAssimpMatrixToMat4(aiMatrix4x4t<ai_real> matrix)
{
    Mat4 r;
    memcpy(r.matrix, &matrix.a1, sizeof(aiMatrix4x4t<ai_real>));
    return r.Transposed();
}

Mat4 Mat4::operator*(float _value) const
{
    Mat4 result = *this;

    for (int i = 0; i < 16; i++)
        result.matrix[i] *= _value;

    return result;
}

Mat4 Mat4::operator/(float scalar) const
{
    Mat4 rst = *this;
    for (size_t i = 0; i < 16; i++)
    {
        rst.matrix[i] /= scalar;
    }
    return rst;
}

void Mat4::operator=(const Mat4& m)
{
    for (int i = 0; i < 16; i++)
    {
        this->matrix[i] = m.matrix[i];
    }
}

void Mat4::operator+=(const Mat4& _m)
{
    *this = *this + _m;
}

void Mat4::operator-=(const Mat4& _m)
{
    *this = *this - _m;
}

void Mat4::operator*=(const Mat4& _m)
{
    *this = *this * _m;
}

void Mat4::operator*=(float _value)
{
    *this = *this * _value;
}

void Mat4::operator/=(float _value)
{
    *this = *this / _value;
}