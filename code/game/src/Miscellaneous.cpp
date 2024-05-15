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

/// Rounds the given direction to an axis and returns the direction to it. The returned vector will never contain -0.0f (only 0.0f). It can however return (0.0f, 0.0f)
/// \param vec
/// \return
glm::vec3 Miscellaneous::RoundTo4Directions(glm::vec3 vec)
{
    if(vec == glm::vec3(0)) return glm::vec3(0);
    float angle = glm::atan(vec.y, vec.x);

    angle = angle / (0.5f * glm::pi<float>());
    angle = glm::round(angle);
    angle = angle * (0.5f * glm::pi<float>());

    float x = glm::round(glm::cos(angle));
    float y = glm::round(glm::sin(angle));

    x = -0.0f == x ? 0.0f : x;
    y = -0.0f == y ? 0.0f : y;
    return glm::vec3(x, y, 0);
}
