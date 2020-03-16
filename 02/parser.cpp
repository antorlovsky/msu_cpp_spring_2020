#include "parser.h"
#include "stdlib.h"

OnNumber numberHandler = nullptr;
OnString stringHandler = nullptr;
OnBegin beginHandler = nullptr;
OnEnd endHandler = nullptr;

void register_on_number_callback(OnNumber callback)
{
    numberHandler = callback;
}

void register_on_string_callback(OnString callback)
{
    stringHandler = callback;
}

void register_on_begin_callback(OnBegin callback)
{
    beginHandler = callback;
}

void register_on_end_callback(OnEnd callback)
{
    endHandler = callback;
}


void parse(const char* text)
{
    size_t startPos = 0, endPos = 0, curPos = 0;
    bool isNumber = false, isString = false;
    char curElem;
    std::string arg;

    if (beginHandler)
        beginHandler();

    while (*(text++))
    {
        curElem = *(text - 1);
        if (curElem >= '0' && curElem <= '9')
        {
            if (!isNumber)
            {
                isNumber = true;
                startPos = curPos;
            }

            endPos = curPos;
        }
        else if (!(curElem == ' ' || curElem == '\t' || curElem == '\n'))
        {
            if (!isString)
            {
                isString = true;
                startPos = curPos;
            }

            endPos = curPos;
        }
        else
        {
            arg = std::string(text - curPos - 1 + startPos, endPos - startPos + 1);
            if (isNumber)
            {
                if (numberHandler)
                    numberHandler(atoi(arg.c_str()));
                isNumber = false;
            }
            else if (isString)
            {
                if (stringHandler)
                    stringHandler(arg.c_str());
                isString = false;
            }
        }
        ++curPos;
    }

    arg = std::string(text - curPos - 1 + startPos, endPos - startPos + 1);

    if (isNumber)
    {
        if (numberHandler)
            numberHandler(atoi(arg.c_str()));
        isNumber = false;
    }
    else if (isString)
    {
        if (stringHandler)
            stringHandler(arg.c_str());
        isString = false;
    }

    if(endHandler)
        endHandler();
}