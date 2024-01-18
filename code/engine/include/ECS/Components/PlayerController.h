#pragma once
#include "InputSystem/InputActionVec2.h"
#include "glm/glm.hpp"
#include <memory>
namespace Engine
{

    struct PlayerController
    {
        glm::vec2 movementInput;

        /*
         * Units per second
         */
        float speed;

        PlayerController() = default;
        ~PlayerController();
        void SetMovementInput(int leftKey, int rightKey, int upKey, int downKey);

    private:
        std::shared_ptr<InputActionVec2> inputAction;

        static void GetMovement(void* object, glm::vec2 input);
    };

} // Engine
