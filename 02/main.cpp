#include "parser.h"

int beginCount, numCount, stringCount, endCount;

void init()
{
    beginCount = 0;
    numCount = 0;
    stringCount = 0;
    endCount = 0;
}

void onBegin()
{
    ++beginCount;
}

void onNumberFound(const char* n)
{
    ++numCount;
}

void onStringFound(const char* str)
{
    ++stringCount;
}

void onEnd()
{
    ++endCount;
}

void testEmptyCallbacks0()
{
    parse("123 abc 123");

    assert(beginCount == 0 && numCount == 0 && stringCount == 0 && endCount == 0);
    init();
}

void testEmptyCallbacks1()
{
    register_on_begin_callback(onBegin);
    parse("123 abc 123");

    assert(beginCount == 1 && numCount == 0 && stringCount == 0 && endCount == 0);
    init();
}

void testEmptyCallbacks2()
{
    register_on_begin_callback(onBegin);
    register_on_number_callback(onNumberFound);
    parse("123 abc 123");

    assert(beginCount == 1 && numCount == 2 && stringCount == 0 && endCount == 0);
    init();
}

void testEmptyCallbacks3()
{
    register_on_begin_callback(onBegin);
    register_on_number_callback(onNumberFound);
    register_on_string_callback(onStringFound);
    parse("123 abc 123");

    assert(beginCount == 1 && numCount == 2 && stringCount == 1 && endCount == 0);
    init();
}

void testEmptyCallbacks4()
{
    register_on_begin_callback(onBegin);
    register_on_number_callback(onNumberFound);
    register_on_string_callback(onStringFound);
    register_on_end_callback(onEnd);
    parse("123 abc 123");

    assert(beginCount == 1 && numCount == 2 && stringCount == 1 && endCount == 1);
    init();
}

void testEmptyParse0()
{
    register_on_begin_callback(onBegin);
    register_on_number_callback(onNumberFound);
    register_on_string_callback(onStringFound);
    register_on_end_callback(onEnd);
    parse("");

    assert(beginCount == 1 && numCount == 0 && stringCount == 0 && endCount == 1);
    init();
}


void testEmptyParse1()
{
    register_on_begin_callback(onBegin);
    register_on_number_callback(onNumberFound);
    register_on_string_callback(onStringFound);
    register_on_end_callback(onEnd);
    parse("   \n\n\n\n\t   \t\t \t\n \n \t    ");

    assert(beginCount == 1 && numCount == 0 && stringCount == 0 && endCount == 1);
    init();
}

void testTabs()
{
    register_on_begin_callback(onBegin);
    register_on_number_callback(onNumberFound);
    register_on_string_callback(onStringFound);
    register_on_end_callback(onEnd);
    parse(" 123 abc\t$ # 123\n c\t");

    assert(beginCount == 1 && numCount == 2 && stringCount == 4 && endCount == 1);
    init();
}

int main()
{
    init();
    testEmptyCallbacks0();
    testEmptyCallbacks1();
    testEmptyCallbacks2();
    testEmptyCallbacks3();
    testEmptyCallbacks4();
    testEmptyParse0();
    testEmptyParse1();
    testTabs();

    return 0;
}