#pragma once
#include "EngineException.h"
#include "ECS/Entity.h"

namespace Engine
{
    ///This exception will append information about the given entity after the error description
    class ECSException : public EngineException
    {
    public:
        ECSException(std::string errorName, std::string errorDescription, Entity entity);
    };
} // Engine
