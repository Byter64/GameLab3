#pragma once
#include <set>
#include "Entity.h"

namespace Engine
{

    class System
    {
    public:
        std::set<Entity> entities;

        /**
         * Gets called when an entity is added to the system. At the time of calling, the entity has not been added to "entities" yet.
         * @param entity The entity that was added.
         */
        virtual void EntityAdded(Entity entity) = 0;

        /**
         * Gets called when an entity is removed from the system. At the time of calling, the entity has already been removed from "entities" but its components are still intact.
         * @param entity
         */
        virtual void EntityRemoved(Entity entity) = 0;
    };

} // Engine
