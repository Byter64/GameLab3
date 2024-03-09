#pragma once
#include "glm/glm.hpp"
#include "ECS/Entity.h"
namespace Engine
{

    struct Bullet
    {
        int damage = 1;
        glm::vec2 velocity;
        Entity spawner;
    };

} // Engine
