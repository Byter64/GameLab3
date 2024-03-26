#pragma once
#include "Engine.h"
#include <chrono>
#include <queue>
struct KindredSpiritExtra
{
    inline static float maxTimeDifference = 1.0f;
    inline static std::queue<glm::vec4> colours;
    bool isMainEntity;
    std::chrono::time_point<std::chrono::system_clock> timeWhenLastHit;
    Engine::Entity other;

};