#pragma once
#include "GameObject.h"

class Player
{
    GameObject gameObject;
    inline static Player* players[2];
    glm::vec2 movementInput;
public:
    Player(std::string name, int playerIndex);

    void SetMovement(glm::vec2 input);

    static void PlayerMovement(void* object, glm::vec2 input);
};
