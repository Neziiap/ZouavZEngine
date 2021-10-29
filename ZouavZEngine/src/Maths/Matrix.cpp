#include "Maths/Matrix.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

Matrix::Matrix(int _nbRow, int _nbCol)
    : nbRow{ _nbRow }, nbCol{ _nbCol }, matrix{ new float[_nbCol * _nbRow] }
{}

Matrix::Matrix(int _nbRow, int _nbCol, float* values)
    : nbRow{ _nbRow }, nbCol{ _nbCol }, matrix{ new float[nbRow * nbCol] }
{
    for (int i = 0; i < nbRow * nbCol; i++)
        matrix[i] = matrix[i];
}

Matrix::Matrix(const Matrix& m)
    : nbRow{ m.nbRow }, nbCol{ m.nbCol }, matrix{ new float[nbCol * nbRow] }
{
    for (int i = 0; i < nbRow * nbCol; i++)
        matrix[i] = m.matrix[i];
}

Matrix::~Matrix()
{
    delete[] matrix;
}

float& Matrix::Accessor(int indexRow, int indexCol)
{
    return matrix[indexRow + indexCol * nbRow];
}

float Matrix::Accessor(int indexRow, int indexCol) const
{
    return matrix[indexRow + indexCol * nbRow];
}

Matrix Matrix::Identity(int nbRowAndCol)
{
    Matrix newMatrix = Matrix::Zero( nbRowAndCol, nbRowAndCol );

    for (int i = 0; i < nbRowAndCol; i++)
        newMatrix.Accessor(i, i) = 1;            

    return newMatrix;
}

Matrix Matrix::Zero(int _nbRow, int _nbCol)
{
    Matrix newMatrix = Matrix( _nbRow, _nbCol );

    for (int i = 0; i < _nbRow; i++)
        for (int j = 0; j < _nbCol; j++)
            newMatrix.Accessor(i, j) = 0;

    return newMatrix;
}

Matrix Matrix::Transposed() const
{
    Matrix tempMatrix = Matrix(nbCol, nbRow);

    for (int i = 0; i < nbRow; i++)
        for (int j = 0; j < nbCol; j++)
            tempMatrix.Accessor(j, i) = Accessor(i, j);

    return tempMatrix;
}

void Matrix::Transpose()
{
    Matrix tempMatrix = Matrix(nbCol, nbRow);

    for (int i = 0; i < nbRow; i++)
        for (int j = 0; j < nbCol; j++)
            Accessor(j, i) = tempMatrix.Accessor(i, j);
}

Matrix Matrix::operator+(const Matrix& m) const
{
    assert(nbCol == m.nbCol && nbRow == m.nbRow);

    Matrix matrix(*this);

    for (int i = 0; i < matrix.nbRow; i++)
        for (int j = 0; j < matrix.nbCol; j++)
            matrix.Accessor(i, j) += m.Accessor(i, j);

    return matrix;
}

Matrix Matrix::operator-(const Matrix& m) const
{
    assert(nbCol == m.nbCol && nbRow == m.nbRow);

    Matrix matrix(*this);

    for (int i = 0; i < matrix.nbRow; i++)
        for (int j = 0; j < matrix.nbCol; j++)
            matrix.Accessor(i, j) -= m.Accessor(i, j);

    return matrix;
}

Matrix Matrix::operator*(const Matrix& m) const
{
    assert(nbCol == m.nbRow);

    Matrix result = Matrix::Zero( nbRow, m.nbCol );

    for (int i = 0; i < result.nbRow; i++)
        for (int j = 0; j < result.nbCol; j++)
            for (int k = 0; k < nbCol; k++)
                result.Accessor(i, j) += Accessor(i, k) * m.Accessor(k, j);

    return result;
}

void Matrix::Display() const
{
    for (int i = 0; i < nbRow; i++)
    {
        for (int j = 0; j < nbCol; j++)
            std::cout << matrix[i * nbCol + j];

        std::cout << "\n";
    }

    std::cout << "\n";
}

Matrix Matrix::GetSubMatrix(int excludedColIndex, int excludedRowIndex) const
{
    Matrix newMatrix( nbRow - 1, nbCol - 1 );
    bool colIndexPassed  = false;
    bool rowIndexPassed = false;

    for (int i = 0; i < nbRow; i++)
    {
        if (i == excludedRowIndex)
        {
            rowIndexPassed = true;
            continue;
        }

        for (int j = 0; j < nbCol; j++)
        {
            if (j == excludedColIndex)
            {
                colIndexPassed = true;
                continue;
            }

            newMatrix.Accessor(i - rowIndexPassed, j - colIndexPassed) = Accessor(i, j);
        }
        colIndexPassed = false;
    }

    return newMatrix;
}

float Matrix::Det() const
{
    assert(nbRow == nbCol);
        
    if (nbRow == 1)
        return matrix[0];

    if (nbRow == 2)
        return matrix[0] * matrix[3] - matrix[2] * matrix[1];
        
    if (nbRow == 3)
    {
        return matrix[0] * (matrix[4] * matrix[8] - matrix[7] * matrix[5]) 
            - matrix[1] * (matrix[3] * matrix[8] - matrix[6] * matrix[5]) 
            + matrix[2] * (matrix[3] * matrix[7] - matrix[6] * matrix[4]);
    }

    if (nbRow == 4)
    {
        return matrix[0] * (matrix[5] * (matrix[10] * matrix[15] - matrix[14] * matrix[11]) 
                         -  matrix[6] * (matrix[9]  * matrix[15] - matrix[13] * matrix[11]) 
                         +  matrix[7] * (matrix[9]  * matrix[14] - matrix[13] * matrix[10]))
             - matrix[1] * (matrix[4] * (matrix[10] * matrix[15] - matrix[14] * matrix[11])
                         -  matrix[6] * (matrix[8]  * matrix[15] - matrix[12] * matrix[11])
                         +  matrix[7] * (matrix[8]  * matrix[14] - matrix[12] * matrix[10]))
             + matrix[2] * (matrix[4] * (matrix[9]  * matrix[15] - matrix[13] * matrix[11])
                         -  matrix[5] * (matrix[8]  * matrix[15] - matrix[12] * matrix[11])
                         +  matrix[7] * (matrix[8]  * matrix[13] - matrix[12] * matrix[9]))
             - matrix[3] * (matrix[4] * (matrix[9]  * matrix[14] - matrix[13] * matrix[10])
                         -  matrix[5] * (matrix[8]  * matrix[14] - matrix[12] * matrix[10])
                         +  matrix[6] * (matrix[8]  * matrix[13] - matrix[12] * matrix[9]));
    }

    else 
    {
        float toReturn = 0;

        for (int i = 0; i < nbRow; i++)
            toReturn += GetSubMatrix(i, 0).Det() * matrix[i] * ((i % 2) ? -1 : 1);

        return toReturn;
    }
}

float Matrix::MatMinor(int indexRow, int indexCol) const
{
    return GetSubMatrix(indexCol, indexRow).Det();
}

float Matrix::Cof(int indexRow, int indexCol) const
{
    return pow(-1, indexRow + indexCol + 2) * MatMinor(indexRow, indexCol);
}

Matrix Matrix::CoMat() const
{
    assert(nbRow == nbCol);

    Matrix cofMat(nbRow, nbCol);

    for (int i = 0; i < nbRow; i++)
        for (int j = 0; j < nbCol; j++)
            cofMat.Accessor(i, j) = Cof(i, j);

    return cofMat;
}

Matrix Matrix::Adjunct() const
{
    assert(nbRow == nbCol);

    return CoMat().Transposed();
}

Matrix Matrix::Reversed() const
{
    assert(nbRow == nbCol || Det() != 0);

    Matrix reverse(Adjunct());
    float det = Det();

    for (int i = 0; i < nbRow; i++)
        for (int j = 0; j < nbCol; j++)
            reverse.Accessor(i, j) /= det;

    return reverse;
}

void Matrix::Reverse()
{
    assert(nbRow == nbCol || Det() != 0);

    Matrix reverse(Adjunct());
    float det = Det();

    for (int i = 0; i < nbRow; i++)
        for (int j = 0; j < nbCol; j++)
            reverse.Accessor(i, j) /= det;

    *this = reverse;
}

Matrix Matrix::EqResolver(const Matrix& equation, const Matrix& results)
{
    return equation.Reversed() * results;
}