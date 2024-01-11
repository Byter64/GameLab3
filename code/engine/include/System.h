#pragma once
#include <set>
#include "Entity.h"

namespace Engine
{

    class System
    {
    public:
        std::set<Entity> entities;
        virtual void EntityRemoved(Entity entity) = 0;
    };

} // Engine
