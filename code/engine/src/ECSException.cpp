#include "ECSException.h"
#include "ECSSystem.h"
#include "ECS/Components/Name.h"

namespace Engine
{
    ECSException::ECSException(std::string errorName, std::string errorDescription, Entity entity) : EngineException()
    {
        this->errorName = errorName;

        errorDescription += "\n";
        errorDescription += "Affected entity is ";
        errorDescription += std::to_string(entity.id);
        if(ecsSystem->HasComponent<Name>(entity))
            errorDescription += "(\"" + ecsSystem->GetComponent<Name>(entity) + "\")";
        errorDescription += "\n";
        errorDescription += "It has the following components:\n";

        for(ComponentType i = 0; i < ecsSystem->GetNumberOfRegisteredComponents(); i++)
        {
            if(ecsSystem->HasComponent(entity, i))
            {
                errorDescription += std::to_string(i) + ": ";
                errorDescription += ecsSystem->GetComponentTypeName(i);
                errorDescription += "\n";
            }
        }
        errorDescription.pop_back();

        this->errorDescription = errorDescription;
    }
} // Engine