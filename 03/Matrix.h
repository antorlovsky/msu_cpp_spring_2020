#pragma once
#include "stdlib.h"

class Matrix
{
public:
    class Row
    {
    public:
        Row(const size_t);
        ~Row();
        const size_t colNum;

        int& operator[](const size_t);
        const int& operator[](const size_t) const;
        int* cols;
    };

    Matrix(const size_t, const size_t);
    ~Matrix();
    const size_t getRows() const;
    const size_t getColumns() const;

    Row& operator[](const size_t);
    const Row& operator[](const size_t) const;
    Matrix& operator*=(const int);
    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix&) const;
private:
    const size_t rowNum;
    Row** rows;
};