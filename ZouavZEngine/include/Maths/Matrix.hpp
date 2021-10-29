#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

class Matrix
{
    public:
        int    nbRow;
        int    nbCol;
        float* matrix;

        Matrix(int _nbRow, int _nbCol);
        Matrix(int _nbRow, int _nbCol, float* values);
        Matrix(const Matrix& m);
        ~Matrix();

        float Accessor(int indexRow, int indexCol) const;
        float& Accessor(int indexRow, int indexCol);
        Matrix Transposed() const;
        void Transpose();

        Matrix operator+(const Matrix& m) const;
        Matrix operator-(const Matrix& m) const;
        Matrix operator*(const Matrix& m) const;

        Matrix GetSubMatrix(int excludedRowIndex, int excludedColIndex) const;
        Matrix Adjunct() const;
        void Reverse();
        Matrix Reversed() const;
        Matrix CoMat() const;
        float MatMinor(int indexRow, int indexCol) const;
        float Cof(int indexRow, int indexCol) const;
        float Det() const;
        void Display() const;

        static Matrix EqResolver(const Matrix&, const Matrix&);
        static Matrix Identity(int nbRowAndCol);
        static Matrix Zero(int _nbRow, int _nbCol);
};
#endif