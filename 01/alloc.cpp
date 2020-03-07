#include "alloc.h"
#include "iostream"

static char* pointer = nullptr;
static size_t usedMemory = 0;
static size_t allMemory = 0;

void makeAllocator(size_t maxSize)
{
    pointer = new char[maxSize];
    allMemory = maxSize;
}

char* alloc(size_t size)
{
    if (usedMemory + size > allMemory)
        return nullptr;

    char* result = pointer + usedMemory;
    usedMemory += size;

    return result;
}

void reset()
{
    usedMemory = 0;
}

void finalize()
{
    usedMemory = 0;
    allMemory = 0;
    delete[] pointer;
}