#pragma once
#include "Engine.h"
#include "Helpers/Movement.h"

struct Hubertus
{
    bool isMoving = false;
    float idleTimer = 0;
    float shootTimer = 0;
    float speed = 1.0f;
    float bulletSpeed = 3.0f;

    Movement movement;
    std::pair<int, int> startPos = {1, 1};

};
