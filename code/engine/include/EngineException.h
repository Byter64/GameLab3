#pragma once
#include <exception>
#include <string>

namespace Engine
{
    ///Contains various information to std::cout on being created
    class EngineException : public std::exception
    {
    public:
        std::string stackTrace;
        std::string errorName;
        std::string errorDescription;
        EngineException();
        EngineException(std::string errorName, std::string errorDescription);
    };

} // Engine
