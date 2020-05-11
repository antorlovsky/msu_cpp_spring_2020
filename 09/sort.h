#pragma once
#include <cstdlib>
#include <string>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>

class BinarySort
{
    using valueType = uint64_t;
    static constexpr size_t valueSize = sizeof(valueType);
    static constexpr size_t defaultSize = 0x800000 * valueSize;
private:
    size_t availableSize;
    int workingThreadsCount;
    int chunkCounter;
    valueType* memory;
    std::ifstream file;
    std::ofstream sortedFile;
    std::string filename;
    std::queue<std::string> sortedChunks;
    std::vector<std::thread> threads;

    std::mutex inputMutex;
    std::mutex queueMutex;
    std::condition_variable cv;

public:
    BinarySort(const char* name, size_t threadNum);
    ~BinarySort();

    bool sortChunks(valueType* chunk);
    bool mergeChunks(valueType* chunk);
    std::string generateFilename();
};