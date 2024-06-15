#pragma once
#include "glm/glm.hpp"
#include "ECS/Helpers/Collision.h"
#include <map>

namespace Engine
{
    //IMPORTANT: This collider class is not able to rotate. Thus, be careful when rotating your entity
    //as the collider will not rotate with it. (scale works though)
    struct BoxCollider
    {
        bool isStatic = false; //Does the object of this collider stay in place or does it change position?
        unsigned char layer = 0; //In which layer should the collider be?
        glm::vec3 position{0}; //Center middle of the box, relative to the transform

        /*
         * Size is length of the box‘s edges. E.g. the unit cube has size {1, 1, 1}
         */
        glm::vec3 size{1};

        std::vector<Collision> collisions;
    };
} // Engine
