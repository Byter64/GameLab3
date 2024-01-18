#include "ECS/ComponentManager.h"

namespace Engine
{
    /*
     * Returns the component type with the highest ID.
     */
    ComponentType ComponentManager::GetHighestComponentType() const
    {
        return nextFreeComponentType - 1;
    }
} // Engine