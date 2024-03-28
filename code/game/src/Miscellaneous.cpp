#include "../../game/include/Miscellaneous.h"

static const float SQRT_THREE_FOURTH = 0.866025f;

glm::vec3 Miscellaneous::RoundToAxis(glm::vec3 vec)
{

    if(vec != glm::vec3(0))
        vec = glm::normalize(vec);

    vec *= SQRT_THREE_FOURTH; //Normalize
    vec.x = roundf(vec.x);
    vec.y = roundf(vec.y);
    vec.z = roundf(vec.z);

    if(vec != glm::vec3(0))
        vec = glm::normalize(vec);
    return vec;
}