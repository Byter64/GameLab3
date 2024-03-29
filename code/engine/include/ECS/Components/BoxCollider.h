#pragma once
#include "glm/glm.hpp"
#include "ECS/Helpers/Collision.h"
#include <map>

namespace Engine
{
    struct BoxCollider
    {
        bool isStatic = false; //Does the object of this collider stay in place or does it change position?
        unsigned char layer = 0; //In which layer should the collider be?
        glm::vec3 position{0}; //Center middle of the box, relative to the transform

        /*
         * Size is half of the length of the box‘s edges (i.e. it is the radius)
         */
        glm::vec3 size{1};

        std::map<Collision, Collision::State> collisions;
    };

} // Engine
