#pragma once
#include "Engine.h"

struct Movement
{
    std::pair<int, int> targetNode;
    std::pair<int, int> oldTargetNode;
    glm::vec2 targetPos;
    glm::vec2 currentPos;
    glm::vec2 direction;
};
