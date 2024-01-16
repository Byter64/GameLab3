#pragma once
#include "InputSystem/InputActionVec2.h"
#include "glm/glm.hpp"
#include <memory>
namespace Engine
{

    class PlayerController
    {
        std::shared_ptr<InputActionVec2> inputAction;
        glm::vec2 movementInput;

        PlayerController() = default;
        ~PlayerController();

        static void GetMovement(void* object, glm::vec2 input);

    public:
        void SetMovementInput(int leftKey, int rightKey, int upKey, int downKey);
    };

} // Engine
