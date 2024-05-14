#pragma once
#include "Engine.h"
#include "Helpers/Movement.h"

struct Duke
{
    inline static const float minDistanceToPlayer = 3;
    inline static const float minWalkDistance = 3;
    inline static const float maxWalkDistance = 10;
    inline static const float teleportTime = 0.5f;

    enum Phase
    {
        Tp_TeleportStart,
        Tp_TeleportEnd,
        Tp_Move,
        Tp_MoveTowardsPlayer,
        Tp_Sp_TeleportStart,

        Sp_TeleportEnd,
        Sp_SpawnEnemy,
        Sp_TeleportStart
    };

    float speed = 5.0f;
    float bulletSpeed = 10.0f;
    float timer = 0.5f;
    Engine::Entity target;
    Phase phase = Tp_TeleportEnd;
    bool canShoot = true;

    Movement movement;
    std::pair<int, int> startPos = {1, 1};

    ~Duke() = default;
};