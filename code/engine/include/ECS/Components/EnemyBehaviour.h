#pragma once
#include "glm/glm.hpp"
#include <utility>

namespace Engine
{
    struct EnemyBehaviour
    {
        enum Behaviour
        {
            Default,
            KindredSpirit,
            Assi,
            Cuball
        };

        Behaviour behaviour = Default;
        float idlerTimer = 0;
        float shootTimer = 0;

        bool isMoving = false;
        std::pair<int, int> targetNode;
        glm::vec2 targetPos;
        glm::vec2 movement;
        float movementSpeed;
    };
}