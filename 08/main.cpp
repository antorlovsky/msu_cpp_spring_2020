#include "ThreadPool.h"
#include <iostream>

int test(int x)
{
    return x + 10;
}

int main()
{
    ThreadPool pool(8);

    for (int i = 0; i < 8; ++i)
        std::cout << pool.exec([i]() { return i; }).get() << ' ';

    auto task = pool.exec(test, 1);
    std::cout << std::endl << task.get();

    return 0;
}
