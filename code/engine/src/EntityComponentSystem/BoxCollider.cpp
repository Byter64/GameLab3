#include "BoxCollider.h"

namespace Engine
{
    void BoxCollider::Empty(BoxCollider& self, BoxCollider& other)
    {

    }

    BoxCollider::BoxCollider()
    {
        onCollisionEnter = Empty;
        onCollisionStay = Empty;
        onCollisionExit = Empty;

        size = glm::vec3(1);
        position = glm::vec3(0);
        isStatic = false;
    }

} // Engine
