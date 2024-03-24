#include "GlobalGameEvents.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include "CollisionLayer.h"
#include "ECSExtension.h"
#include "GameDefines.h"


extern std::shared_ptr<BulletSystem> bulletSystem;
extern std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<DungeonEnemySystem> dungeonEnemySystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<PlayerControllerSystem> playerControllerSystem; //Never change this name, as Systems depend on this symbol
extern GLFWwindow *window;

static std::shared_ptr<Engine::InputActionButton> pause;

static void PauseGame(void*);

void OnStartGame(int screenWidth, int screenHeight)
{
    Defines::InitializeDefines();
    ECSHelper::Initialize();
    Engine::Systems::textRenderSystem->Initialize(screenWidth, screenHeight);

    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Enemy), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Bullet), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Dungeon), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Dungeon), static_cast<unsigned char>(CollisionLayer::Enemy), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Dungeon), static_cast<unsigned char>(CollisionLayer::Dungeon), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Collectible), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Bullet), static_cast<unsigned char>(CollisionLayer::Bullet), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Enemy), static_cast<unsigned char>(CollisionLayer::Enemy), false);

    EnemyBehaviour::scores[EnemyBehaviour::Hubertus] = Defines::Int("Hubertus_Score");


    Engine::Entity dungeon = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Name>(dungeon, "Dungeon");
    ecsSystem->AddComponent(dungeon, Engine::Transform());
    ecsSystem->AddComponent<Dungeon>(dungeon, Dungeon(Engine::Files::ASSETS / "Dungeons", "Dungeon_"));

    enemyBehaviourSystem->Initialize(dungeonSystem->wallMap);

    Engine::Entity player1Text = ecsSystem->CreateEntity();
    auto& player1UI = ecsSystem->AddComponent<Engine::Text>(player1Text);
    player1UI.scale = 2;
    player1UI.SetText("Player 1");

    Engine::Entity scoreText = ecsSystem->CreateEntity();
    auto& scoreTextUI = ecsSystem->AddComponent<Engine::Text>(scoreText);
    scoreTextUI.scale = 2;
    scoreTextUI.position = {0, 35};
    scoreTextUI.SetText("Score:");

    Engine::Entity playerUI = ecsSystem->CreateEntity();
    auto& textUI = ecsSystem->AddComponent<Engine::Text>(playerUI);
    textUI.scale = 2;
    textUI.position = {110, 35};

    Engine::Entity player = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Player\\Player.glb")[0];
    PlayerController& controller = ecsSystem->AddComponent<PlayerController>(player);
    controller.uiTextScore = playerUI;
    controller.SetMovementInput(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_GAMEPAD_AXIS_LEFT_Y);
    controller.SetMovementInput(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, GLFW_GAMEPAD_BUTTON_DPAD_UP, GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
    controller.SetFireInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_A, Engine::GamepadInputID::Button});
    controller.SetFireInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_B, Engine::GamepadInputID::Button});
    controller.SetReviveInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_X, Engine::GamepadInputID::Button});
    controller.SetReviveInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_Y, Engine::GamepadInputID::Button});
    controller.speed = Defines::Float("Player1_Speed");
    controller.stunnedTime = Defines::Float("Player1_StunnedTime");
    controller.bulletSpeed = Defines::Float("Player1_BulletSpeed");
    controller.maxBullets = Defines::Int("Player1_MaxBullets");
    controller.spawnTime = Defines::Float("Player1_RespawnTime");

    pause = std::make_shared<Engine::InputActionButton>("Pause");
    pause->AddGamepadBinding({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button});
    pause->AddGamepadBinding({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button});
    pause->AddOnStart(nullptr, PauseGame);
    Engine::Systems::inputSystem->Add(pause);

    ecsSystem->AddComponent<Engine::BoxCollider>(player, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player).layer = static_cast<unsigned char>(CollisionLayer::Player);
    ecsSystem->AddComponent<Health>(player, Health{Defines::Int("Player1_Health")});

#define PLAYER2
#ifdef PLAYER2
    Engine::Entity player2Text = ecsSystem->CreateEntity();
    auto& player2UI = ecsSystem->AddComponent<Engine::Text>(player2Text);
    player2UI.scale = 2;
    player2UI.position = {0, 200};
    player2UI.SetText("Player 2");

    Engine::Entity scoreText2 = ecsSystem->CreateEntity();
    auto& scoreTextUI2 = ecsSystem->AddComponent<Engine::Text>(scoreText2);
    scoreTextUI2.scale = 2;
    scoreTextUI2.position = {0, 235};
    scoreTextUI2.SetText("Score:");

    Engine::Entity playerUI2 = ecsSystem->CreateEntity();
    auto& textUI2 = ecsSystem->AddComponent<Engine::Text>(playerUI2);
    textUI2.scale = 2;
    textUI2.position = {110, 235};

    Engine::Entity player2 = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Player\\Player.glb")[0];
    ecsSystem->GetComponent<Engine::Transform>(player2).SetTranslation({1, 0, 0});
    ecsSystem->GetComponent<Engine::Transform>(player).SetTranslation({-1, 0,  00});
    PlayerController& controller2 = ecsSystem->AddComponent<PlayerController>(player2);
    controller2.uiTextScore = playerUI2;
    controller2.SetMovementInput(GLFW_JOYSTICK_2, GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_GAMEPAD_AXIS_LEFT_Y);
    controller2.SetMovementInput(GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, GLFW_GAMEPAD_BUTTON_DPAD_UP, GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
    controller2.SetFireInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_A, Engine::GamepadInputID::Button});
    controller2.SetFireInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_B, Engine::GamepadInputID::Button});
    controller2.SetReviveInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_X, Engine::GamepadInputID::Button});
    controller2.SetReviveInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_Y, Engine::GamepadInputID::Button});
    controller2.speed = Defines::Float("Player2_Speed");
    controller2.stunnedTime = Defines::Float("Player2_StunnedTime");
    controller2.bulletSpeed = Defines::Float("Player2_BulletSpeed");
    controller2.maxBullets = Defines::Int("Player2_MaxBullets");
    controller2.spawnTime = Defines::Float("Player2_RespawnTime");

    pause->AddGamepadBinding({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button});
    pause->AddGamepadBinding({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button});
    Engine::Systems::inputSystem->Remove(pause);
    Engine::Systems::inputSystem->Add(pause);

    ecsSystem->AddComponent<Engine::BoxCollider>(player2, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player2).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player2).layer = static_cast<unsigned char>(CollisionLayer::Player);
    ecsSystem->AddComponent<Health>(player2, Health{Defines::Int("Player2_Health")});
#endif

    Engine::Systems::renderSystem->camera.SetTranslation(glm::vec3(0,0,-12));
    Engine::Systems::renderSystem->camera.SetScale(glm::vec3(1));
    Engine::Systems::renderSystem->camera.SetRotation(glm::vec3(glm::radians(-15.0f),0,0));

}

void OnEndGame()
{

}

void Update(float deltaTime)
{
    playerControllerSystem->Update(deltaTime);
    enemyBehaviourSystem->Update(deltaTime);
    dungeonSystem->Update();
    bulletSystem->Update(deltaTime);
}

static void PauseGame(void*)
{
    if(Engine::GetIsGamePaused())
        Engine::ContinueGame();
    else
        Engine::PauseGame(false);
}