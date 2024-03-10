#pragma once
#include "glm/glm.hpp"
#include <utility>
#include <map>
#include <string>
#include <vector>

struct EnemyBehaviour
{
    enum Behaviour
    {
        Default,
        KindredSpirit,
        Assi,
        Cuball
    };
    inline static const std::map<std::string, Behaviour> stringToBehaviour =
            {
                    {"Hubertus", Default},
                    {"KindredSpirit", KindredSpirit}
    };

    inline static const std::map<Behaviour, std::string> behaviourToString =
            {
                    {Default, "Hubertus"},
                    {KindredSpirit, "KindredSpirit"}
            };

    //Use the enum Behaviour to access the appropriate scores
    inline static const std::vector<int> scores =
            {
                100,
                200,
                300,
                200
            };

    bool isMoving = false;
    Behaviour behaviour = Default;
    float idlerTimer = 0;
    float shootTimer = 0;

    std::pair<int, int> startPos = {1, 1};
    std::pair<int, int> targetNode;
    std::pair<int, int> oldTargetNode;
    glm::vec2 targetPos;
    glm::vec2 movement;
    float movementSpeed = 1.0f;
};