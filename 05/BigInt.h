#pragma once
#include "stdlib.h"
#include "iostream"

class BigInt
{
public:
    BigInt(long long value = 0);
    BigInt(const BigInt&);
    BigInt(BigInt&&);
    ~BigInt();

    BigInt& operator=(const BigInt&);
    BigInt& operator=(BigInt&&);

    BigInt operator-() const;
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    friend std::ostream& operator<< (std::ostream&, const BigInt&);

private:
    char* data;
    bool sign;
    size_t size;

    void updateSize(size_t);
};