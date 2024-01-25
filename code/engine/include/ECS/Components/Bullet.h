#pragma once
#include "glm/glm.hpp"
#include "ECS/Entity.h"
namespace Engine
{

    struct Bullet
    {
        glm::vec2 velocity;
        int damage;
        Entity spawner;
    };

} // Engine
