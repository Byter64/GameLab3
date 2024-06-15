#include "Components/PlayerController.h"
#include "Engine.h"

void PlayerController::AddMovementInput(int leftKey, int rightKey, int upKey, int downKey)
{
    if(inputAction == nullptr)
    {
        inputAction = std::make_shared<Engine::InputActionVec2>("Movement");
        inputAction->AddOnValueChange(this, GetMovement);
    }

    Engine::Systems::inputSystem->Remove(inputAction);
    inputAction->AddKeyboardBinding(leftKey, rightKey, upKey, downKey);
    Engine::Systems::inputSystem->Add(inputAction);
}

void PlayerController::AddMovementInput(unsigned char joystickID, unsigned char leftButton, unsigned char rightButton, unsigned char upButton, unsigned char downButton)
{
    Engine::GamepadInputID leftKey = {joystickID, leftButton, Engine::GamepadInputID::Button};
    Engine::GamepadInputID rightKey = {joystickID, rightButton, Engine::GamepadInputID::Button};
    Engine::GamepadInputID upKey = {joystickID, upButton, Engine::GamepadInputID::Button};
    Engine::GamepadInputID downKey = {joystickID, downButton, Engine::GamepadInputID::Button};

    if(inputAction == nullptr)
    {
        inputAction = std::make_shared<Engine::InputActionVec2>("Movement");
        inputAction->AddOnValueChange(this, GetMovement);
    }

    Engine::Systems::inputSystem->Remove(inputAction);
    inputAction->AddGamepadButtonBinding(leftKey, rightKey, downKey, upKey);
    Engine::Systems::inputSystem->Add(inputAction);
}

void PlayerController::AddMovementInput(unsigned char joystickID, unsigned char xAxis, unsigned char yAxis)
{
    Engine::GamepadInputID xAxisID = {joystickID, xAxis, Engine::GamepadInputID::Axis};
    Engine::GamepadInputID yAxisID = {joystickID, yAxis, Engine::GamepadInputID::Axis};

    if(inputAction == nullptr)
    {
        inputAction = std::make_shared<Engine::InputActionVec2>("Movement");
        inputAction->AddOnValueChange(this, GetMovement);
    }

    Engine::Systems::inputSystem->Remove(inputAction);
    inputAction->AddGamepadAxesBinding(xAxisID, yAxisID);
    Engine::Systems::inputSystem->Add(inputAction);
}

void PlayerController::GetMovement(void* object, glm::vec2 input)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->movementInput.x = input.x;
    player->movementInput.y = input.y;
}

void PlayerController::GetMovementZ(void *object, glm::vec2 input)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->movementInput.z = input.x;
}

void PlayerController::AddFireInput(int key)
{
    if(fireAction == nullptr)
    {
        fireAction = std::make_shared<Engine::InputActionButton>("Fire");
        fireAction->AddOnStart(this, GetFireActionStart);
        fireAction->AddOnEnd(this, GetFireActionEnd);
    }

    Engine::Systems::inputSystem->Remove(fireAction);
    fireAction->AddKeyboardBinding(key);
    Engine::Systems::inputSystem->Add(fireAction);
}

void PlayerController::AddFireInput(Engine::GamepadInputID button)
{
    if(fireAction == nullptr)
    {
        fireAction = std::make_shared<Engine::InputActionButton>("Fire");
        fireAction->AddOnStart(this, GetFireActionStart);
        fireAction->AddOnEnd(this, GetFireActionEnd);
    }

    Engine::Systems::inputSystem->Remove(fireAction);
    fireAction->AddGamepadBinding(button);
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

void PlayerController::AddReviveInput(Engine::GamepadInputID button)
{
    if(reviveAction == nullptr)
    {
        reviveAction = std::make_shared<Engine::InputActionButton>("Revive");
        reviveAction->AddOnStart(this, GetReviveActionStart);
    }

    Engine::Systems::inputSystem->Remove(reviveAction);
    reviveAction->AddGamepadBinding(button);
    Engine::Systems::inputSystem->Add(reviveAction);
}

void PlayerController::AddReviveInput(int button)
{
    if(reviveAction == nullptr)
    {
        reviveAction = std::make_shared<Engine::InputActionButton>("Revive");
        reviveAction->AddOnStart(this, GetReviveActionStart);
    }

    Engine::Systems::inputSystem->Remove(reviveAction);
    reviveAction->AddKeyboardBinding(button);
    Engine::Systems::inputSystem->Add(reviveAction);
}

void PlayerController::GetReviveActionStart(void *object)
{
    PlayerController* player = static_cast<PlayerController*>(object);
    player->wasRevivePushed = true;
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
    if(fireAction != nullptr)
        Engine::Systems::inputSystem->Remove(fireAction);
    if(reviveAction != nullptr)
        Engine::Systems::inputSystem->Remove(reviveAction);

    movementInput = other.movementInput;
    lookDirection = other.lookDirection;
    wasFirePushed = other.wasFirePushed;
    activeBullets = other.activeBullets;
    speed = other.speed;

    hasScoreChanged = true;
    isActive = other.isActive;
    stunnedTimer = other.stunnedTimer;
    respawnTimer = other.respawnTimer;
    uiTextScore = other.uiTextScore;
    score = other.score;

    if(other.inputAction != nullptr)
    {
        other.inputAction->RemoveOnValueChange(&other, GetMovement);
        inputAction = other.inputAction;
        inputAction->AddOnValueChange(this, GetMovement);
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

void PlayerController::ResetInput()
{
    Engine::Systems::inputSystem->Remove(inputAction);
    Engine::Systems::inputSystem->Remove(fireAction);
    Engine::Systems::inputSystem->Remove(reviveAction);
}

int PlayerController::GetScore()
{
    return score;
}
