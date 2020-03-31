#pragma once
#include "stdlib.h"
#include "iostream"

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out): out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream &out_;

    Error save(const bool value)
    {
        if (value)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;

        return Error::NoError;
    }

    Error save(const uint64_t number)
    {
        out_ << number << Separator;

        return Error::NoError;
    }

    template <class T>
    Error process(T&& val)
    {
        return save(std::forward<T>(val));
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        Error error = save(std::forward<T>(val));

        if (error == Error::NoError)
            return process(std::forward<Args>(args)...);
        else
            return error;
    }
};

class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in): in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::istream &in_;

    Error load(bool& value)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error load(uint64_t& number)
    {
        std::string text;
        in_ >> text;

        if (text.length() == 0)
            return Error::CorruptedArchive;

        int i = 0;
        while (i < text.length())
        {
            if (!std::isdigit(text[i]))
                return Error::CorruptedArchive;
            ++i;
        }

        number = std::stoul(text, NULL, 10);

        return Error::NoError;
    }

    template <class T>
    Error process(T&& val)
    {
        return load(std::forward<T>(val));
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        Error error = load(std::forward<T>(val));

        if (error == Error::NoError)
            return process(std::forward<Args>(args)...);
        else
            return error;
    }
};