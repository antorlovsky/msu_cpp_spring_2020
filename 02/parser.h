#pragma once
#include "iostream"

using OnNumber = void (*)(int number);
using OnString = void (*)(const char* string);
using OnBegin = void (*)();
using OnEnd = void (*)();

void register_on_number_callback(OnNumber callback);
void register_on_string_callback(OnString callback);
void register_on_begin_callback(OnBegin callback);
void register_on_end_callback(OnEnd callback);

void onFoundElement(bool& isNumber, bool& isString, const char* text, size_t start, size_t length);
void parse(const char* text);