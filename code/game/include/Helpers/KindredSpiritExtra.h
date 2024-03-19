#pragma once
#include "Engine.h"
#include <chrono>

struct KindredSpiritExtra
{
    inline static const float maxTimeDifference = 1.0f;
    bool isMainEntity;
    std::chrono::time_point<std::chrono::system_clock> timeWhenLastHit;
    Engine::Entity other;

};