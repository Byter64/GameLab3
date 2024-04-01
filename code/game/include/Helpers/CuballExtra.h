#pragma once
#include "Engine.h"
#include "glm/gtc/quaternion.hpp"

struct CuballExtra
{
    Engine::Entity rotationParent = Engine::Entity::INVALID_ENTITY_ID;
    glm::quat startRotation = glm::identity<glm::quat>();
    glm::quat targetRotation = glm::identity<glm::quat>();
    float progress = 1;
    int cubeHealth;
    int ballHealth;
};
