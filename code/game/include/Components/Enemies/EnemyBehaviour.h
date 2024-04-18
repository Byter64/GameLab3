#pragma once
#include "glm/glm.hpp"
#include <utility>
#include <map>
#include <string>
#include <vector>

struct EnemyBehaviour
{
    enum Type
    {
        Hubertus,
        KindredSpirit,
        Assi,
        Cuball,
        Duke
    };
    inline static const std::map<std::string, Type> stringToBehaviour =
            {
                    {"Hubertus", Hubertus},
                    {"KindredSpirit", KindredSpirit},
                    {"hubertus", Hubertus},
                    {"kindredspirit", KindredSpirit},
                    {"Assi", Assi},
                    {"assi", Assi},
                    {"Cuball", Cuball},
                    {"cuball", Cuball},
                    {"Duke", Duke},
                    {"duke", Duke}
    };

    inline static const std::map<Type, std::string> behaviourToString =
            {
                    {Hubertus, "Hubertus"},
                    {KindredSpirit, "KindredSpirit"},
                    {Assi, "Assi"},
                    {Cuball, "Cuball"},
                    {Duke, "Duke"}
            };

    //Use the enum Behaviour to access the appropriate scores
    inline static std::vector<int> scores =
            {
                100,
                200,
                400,
                400,
                10
            };

    inline static float scoreDecrease;
    bool isActive = false;
    float spawnTime;
};