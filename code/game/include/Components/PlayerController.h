#pragma once
#include "glm/glm.hpp"
#include <memory>
#include "Engine.h"

struct PlayerController
{
    friend class PlayerControllerSystem;
    glm::vec3 movementInput{0, 0, 0};
    glm::vec3 lookDirection{1, 0, 0};
    bool wasFirePushed = false;

    float bulletSpeed = 0;
    int maxBullets = 2;
    int activeBullets = 0;

    float speed = 3.0f;
    bool hasScoreChanged = true;

    float stunnedTime = 5.0f;
    float stunnedTimer = 0;
    Engine::Entity uiTextScore = Engine::Entity::INVALID_ENTITY_ID;

    float respawnTime = 0;
    float respawnTimer = 0;

    PlayerController() = default;
    PlayerController(PlayerController& other) = default;

    PlayerController& operator= (PlayerController& other) = default;
    PlayerController& operator= (PlayerController&& other);
    ~PlayerController();

    void ResetInput();
    void AddMovementInput(int leftKey, int rightKey, int upKey, int downKey);
    void AddMovementInput(unsigned char joystickID, unsigned char leftButton, unsigned char rightButton, unsigned char upButton, unsigned char downButton);
    void AddMovementInput(unsigned char joystickID, unsigned char xAxis, unsigned char yAxis);
    void AddFireInput(int key);
    void AddFireInput(Engine::GamepadButton button);
    void AddScore(int points);

    int GetScore();
private:
    long long int score = 0;
    bool isActive = true;

    std::shared_ptr<Engine::InputActionVec2> inputAction = nullptr;
    std::shared_ptr<Engine::InputActionButton> fireAction = nullptr;

    static void GetMovement(void* object, glm::vec2 input);
    static void GetFireActionStart(void* object);
    static void GetFireActionEnd(void* object);
};