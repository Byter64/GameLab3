#pragma once
#include "glm/glm.hpp"
#include <utility>
#include <map>
#include <string>
#include <vector>
#include "Helpers/EnemyExtra.h"

struct EnemyBehaviour
{
    enum Behaviour
    {
        Hubertus,
        KindredSpirit,
        Assi,
        Cuball
    };
    inline static const std::map<std::string, Behaviour> stringToBehaviour =
            {
                    {"Hubertus", Hubertus},
                    {"KindredSpirit", KindredSpirit},
                    {"hubertus", Hubertus},
                    {"kindredspirit", KindredSpirit}
    };

    inline static const std::map<Behaviour, std::string> behaviourToString =
            {
                    {Hubertus, "Hubertus"},
                    {KindredSpirit, "KindredSpirit"},
                    {Hubertus, "hubertus"},
                    {KindredSpirit, "kindredspirit"}
            };

    //Use the enum Behaviour to access the appropriate scores
    inline static std::vector<int> scores =
            {
                100,
                200,
                300,
                200
            };

    bool isMoving = false;
    Behaviour behaviour = Hubertus;
    float idlerTimer = 0;
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