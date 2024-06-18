#pragma once
#include "glm/glm.hpp"
#include "ECS/Entity.h"
struct Bullet
{
    int damage = 1;
    bool isSprite = false;
    glm::vec2 velocity;
    Engine::Entity spawner;
};
