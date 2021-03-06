#include "Matrix.h"
#include "iostream"

Matrix::Matrix(const size_t row, const size_t col) : rowNum(row)
{
    rows = new Matrix::Row* [rowNum];

    for (int i = 0; i < rowNum; ++i)
        rows[i] = new Matrix::Row(col);
}

Matrix::~Matrix()
{
    for (int i = 0; i < rowNum; ++i)
        delete rows[i];
    delete[] rows;
}

Matrix::Row::Row(const size_t col) : colNum(col)
{
    cols = new int[colNum];
}

Matrix::Row::~Row()
{
    delete[] cols;
}

const size_t Matrix::getRows() const
{
    return rowNum;
}

const size_t Matrix::getColumns() const
{
    if (rowNum == 0)
        return 0;
    return rows[0]->colNum;
}

Matrix::Row& Matrix::operator[](size_t row)
{
    if (row < rowNum)
        return *rows[row];
    else
        throw std::out_of_range("row");
}

const Matrix::Row& Matrix::operator[](size_t row) const
{
    if (row < rowNum)
        return *rows[row];
    else
        throw std::out_of_range("row");
}

int& Matrix::Row::operator[](const size_t col)
{
    if (col < colNum)
        return cols[col];
    else
        throw std::out_of_range("col");
}

const int& Matrix::Row::operator[](const size_t col) const
{
    if (col < colNum)
        return cols[col];
    else
        throw std::out_of_range("col");
}

Matrix& Matrix::operator*=(const int number)
{
    for (int i = 0; i < rowNum; ++i)
        for (int j = 0; j < getColumns(); ++j)
            rows[i]->cols[j] *= number;

    Matrix& res = *this;
    return res;
}

bool Matrix::operator==(const Matrix& other) const
{
    bool res;
    const size_t colNum = getColumns();

    if (rowNum != other.rowNum || colNum != other.getColumns())
        return false;

    for (int i = 0; i < rowNum; ++i)
    {
        for (int j = 0; j < colNum; ++j)
        {
            if (rows[i]->cols[j] != other.rows[i]->cols[j])
                return false;
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix& other) const
{
    return !(*this == other);
}