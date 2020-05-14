#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>

class Sum
{
private:
    int resultSum;
    bool isReading;
    std::queue<std::string> stringsToSum;
    std::vector<std::thread> threads;

    std::mutex sumMutex;
    std::mutex queueMutex;
    std::mutex workingMutex;
    std::condition_variable cv;

public:
    Sum(std::string filename);
    ~Sum() {};

    bool calcSum();
    void readString(std::string&);
    void updateResult(std::string&);
};
