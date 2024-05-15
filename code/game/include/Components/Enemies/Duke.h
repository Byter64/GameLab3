#pragma once
#include "Engine.h"
#include "Helpers/Movement.h"

struct Duke
{
    inline static const float minDistanceToPlayer = 3;
    inline static const float maxDistanceToPlayerAttacking = 5;
    inline static const float minDistanceToPlayerSpawning = 10;
    inline static const float minWalkDistance = 3;
    inline static const float maxWalkDistance = 10;
    inline static const float teleportTime = 0.5f;
    inline static const float spawnTime = 1.0f;

    enum Phase
    {
        PhaseStart_TeleportStart,
        PhaseStart_SpawnEnemy,
        PhaseStart_TeleportEnd,

        Tp_TeleportStart,
        Tp_TeleportEnd,
        Tp_Move,
        Tp_TeleportTowardsPlayerStart,

        Sp_SpawnEnemy,
        Sp_TeleportEnd,
    };

    inline static std::map<EnemyBehaviour::Type, unsigned int> spawnCount =
            {
                    {EnemyBehaviour::Hubertus, 6},
                    {EnemyBehaviour::KindredSpirit, 2},
                    {EnemyBehaviour::Assi, 3},
                    {EnemyBehaviour::Cuball, 3}};

    float speed = 5.0f;
    float bulletSpeed = 10.0f;
    float timer = 0.5f;
    Engine::Entity target;
    Phase phase = PhaseStart_TeleportStart;
    bool canShoot = true;
    EnemyBehaviour::Type spawnedType = EnemyBehaviour::Hubertus;

    unsigned int spawnCounter = 0;
    unsigned int teleportCounter = 0;

    Movement movement;
    std::pair<int, int> startPos = {1, 1};

    ~Duke() = default;
};
