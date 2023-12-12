#pragma once
#include "glm/glm.hpp"

namespace Engine
{
    struct BoxCollider
    {
        void (*onCollisionEnter)(BoxCollider& self, BoxCollider& other);
        void (*onCollisionStay)(BoxCollider& self, BoxCollider& other);
        void (*onCollisionExit)(BoxCollider& self, BoxCollider& other);
        glm::vec3 position; //Center middle of the box, relative to the transform
        glm::vec3 size;
        bool isStatic; //Does the object of this collider stay in place or does it change position?

        BoxCollider();

        static void Empty(BoxCollider& self, BoxCollider& other);
    };

} // Engine
