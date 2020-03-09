#include "alloc.h"
#include "iostream"

void testZero()
{
    makeAllocator(0);
    char* test0 = alloc(0);

    assert(test0 != nullptr);
    finalize();
}

void testUsual()
{
    makeAllocator(8);
    char* test0 = alloc(1);
    char* test1 = alloc(5);

    assert(test0 != nullptr && test1 != nullptr && (test1 - test0) == 1);
    finalize();
}

void testFull()
{
    makeAllocator(5);
    char* test0 = alloc(2);
    char* test1 = alloc(3);

    assert(test0 != nullptr && test1 != nullptr && (test1 - test0) == 2);
    finalize();
}

void testStartOut()
{
    makeAllocator(18);
    char* test0 = alloc(20);

    assert(test0 == nullptr);
    finalize();
}

void testRunOut()
{
    makeAllocator(6);
    char* test0 = alloc(3);
    char* test1 = alloc(3);
    char* test2 = alloc(1);

    assert(test0 != nullptr && test1 != nullptr && test2 == nullptr && (test1 - test0) == 3);
    finalize();
}

void testReset()
{
    makeAllocator(12);
    char* test0 = alloc(8);
    char* test1 = alloc(5);

    assert(test0 != nullptr && test1 == nullptr);

    reset();
    test1 = alloc(5);

    assert(test1 != nullptr);
    finalize();
}

void testValue()
{
    makeAllocator(6);
    char* test0 = alloc(5);

    for (int i = 0; i < 5; ++i)
        test0[i] = i + '0';

    std::string s(test0, test0 + 5);
    assert(test0 != nullptr && s == "01234");
    finalize();
}

void testError()
{
    makeAllocator(-1);
    char* test0 = alloc(5);

    assert(test0 == nullptr);
}

int main()
{
    testZero();
    testUsual();
    testFull();
    testStartOut();
    testRunOut();
    testReset();
    testValue();
    testError();

    return 0;
}