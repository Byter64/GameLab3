#include "../../game/include/Miscellaneous.h"

static const float SQRT_THREE_FOURTH = 0.866025f;

glm::vec3 Miscellaneous::RoundTo8Directions(glm::vec3 vec)
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

glm::vec3 Miscellaneous::RoundTo4Directions(glm::vec3 vec)
{
    float angle = glm::atan(vec.y, vec.x);
    angle = angle / (0.5f * glm::pi<float>());
    angle = glm::round(angle);
    angle = angle * (0.5f * glm::pi<float>());

    return glm::vec3(glm::cos(angle), glm::sin(angle), 0);
}
