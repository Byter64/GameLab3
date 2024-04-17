#pragma once
#include "Engine.h"
#include "Helpers/Movement.h"

struct Cuball
{
    static inline std::pair<int, int> mirrorDirection = {1, 0};

    enum Phase {
        Cube,
        MoveToNewPosition,
        BecomeBall,
        DisableMeshes,
        Ball
    };

    float progress = 1;
    float speed = 1.0f;
    float bulletSpeed = 3.0f;
    Phase phase = Cube;

    std::pair<int, int> startPos = {1, 1};
    Movement movement;

    Engine::Entity rotationParent = Engine::Entity::INVALID_ENTITY_ID;
    glm::quat startRotation = glm::identity<glm::quat>();
    glm::quat targetRotation = glm::identity<glm::quat>();
};
