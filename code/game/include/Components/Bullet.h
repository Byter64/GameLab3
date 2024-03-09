#pragma once
#include "glm/glm.hpp"
#include "ECS/Entity.h"
struct Bullet
{
    int damage = 1;
    glm::vec2 velocity;
    Engine::Entity spawner;
};
