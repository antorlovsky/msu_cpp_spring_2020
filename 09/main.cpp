#include <iostream>
#include <cstdlib>
#include "sort.h"

void createInputFile()
{
    int n = 0x800000;
    std::ofstream input("input.txt", std::ios::out | std::ios::binary);
    for (int i = 0; i < n; ++i)
    {
        uint64_t a = rand();
        input.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
    }
    input.close();
}

void sortFile()
{
    BinarySort binSort("input.txt", 2);
}

void testSorting()
{
    std::ifstream output("output.txt", std::ios::in | std::ios::binary);

    int n = 0;
    uint64_t prev = 0, next = 0;

    while(!output.read(reinterpret_cast<char*>(&next), sizeof(uint64_t)).eof())
    {
        ++n;
        if (prev > next)
        {
            std::cout << prev << " > " << next << std::endl;
            output.close();
            return;
        }
    }

    output.close();
    std::cout << "Correct sort!" << std::endl;
}

int main()
{
    createInputFile();
    sortFile();
    testSorting();

    return 0;
}
