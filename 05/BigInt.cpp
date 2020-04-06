#include "BigInt.h"
#include "iostream"

BigInt::BigInt(long long value)
    : sign(false)
{
    if (value < 0)
    {
        sign = true;
        value = -value;
    }

    size = value > 0 ? std::to_string(value).length() : 0;
    data = new char[size];

    for (int i = 0; i < size; ++i)
    {
        data[i] = value % 10;
        value /= 10;
    }
}

BigInt::BigInt(const BigInt& copied)
    : sign(copied.sign)
    , size(copied.size)
    , data(new char[copied.size])
{
    std::copy(copied.data, copied.data + size, data);
}

BigInt::BigInt(BigInt&& moved)
    : sign(moved.sign)
    , size(moved.size)
    , data(moved.data)
{
    moved.sign = false;
    moved.size = 0;
    moved.data = nullptr;
}

BigInt::~BigInt()
{
    delete[] data;
}

BigInt& BigInt::operator=(const BigInt& copied)
{
    if (this == &copied)
        return *this;

    char* tmp = new char[copied.size];
    delete[] data;

    data = tmp;
    sign = copied.sign;
    size = copied.size;

    std::copy(copied.data, copied.data + size, data);

    return *this;
}

BigInt& BigInt::operator=(BigInt&& moved)
{
    if (this == &moved)
        return *this;

    delete[] data;

    data = moved.data;
    sign = moved.sign;
    size = moved.size;

    moved.data = nullptr;
    moved.sign = false;
    moved.size = 0;

    return *this;
}

void BigInt::updateSize(size_t newSize)
{
    char* tmp = new char[newSize];

    for (int i = 0; i < std::min(size, newSize); ++i)
        tmp[i] = data[i];

    delete[] data;
    data = tmp;
    size = newSize;
}

BigInt BigInt::operator-() const
{
    BigInt tmp(*this);
    tmp.sign = !sign;
    return tmp;
}

BigInt BigInt::operator+(const BigInt& other) const
{
    if (sign)
    {
        if (other.sign)
            return -(-*this + (-other));
        else
            return (other - (-*this));
    }
    else if (other.sign)
    {
        return (*this - (-other));
    }

    size_t maxSize = std::max(size, other.size);
    BigInt tmp;
    tmp.updateSize(maxSize);
    tmp.sign = sign;

    int a = 0, b = 0, carry = 0;

    for (int i = 0; i < maxSize; ++i)
    {
        a = i < size ? data[i] : 0;
        b = i < other.size ? other.data[i] : 0;

        int c = a + b + carry;
        carry = (c >= 10) ? 1 : 0;
        c %= 10;

        tmp.data[i] = c;
    }

    if (carry)
    {
        tmp.updateSize(maxSize + 1);
        tmp.data[maxSize] = carry;
    }

    if (tmp.data[tmp.size - 1] == 0)
        tmp.updateSize(tmp.size - 1);

    if (tmp.size == 0)
        tmp.sign = false;

    return tmp;
}

BigInt BigInt::operator-(const BigInt& other) const
{
    if (other.sign)
        return *this + (-other);
    else if (sign)
        return -(-*this + other);
    else if (*this < other)
        return -(other - *this);

    BigInt tmp;
    tmp.updateSize(size);
    tmp.sign = sign;

    int a = 0, b = 0, carry = 0;

    for (int i = 0; i < size; ++i)
    {
        a = data[i] + carry;
        b = i < other.size ? other.data[i] : 0;

        int c = a - b;
        carry = (c < 0) ? -1 : 0;
        if (c < 0)
            c += 10;

        tmp.data[i] = c;
    }

    while (tmp.size > 0 && (tmp.data[tmp.size - 1] == 0))
        --tmp.size;

    if (tmp.size < size)
        tmp.updateSize(tmp.size);

    if (tmp.size == 0)
        tmp.sign = false;

    return tmp;
}

bool BigInt::operator==(const BigInt& other) const
{
    if (this == &other)
        return true;

    if (size == 0 && other.size == 0)
        return true;

    if (size != other.size || sign != other.sign)
        return false;

    for (int i = 0; i < size; ++i)
        if (data[i] != other.data[i])
            return false;

    return true;
}

bool BigInt::operator!=(const BigInt& other) const
{
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const
{
    if (sign != other.sign)
        return sign;

    if (size != other.size)
        return size < other.size ? !sign : sign;

    for (int i = size - 1; i >= 0; --i)
        if (data[i] != other.data[i])
            return data[i] < other.data[i] ? !sign : sign;

    return false;
}

bool BigInt::operator<=(const BigInt& other) const
{
    return (*this < other) || (*this == other);
}

bool BigInt::operator>(const BigInt& other) const
{
    return !(*this <= other);
}

bool BigInt::operator>=(const BigInt& other) const
{
    return !(*this < other);
}


std::ostream& operator<<(std::ostream& stream, const BigInt& number)
{
    if (number.sign && number.size != 0)
        stream << '-';

    if (number.size == 0)
        stream << 0;

    for (int i = number.size - 1; i >= 0; --i)
        stream << (int) number.data[i];
    return stream;
}