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
            errorDescription += " (\"" + ecsSystem->GetComponent<Name>(entity) + "\")";
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

        if(ecsSystem->HasComponent<Transform>(entity))
        {
            Transform& transform = ecsSystem->GetComponent<Transform>(entity);

            if(transform.GetParent() == nullptr)
                errorDescription += "Transform has no parent";
            else
            {
                errorDescription += "Parent of transform is entity ";
                Engine::Entity parentEntity = ecsSystem->GetEntity(*transform.GetParent());
                errorDescription += std::to_string(parentEntity.id);
                if(ecsSystem->HasComponent<Name>(parentEntity))
                    errorDescription += " (\"" + ecsSystem->GetComponent<Name>(parentEntity) + "\")";
            }

            errorDescription += "\n";
            errorDescription += "Transform has " + std::to_string(transform.GetChildren().size()) + " children:";
            for(int i = 0; i < transform.GetChildren().size(); i++)
            {
                Engine::Entity childEntity = ecsSystem->GetEntity(*transform.GetChild(i));
                errorDescription += "\n\t" + std::to_string(childEntity.id);
                if(ecsSystem->HasComponent<Name>(childEntity))
                    errorDescription += " (\"" + ecsSystem->GetComponent<Name>(childEntity) + "\")";
            }
        }

        this->errorDescription = errorDescription;
    }
} // Engine