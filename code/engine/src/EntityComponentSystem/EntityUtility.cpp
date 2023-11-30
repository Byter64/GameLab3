#include "EntityUtility.h"
#include "Name.h"
#include "Transform.h"
#include "ECSSystem.h"
#include <iostream>

extern Engine::ECSSystem ecsSystem;
namespace Engine
{
    /**
     * Finds a child with a name name in root and returns it. Prints a warning. if an Entity was found that has not both a Name and a Transform
     * @param root A parent of the looked for child. Does not need to be a direct parent.
     * @param name The name of the child which is looked for
     * @return The found entity, or INVALID_ENTITY_ID if none was found.
     */
    Entity FindChild(Entity root, Name name)
    {
        if(!ecsSystem.HasComponent<Name>(root))
        {
            std::cout << "ERROR: FindChild(Entity root, Name name) was called with a root that does not have a Name component. \n"
                      << "EntityID of root is: " << root << "\n";
            return INVALID_ENTITY_ID;
        }

        if(!ecsSystem.HasComponent<Transform>(root))
        {
            std::cout << "ERROR: FindChild(Entity root, Name name) was called with a root that does not have a Transform component. \n"
                      << "EntityID of root is: " << root << "\n"
                      << "Entity name of root is: " << ecsSystem.GetComponent<Name>(root).c_str() << "\n";
            return INVALID_ENTITY_ID;
        }

        for (Transform* child : ecsSystem.GetComponent<Transform>(root).GetChildren())
        {
            Entity result = FindChild(ecsSystem.GetEntity(*child), name);
            if(result != INVALID_ENTITY_ID)
            {
                return result;
            }
        }

        return INVALID_ENTITY_ID;
    }
} // Engine