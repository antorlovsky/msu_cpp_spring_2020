#include "Matrix.h"
#include "iostream"

void testEmpty()
{
    const size_t rows = 0;
    const size_t cols = 0;

    Matrix m(rows, cols);

    assert(m.getRows() == 0);
    assert(m.getColumns() == 0);
}

void testOutRange()
{
    const size_t rows = 2;
    const size_t cols = 3;

    Matrix m(rows, cols);

    try
    {
        double x = m[3][0];
    }
    catch(std::out_of_range e)
    {
        assert(e.what() == std::string("row"));
    }
}

void testCreate()
{
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);

    assert(m.getRows() == 5);
    assert(m.getColumns() == 3);
}

void testFill()
{
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            m[i][j] = 1;

    m[1][2] = 5;

    double x = m[4][1];

    assert(m[4][2] == 1);
    assert(m[1][2] == 5);
    assert(x == 1);
}

void testMult()
{
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            m[i][j] = 1;

    m[1][2] = 5;

    m *= 3;

    assert(m[3][2] == 3);
    assert(m[1][2] == 15);
}

void testEqual()
{
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);
    Matrix m1(rows, cols);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            m[i][j] = 1;
            m1[i][j] = 1;
        }

    assert(m == m1);
}

void testInequal()
{
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);
    Matrix m1(rows, cols);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            m[i][j] = 1;
            m1[i][j] = 1;
        }

    m[1][2] = 5;

    assert(m != m1);
}

int main()
{
    testEmpty();
    testOutRange();
    testCreate();
    testFill();
    testMult();
    testEqual();
    testInequal();

    return 0;
}