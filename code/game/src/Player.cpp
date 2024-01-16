#include "Player.h"
#include "Engine.h"
extern Engine::InputSystem* inputSystem;

Player::Player(std::string name, int playerIndex) : gameObject(name)
{

    if(playerIndex > 2)
    {
        throw std::runtime_error("Only two players allowed");
    }
    if(playerIndex == 1)
    {
        players[0] = this;

        std::shared_ptr<Engine::InputActionVec2> movementXY = std::make_shared<Engine::InputActionVec2>("MovementXY");
        movementXY->AddKeyboardBinding(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
        movementXY->AddOnValueChange(this, PlayerMovement);

        inputSystem->Add(movementXY);
    }
}

void Player::SetMovement(glm::vec2 input)
{
    if(glm::length(input) < 0.01f)
    {
        input = glm::vec2(0);
    }
    else
    {
        input = glm::normalize(input);
    }
    movementInput = input;
}

void Player::PlayerMovement(void* object, glm::vec2 input)
{
    Player* player = static_cast<Player*>(object);
    player->SetMovement(input);
}


