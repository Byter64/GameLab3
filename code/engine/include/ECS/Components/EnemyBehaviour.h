#pragma once
#include "glm/glm.hpp"

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
        enum State
        {
            Spawning,
            Idling,
            Moving,
            Shooting,
        };

        Behaviour behaviour = Default;
        State state = Spawning;
        float time = 0;

        glm::vec2 movement;
        float movementSpeed;
    };
}