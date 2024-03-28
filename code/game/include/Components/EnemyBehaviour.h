#pragma once
#include "glm/glm.hpp"
#include <utility>
#include <map>
#include <string>
#include <vector>
#include "Helpers/EnemyExtra.h"

struct EnemyBehaviour
{
    enum Type
    {
        Hubertus,
        KindredSpirit,
        Assi,
        Cuball
    };
    inline static const std::map<std::string, Type> stringToBehaviour =
            {
                    {"Hubertus", Hubertus},
                    {"KindredSpirit", KindredSpirit},
                    {"hubertus", Hubertus},
                    {"kindredspirit", KindredSpirit},
                    {"Assi", Assi},
                    {"assi", Assi}
    };

    inline static const std::map<Type, std::string> behaviourToString =
            {
                    {Hubertus, "Hubertus"},
                    {KindredSpirit, "KindredSpirit"},
                    {Assi, "Assi"},
            };

    //Use the enum Behaviour to access the appropriate scores
    inline static std::vector<int> scores =
            {
                100,
                200,
                400,
                200
            };

    bool isActive = false;
    bool isMoving = false;
    Type behaviour = Hubertus;
    float idleTimer = 0;
    float shootTimer = 0;

    std::pair<int, int> startPos = {1, 1};
    std::pair<int, int> targetNode;
    std::pair<int, int> oldTargetNode;
    glm::vec2 targetPos;
    glm::vec2 movement;
    float speed = 1.0f;
    float bulletSpeed = 3.0f;
    float spawnTime;

    //The type is determined by behaviour
    //You could call it a tagged union ヾ(⌐■_■)ノ♪
    EnemyExtra enemyExtra;
};