#pragma once
#include "ECS/Entity.h"
#include "glm/glm.hpp"
namespace Engine
{

    struct Collision
    {
        enum State
        {
            ENTER,
            STAY,
            EXIT
        };

        Entity entity;
        Entity other;

        glm::vec3 entityPos; //In case of a tilemap, this will be equal to the tile's global position
        glm::vec3 otherPos; //In case of a tilemap, this will be equal to the tile's global position
        State state;

        Collision() = default;

        bool operator==(const Collision &other) const;
        bool operator!=(const Collision &other) const;
        bool operator<(const Collision &other) const;
    };

} // Engine
