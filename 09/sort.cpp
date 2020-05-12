#include "sort.h"
#include <iostream>

BinarySort::BinarySort(const char* name, size_t threadNum):
    filename(name),
    availableSize(defaultSize / valueSize / threadNum),
    memory(new valueType[defaultSize / valueSize]),
    workingThreadsCount(0),
    chunkCounter(0)
{
    file.open(name, std::ios::in | std::ios::binary);

    for (size_t i = 0; i < threadNum; ++i)
    {
        threads.emplace_back(
                [this, i]
                {
                    if (i % 2 == 0)
                    {
                        ++this->workingThreadsCount;
                        this->sortChunks(this->memory + i * this->availableSize);
                        --this->workingThreadsCount;
                    }

                    while (this->mergeChunks(this->memory + i * this->availableSize)) {};

                    cv.notify_one();
                });
    }

    for (auto& thread: threads)
        thread.join();
}

BinarySort::~BinarySort()
{
    std::ifstream sortedChunk;
    if (!sortedChunks.empty())
    {
        sortedChunk.open(sortedChunks.front(), std::ios::in | std::ios::binary);
        sortedFile.open("output.txt", std::ios::out | std::ios::binary);

        while (!sortedChunk.eof())
        {
            sortedChunk.read(reinterpret_cast<char*>(memory), defaultSize / valueSize);
            sortedFile.write(reinterpret_cast<char*>(memory), sortedChunk.gcount());
        }

        sortedChunk.close();
    }

    file.close();
    sortedFile.close();

    delete[] memory;
}

void BinarySort::sortChunks(valueType* chunk)
{
    while (!file.eof())
    {
        size_t chunkSize;
        {
            std::unique_lock<std::mutex> lock(inputMutex);

            file.read(reinterpret_cast<char*>(chunk), availableSize);
            chunkSize = file.gcount();
        }

        if (chunkSize == 0)
            return;

        std::sort(chunk, chunk + chunkSize / valueSize);

        std::string chunkName = generateFilename();
        std::ofstream chunkFile;
        chunkFile.open(chunkName, std::ios::out | std::ios::binary);
        chunkFile.write(reinterpret_cast<char*>(chunk), chunkSize);
        chunkFile.close();

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            sortedChunks.push(chunkName);
        }
    }
}

bool BinarySort::mergeChunks(valueType* chunk)
{
    std::unique_lock<std::mutex> lock(queueMutex);
    while (sortedChunks.size() < 2)
    {
        if (workingThreadsCount > 0)
            cv.wait(lock);
        else
            return false;
    }
    ++workingThreadsCount;

    std::ifstream firstChunk(sortedChunks.front(), std::ios::in | std::ios::binary);
    sortedChunks.pop();
    std::ifstream secondChunk(sortedChunks.front(), std::ios::in | std::ios::binary);
    sortedChunks.pop();

    lock.unlock();

    std::string mergedChunkName = generateFilename();
    std::ofstream mergedFile(mergedChunkName, std::ios::out | std::ios::binary);

    valueType first, second;
    bool readFirst = true;
    bool readSecond = true;

    while (!firstChunk.eof() && ! secondChunk.eof())
    {
        if (readFirst)
            firstChunk.read(reinterpret_cast<char*>(&first), valueSize);
        if (readSecond)
            secondChunk.read(reinterpret_cast<char*>(&second), valueSize);

        if (first <= second)
        {
            mergedFile.write(reinterpret_cast<char*>(&first), valueSize);
            readFirst = true;
            readSecond = false;
        }
        else
        {
            mergedFile.write(reinterpret_cast<char*>(&second), valueSize);
            readFirst = false;
            readSecond = true;
        }
    }

    if (!firstChunk.eof())
    {
        while (!(firstChunk.read(reinterpret_cast<char*>(chunk), availableSize)).eof())
            mergedFile.write(reinterpret_cast<char*>(chunk), firstChunk.gcount());
    }
    else if (!secondChunk.eof())
    {
        while (!(secondChunk.read(reinterpret_cast<char*>(chunk), availableSize)).eof())
            mergedFile.write(reinterpret_cast<char*>(chunk), secondChunk.gcount());
    }

    firstChunk.close();
    secondChunk.close();
    mergedFile.close();

    lock.lock();
    sortedChunks.push(mergedChunkName);
    --workingThreadsCount;

    cv.notify_one();

    return true;
}

std::string BinarySort::generateFilename()
{
    return "chunk_" + std::to_string(chunkCounter++) + ".txt";
}