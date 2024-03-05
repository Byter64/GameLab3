#include "ECS/Components/PlayerController.h"
#include "InputSystem/InputSystem.h"

extern Engine::InputSystem* inputSystem;

namespace Engine
{
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
            inputSystem->Remove(inputAction);
            inputSystem->Remove(inputActionZ);
        }

        inputAction = std::make_shared<Engine::InputActionVec2>("Movement");
        inputAction->AddKeyboardBinding(leftKey, rightKey, upKey, downKey);
        inputAction->AddOnValueChange(this, GetMovement);
        inputSystem->Add(inputAction);

        inputActionZ = std::make_shared<Engine::InputActionVec2>("MovementZ");
        inputActionZ->AddKeyboardBinding(backKey, frontKey, 0, 0);
        inputActionZ->AddOnValueChange(this, GetMovementZ);
        inputSystem->Add(inputActionZ);
    }

    PlayerController::~PlayerController()
    {
        if(inputAction != nullptr)
        {
            inputSystem->Remove(inputAction);
            inputSystem->Remove(inputActionZ);
            inputSystem->Remove(fireAction);
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
            inputSystem->Remove(fireAction);
        }

        fireAction = std::make_shared<Engine::InputActionButton>("Fire");
        fireAction->AddKeyboardBinding(key);
        fireAction->AddOnStart(this, GetFireActionStart);
        fireAction->AddOnEnd(this, GetFireActionEnd);
        inputSystem->Add(fireAction);
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
} // Engine