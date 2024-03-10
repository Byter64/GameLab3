#include "Components/PlayerController.h"
#include "InputSystem/InputSystem.h"


void PlayerController::GetMovement(void* object, glm::vec2 input)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->movementInput.x = input.x;
    player->movementInput.y = input.y;
}

void PlayerController::SetMovementInput(int leftKey, int rightKey, int upKey, int downKey, int backKey, int frontKey)
{
    if(inputAction != nullptr)
    {
        Engine::Systems::inputSystem->Remove(inputAction);
        Engine::Systems::inputSystem->Remove(inputActionZ);
    }

    inputAction = std::make_shared<Engine::InputActionVec2>("Movement");
    inputAction->AddKeyboardBinding(leftKey, rightKey, upKey, downKey);
    inputAction->AddOnValueChange(this, GetMovement);
    Engine::Systems::inputSystem->Add(inputAction);

    if(backKey != -1 && frontKey != -1)
    {
        inputActionZ = std::make_shared<Engine::InputActionVec2>("MovementZ");
        inputActionZ->AddKeyboardBinding(backKey, frontKey, 0, 0);
        inputActionZ->AddOnValueChange(this, GetMovementZ);
        Engine::Systems::inputSystem->Add(inputActionZ);
    }
}

PlayerController::~PlayerController()
{
    if(inputAction != nullptr)
    {
        Engine::Systems::inputSystem->Remove(inputAction);
        Engine::Systems::inputSystem->Remove(inputActionZ);
        Engine::Systems::inputSystem->Remove(fireAction);
    }
}

void PlayerController::GetMovementZ(void *object, glm::vec2 input)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->movementInput.z = input.x;
}

void PlayerController::SetFireInput(int key)
{
    if(fireAction != nullptr)
    {
        Engine::Systems::inputSystem->Remove(fireAction);
    }

    fireAction = std::make_shared<Engine::InputActionButton>("Fire");
    fireAction->AddKeyboardBinding(key);
    fireAction->AddOnStart(this, GetFireActionStart);
    fireAction->AddOnEnd(this, GetFireActionEnd);
    Engine::Systems::inputSystem->Add(fireAction);
}

void PlayerController::GetFireActionStart(void *object)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->wasFirePushed = true;

}

void PlayerController::GetFireActionEnd(void *object)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->wasFirePushed = false;
}

void PlayerController::AddScore(int points)
{
    score += points;
    hasScoreChanged = true;
}