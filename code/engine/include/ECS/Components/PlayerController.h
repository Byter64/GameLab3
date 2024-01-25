#pragma once
#include "InputSystem/InputActionVec2.h"
#include "InputSystem/InputActionButton.h"
#include "glm/glm.hpp"
#include <memory>
namespace Engine
{

    struct PlayerController
    {
        glm::vec3 movementInput;
        glm::vec3 lookDirection;
        float wasFirePushed;
        /*
         * Units per second
         */
        float speed;

        PlayerController() = default;
        ~PlayerController();
        void SetMovementInput(int leftKey, int rightKey, int upKey, int downKey, int backKey, int frontKey);
        void SetFireInput(int key);

    private:
        std::shared_ptr<InputActionVec2> inputAction;
        std::shared_ptr<InputActionVec2> inputActionZ;
        std::shared_ptr<InputActionButton> fireAction;

        static void GetMovement(void* object, glm::vec2 input);
        static void GetMovementZ(void* object, glm::vec2 input);
        static void GetFireActionStart(void* object);
        static void GetFireActionEnd(void* object);
    };

} // Engine
