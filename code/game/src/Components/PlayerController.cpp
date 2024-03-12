#include "Components/PlayerController.h"
#include "Engine.h"


void PlayerController::GetMovement(void* object, glm::vec2 input)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->movementInput.x = input.x;
    player->movementInput.y = input.y;
}

void PlayerController::SetMovementInput(int leftKey, int rightKey, int upKey, int downKey, int backKey, int frontKey)
{
    if(inputAction != nullptr)
        Engine::Systems::inputSystem->Remove(inputAction);

    if(inputActionZ != nullptr)
        Engine::Systems::inputSystem->Remove(inputActionZ);

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

void PlayerController::GetMovementZ(void *object, glm::vec2 input)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->movementInput.z = input.x;
}

void PlayerController::SetFireInput(int key)
{
    if(fireAction != nullptr)
        Engine::Systems::inputSystem->Remove(fireAction);

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

PlayerController &PlayerController::operator=(PlayerController &&other)
{
    if(inputAction != nullptr)
        Engine::Systems::inputSystem->Remove(inputAction);
    if(inputActionZ != nullptr)
        Engine::Systems::inputSystem->Remove(inputActionZ);
    if(fireAction != nullptr)
        Engine::Systems::inputSystem->Remove(fireAction);

    movementInput = other.movementInput;
    lookDirection = other.lookDirection;
    wasFirePushed = other.wasFirePushed;
    speed = other.speed;

    hasScoreChanged = true;
    stunnedTimer = other.stunnedTimer;
    uiTextScore = other.uiTextScore;
    score = other.score;

    if(other.inputAction != nullptr)
    {
        other.inputAction->RemoveOnValueChange(&other, GetMovement);
        inputAction = other.inputAction;
        inputAction->AddOnValueChange(this, GetMovement);
    }

    if(other.inputActionZ != nullptr)
    {
        other.inputActionZ->RemoveOnValueChange(&other, GetMovementZ);
        inputActionZ = other.inputActionZ;
        inputActionZ->AddOnValueChange(this, GetMovementZ);
    }

    if(other.fireAction != nullptr)
    {
        other.fireAction->RemoveOnStart(&other, GetFireActionStart);
        other.fireAction->RemoveOnEnd(&other, GetFireActionEnd);
        fireAction = other.fireAction;
        fireAction->AddOnStart(this, GetFireActionStart);
        fireAction->RemoveOnStart(&other, GetFireActionEnd);
    }

    return *this;
}
