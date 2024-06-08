#pragma once
#include "Helpers/Movement.h"

struct KindredSpirit
{
    inline static float maxTimeDifference = 1.0f;
    inline static std::queue<glm::vec4> colours;

    bool isMainEntity;
    float speed = 1.0f;

    std::chrono::time_point<std::chrono::system_clock> timeWhenLastHit;
    std::pair<int, int> startPos = {1, 1};
    Movement movement;
    glm::vec4 colour;

    Engine::Entity other;
};
