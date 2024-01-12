#pragma once
#include "ECS/System.h"
#include "ECS/Components/Transform.h"
#include "ECS/Entity.h"

namespace Engine
{

    class TransformParentSystem : public System
    {
    public:
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;
    };

} // Engine
