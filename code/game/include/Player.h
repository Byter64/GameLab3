#pragma once
#include "ECSSystem.h"
#include "glm/glm.hpp"

class Player
{
    Engine::Entity gameObject;
    inline static Player* players[2];
    glm::vec2 movementInput;
public:
    Player(std::string name, int playerIndex);

    void SetMovement(glm::vec2 input);

    static void PlayerMovement(void* object, glm::vec2 input);
};
