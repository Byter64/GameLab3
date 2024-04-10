#pragma once
#include <exception>
#include <string>

namespace Engine
{
    ///Contains various information to std::cout on being created
    class BaseException : public std::exception
    {
    public:
        std::string stackTrace;
        const char *errorName;
        const char *errorDescription;
        BaseException();
        BaseException(const char *errorName, const char *errorDescription);
    };

} // Engine
