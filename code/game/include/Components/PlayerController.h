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
    float speed = 3.0f;

    bool hasScoreChanged = true;
    float stunnedTimer = 0;
    Engine::Entity uiTextScore = Engine::Entity::INVALID_ENTITY_ID;

    PlayerController() = default;
    PlayerController(PlayerController& other) = default;

    PlayerController& operator= (PlayerController& other) = default;
    PlayerController& operator= (PlayerController&& other);

    void SetMovementInput(int leftKey, int rightKey, int upKey, int downKey);
    void SetMovementInput(unsigned char joystickID, unsigned char leftButton, unsigned char rightButton, unsigned char upButton, unsigned char downButton);
    void SetMovementInput(unsigned char joystickID, unsigned char xAxis, unsigned char yAxis);
    void SetFireInput(int key);
    void SetFireInput(Engine::GamepadInputID button);
    void AddScore(int points);

private:
    long long int score = 0;

    std::shared_ptr<Engine::InputActionVec2> inputAction{};
    std::shared_ptr<Engine::InputActionButton> fireAction{};

    static void GetMovement(void* object, glm::vec2 input);
    static void GetMovementZ(void* object, glm::vec2 input);
    static void GetFireActionStart(void* object);
    static void GetFireActionEnd(void* object);
};