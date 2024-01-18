#include "ECS/Components/PlayerController.h"
#include "InputSystem/InputSystem.h"

extern Engine::InputSystem* inputSystem;

namespace Engine
{
    void PlayerController::GetMovement(void* object, glm::vec2 input)
    {
        PlayerController* player = static_cast<PlayerController*>(object);
        player->movementInput = input;
    }

    void PlayerController::SetMovementInput(int leftKey, int rightKey, int upKey, int downKey)
    {
        if(inputAction != nullptr)
        {
            inputSystem->Remove(inputAction);
        }

        inputAction = std::make_shared<Engine::InputActionVec2>("Movement");
        inputAction->AddKeyboardBinding(leftKey, rightKey, upKey, downKey);
        inputAction->AddOnValueChange(this, GetMovement);
        inputSystem->Add(inputAction);
    }

    PlayerController::~PlayerController()
    {
        if(inputAction != nullptr)
        {
            inputSystem->Remove(inputAction);
        }
    }
} // Engine