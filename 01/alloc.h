#pragma once
#include <cstdio>

void makeAllocator(size_t maxSize);
char* alloc(size_t size);
void reset();
void finalize();