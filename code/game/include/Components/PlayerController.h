#pragma once
#include "glm/glm.hpp"
#include <memory>
#include "Engine.h"

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
    Engine::Entity uiTextScore = Engine::Entity::INVALID_ENTITY_ID;

    PlayerController() = default;
    PlayerController(PlayerController& other) = default;

    PlayerController& operator= (PlayerController& other) = default;
    PlayerController& operator= (PlayerController&& other);

    void SetMovementInput(int leftKey, int rightKey, int upKey, int downKey, int backKey = -1, int frontKey = -1);
    void SetFireInput(int key);
    void AddScore(int points);

private:
    long long int score = 0;

    std::shared_ptr<Engine::InputActionVec2> inputAction{};
    std::shared_ptr<Engine::InputActionVec2> inputActionZ{};
    std::shared_ptr<Engine::InputActionButton> fireAction{};

    static void GetMovement(void* object, glm::vec2 input);
    static void GetMovementZ(void* object, glm::vec2 input);
    static void GetFireActionStart(void* object);
    static void GetFireActionEnd(void* object);
};