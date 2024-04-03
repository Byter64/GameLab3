#pragma once
#include "glm/glm.hpp"
#include "ECS/Helpers/Collision.h"
#include <map>
#include <utility>
#include <vector>

namespace Engine
{
    struct TilemapCollider
    {
        inline static const bool isStatic = true; //Tilemap Colliders are always static
        unsigned char layer = 0; //In which layer should the tilemap be?
        glm::vec3 position{0}; //bottom left of the tilemap, relative to the transform
        std::map<Collision, Collision::State> collisions;

        std::vector<std::vector<bool>> map;
    };
} // Engine
