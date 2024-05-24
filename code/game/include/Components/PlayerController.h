#pragma once
#include "glm/glm.hpp"
#include <memory>
#include "Engine.h"

struct PlayerController
{
    friend class PlayerControllerSystem;
    glm::vec3 movementInput;
    glm::vec3 lookDirection{1, 0, 0};
    bool wasFirePushed;
    bool wasRevivePushed;

    float bulletSpeed;
    int maxBullets = 2;
    int activeBullets = 0;

    float speed = 3.0f;
    bool hasScoreChanged = true;

    float stunnedTime = 5.0f;
    float stunnedTimer = 0;
    Engine::Entity uiTextScore = Engine::Entity::INVALID_ENTITY_ID;

    float respawnTime;
    float respawnTimer;

    PlayerController() = default;
    PlayerController(PlayerController& other) = default;

    PlayerController& operator= (PlayerController& other) = default;
    PlayerController& operator= (PlayerController&& other);

    void ResetInput();
    void AddMovementInput(int leftKey, int rightKey, int upKey, int downKey);
    void AddMovementInput(unsigned char joystickID, unsigned char leftButton, unsigned char rightButton, unsigned char upButton, unsigned char downButton);
    void AddMovementInput(unsigned char joystickID, unsigned char xAxis, unsigned char yAxis);
    void AddFireInput(int key);
    void AddFireInput(Engine::GamepadInputID button);
    void AddReviveInput(int button);
    void AddReviveInput(Engine::GamepadInputID button);
    void AddScore(int points);

    int GetScore();
private:
    long long int score = 0;
    bool isActive = true;

    std::shared_ptr<Engine::InputActionVec2> inputAction{};
    std::shared_ptr<Engine::InputActionButton> fireAction{};
    std::shared_ptr<Engine::InputActionButton> reviveAction{};

    static void GetMovement(void* object, glm::vec2 input);
    static void GetMovementZ(void* object, glm::vec2 input);
    static void GetFireActionStart(void* object);
    static void GetFireActionEnd(void* object);
    static void GetReviveActionStart(void* object);
};