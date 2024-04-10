#pragma once
#include "ECS/ComponentManager.h"

namespace Engine
{

    const char *ComponentManager::GetTypeName(ComponentType componentType)
    {
        if(typeToName.count(componentType))
            return typeToName[componentType];
        else
            return "Unknown component";
    }

    ComponentType ComponentManager::GetNumberOfRegisteredComponents()
    {
        return nextFreeComponentType;
    }
} //Engine