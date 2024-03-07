#pragma once
#include "InputSystem/InputActionVec2.h"
#include "InputSystem/InputActionButton.h"
#include "glm/glm.hpp"
#include <memory>
#include "ECS/Entity.h"

namespace Engine
{

    struct PlayerController
    {
        friend class PlayerControllerSystem;
        glm::vec3 movementInput;
        glm::vec3 lookDirection;
        float wasFirePushed;
        /*
         * Units per second
         */
        float speed;

        bool hasScoreChanged = true;
        float stunnedTimer = 0;
        Entity uiTextScore = Entity::INVALID_ENTITY_ID;

        PlayerController() = default;
        ~PlayerController();
        void SetMovementInput(int leftKey, int rightKey, int upKey, int downKey, int backKey = -1, int frontKey = -1);
        void SetFireInput(int key);
        void AddScore(int points);

    private:
        long long int score = 0;

        std::shared_ptr<InputActionVec2> inputAction;
        std::shared_ptr<InputActionVec2> inputActionZ;
        std::shared_ptr<InputActionButton> fireAction;

        static void GetMovement(void* object, glm::vec2 input);
        static void GetMovementZ(void* object, glm::vec2 input);
        static void GetFireActionStart(void* object);
        static void GetFireActionEnd(void* object);
    };

} // Engine
