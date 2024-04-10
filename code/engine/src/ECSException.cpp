#include "ECSException.h"
#include "ECSSystem.h"
#include "ECS/Components/Name.h"
#include "ECS/Components/Transform.h"

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

        if(ecsSystem->HasComponent<Transform>(entity))
        {
            Transform& transform = ecsSystem->GetComponent<Transform>(entity);
            errorDescription += "Transform has " + std::to_string(transform.GetChildren().size()) + " children:";
            for(int i = 0; i < transform.GetChildren().size(); i++)
            {
                Engine::Entity childEntity = ecsSystem->GetEntity(transform.GetChild(i));
                errorDescription += "\n\t";
                if(ecsSystem->HasComponent<Name>(childEntity))
                    errorDescription += ecsSystem->GetComponent<Name>(childEntity);
                else
                    errorDescription += "Without name";
            }
        }

        this->errorDescription = errorDescription;
    }
} // Engine