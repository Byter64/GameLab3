#pragma once
#include "InputSystem/InputActionVec2.h"
#include "glm/glm.hpp"
#include <memory>
namespace Engine
{

    struct PlayerController
    {
        glm::vec3 movementInput;

        /*
         * Units per second
         */
        float speed;

        PlayerController() = default;
        ~PlayerController();
        void SetMovementInput(int leftKey, int rightKey, int upKey, int downKey, int backKey, int frontKey);

    private:
        std::shared_ptr<InputActionVec2> inputAction;
        std::shared_ptr<InputActionVec2> inputActionZ;

        static void GetMovement(void* object, glm::vec2 input);
        static void GetMovementZ(void* object, glm::vec2 input);
    };

} // Engine
