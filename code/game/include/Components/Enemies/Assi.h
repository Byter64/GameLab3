#pragma once
#include "Helpers/Movement.h"

struct Assi
{
    inline static float stunnedTime;

    bool isMoving = false;
    bool isPlayerInSight = false;

    float idleTimer = 0;
    float stunnedTimer = 0.0f;
    float speed = 1.0f;

    std::pair<int, int> startPos = {1, 1};

    Movement movement;
};
