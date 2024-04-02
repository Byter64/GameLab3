#pragma once
#include "Engine.h"
#include "glm/gtc/quaternion.hpp"

struct CuballExtra
{
    static inline std::pair<int, int> mirrorDirection = {1, 0};

    Engine::Entity rotationParent = Engine::Entity::INVALID_ENTITY_ID;
    glm::quat startRotation = glm::identity<glm::quat>();
    glm::quat targetRotation = glm::identity<glm::quat>();
    float progress = 1;
    enum Phase {
        Cube,
        MoveToNewPosition,
        BecomeBall,
        DisableMeshes,
        Ball
    };
    Phase phase = Cube;
    int cubeHealth;
    int ballHealth;
};
