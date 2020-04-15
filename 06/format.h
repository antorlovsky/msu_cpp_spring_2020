#pragma once
#include <iostream>
#include <sstream>
#include <vector>

class Format
{
public:
    Format(const std::string& str) : argNum(0), pattern(str) {}

    void addArguments() {}

    template <class T, class... ArgsT>
    void addArguments(T&& arg, ArgsT&&... args)
    {
        std::stringstream stream;
        stream << arg;

        ++argNum;
        patternArgs.push_back(stream.str());
        addArguments(std::forward<ArgsT>(args)...);
    }

    std::string getString()
    {
        std::string result;
        bool inPattern = false;
        size_t patternNum = 0;

        for (auto symbol: pattern)
        {
            if (symbol == '{')
            {
                if (inPattern)
                    throw std::runtime_error("Nesting");

                inPattern = true;
                patternNum = 0;
            }
            else
            {
                if (symbol == '}')
                {
                    if (!inPattern)
                        throw std::runtime_error("No opening {");

                    inPattern = false;
                    if (argNum <= patternNum)
                        throw std::runtime_error("Wrong number in {}");

                    result += patternArgs[patternNum];
                    continue;
                }

                if (inPattern)
                {
                    if (!std::isdigit(symbol))
                        throw std::runtime_error("Non-digit in {}");

                    patternNum = patternNum * 10 + symbol - '0';
                }
                else
                {
                    result += symbol;
                }
            }
        }

        if (inPattern)
            throw std::runtime_error("No closing }");

        return result;
    }

private:
    size_t argNum;
    const std::string pattern;
    std::vector<std::string> patternArgs;
};

template <class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args)
{
    Format format(str);
    format.addArguments(std::forward<ArgsT>(args)...);
    return format.getString();
}