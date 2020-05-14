#include "sum.h"
#include <iostream>
#include <fstream>

Sum::Sum(std::string filename): isReading(true), resultSum(0)
{
    std::ifstream input(filename, std::ios::in);

    for (int i = 0; i < 2; ++i)
    {
        threads.emplace_back(
                [this]
                {
                    while (this->calcSum()) {};
                });
    }
    std::string newString;
    char symbol = input.get();

    while (input.good())
    {
        if (symbol != '\n')
        {
            newString += symbol;
        }
        else
        {
            readString(newString);
            newString = "";
        }

        symbol = input.get();
    }

    readString(newString);

    isReading = false;
    cv.notify_all();

    input.close();

    for (auto& thread: threads)
        thread.join();

    std::cout << "Result sum = " << resultSum << std::endl;
}

void Sum::readString(std::string& newString)
{
    if (newString.empty())
        return;

    std::unique_lock<std::mutex> lock(queueMutex);

    stringsToSum.push(newString);
    cv.notify_one();
}

bool Sum::calcSum()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    while (stringsToSum.empty())
    {
        if (isReading)
            cv.wait(lock);
        else
            return false;
    }

    std::string currentString = stringsToSum.front();
    stringsToSum.pop();
    lock.unlock();

    std::string currentElement;

    for (char &it: currentString)
    {
        if (it == ' ')
        {
            updateResult(currentElement);
            currentElement = "";
        }
        else
        {
            currentElement += it;
        }
    }

    updateResult(currentElement);

    return true;
}

void Sum::updateResult(std::string& element)
{
    if (element.empty())
        return;

    std::unique_lock<std::mutex> lock(sumMutex);
    resultSum += std::stoi(element);
}