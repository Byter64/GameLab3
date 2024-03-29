#include "GlobalGameEvents.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include "CollisionLayer.h"
#include "ECSExtension.h"
#include "GameDefines.h"


extern std::shared_ptr<BulletSystem> bulletSystem;
extern std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<DungeonEnemySystem> dungeonEnemySystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<PlayerControllerSystem> playerControllerSystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<ElevatorSystem> elevatorSystem;
extern GLFWwindow *window;

static std::shared_ptr<Engine::InputActionButton> pause;

static void PauseGame(void*);
static float pauseStartTime;
static Engine::Entity pauseText;
static float pauseTextScale = 8;
static Dungeon* dungeon;

std::pair<Engine::Entity, Engine::Entity> players = {Engine::Entity::INVALID_ENTITY_ID, Engine::Entity::INVALID_ENTITY_ID};

void OnStartGame(int screenWidth, int screenHeight)
{
    Defines::InitializeDefines();
    ECSHelper::Initialize();
    Engine::Systems::textRenderSystem->Initialize(screenWidth, screenHeight);
    Engine::Systems::textRenderSystem->SetReferenceResolution(screenWidth, screenHeight);

    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Enemy), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Bullet), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Dungeon), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Dungeon), static_cast<unsigned char>(CollisionLayer::Enemy), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Dungeon), static_cast<unsigned char>(CollisionLayer::Dungeon), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Collectible), static_cast<unsigned char>(CollisionLayer::Collectible), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Bullet), static_cast<unsigned char>(CollisionLayer::Bullet), false);
    Engine::Systems::collisionSystem->SetCollisionBetweenLayers(static_cast<unsigned char>(CollisionLayer::Enemy), static_cast<unsigned char>(CollisionLayer::Enemy), false);

    EnemyBehaviour::scores[EnemyBehaviour::Hubertus] = Defines::Int("Hubertus_Score");
    EnemyBehaviour::scores[EnemyBehaviour::KindredSpirit] = Defines::Int("KindredSpirit_Score");
    EnemyBehaviour::scores[EnemyBehaviour::Assi] = Defines::Int("Assi_Score");

    pauseText = ecsSystem->CreateEntity();
    Engine::Text& pauseTextText = ecsSystem->AddComponent<Engine::Text>(pauseText);
    pauseTextText.scale = 0;
    pauseTextText.position = {screenWidth / 2, screenHeight / 2};
    pauseTextText.horizontalAlignment = Engine::Text::Center;
    pauseTextText.verticalAlignment = Engine::Text::Center;
    pauseTextText.SetText("Game Paused");

    Engine::Entity dungeon = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Name>(dungeon, "Dungeon");
    ecsSystem->AddComponent(dungeon, Engine::Transform());
    ecsSystem->AddComponent<Dungeon>(dungeon, Dungeon(Engine::Files::ASSETS / "Dungeons", "Dungeon_"));
    ::dungeon = &ecsSystem->GetComponent<Dungeon>(dungeon);

    Engine::Entity playersText = ecsSystem->CreateEntity();
    auto& playersUI = ecsSystem->AddComponent<Engine::Text>(playersText);
    playersUI.scale = 4;
    playersUI.position = {890, 0};
    playersUI.SetText("Both");

    Engine::Entity playersUIActualScore = ecsSystem->CreateEntity();
    auto& playersTextUI = ecsSystem->AddComponent<Engine::Text>(playersUIActualScore);
    playersTextUI.scale = 4;
    playersTextUI.position = {960, 80};
    playersTextUI.horizontalAlignment = Engine::Text::Center;
    playerControllerSystem->scoreUI = playersUIActualScore;


    Engine::Entity player1Text = ecsSystem->CreateEntity();
    auto& player1UI = ecsSystem->AddComponent<Engine::Text>(player1Text);
    player1UI.scale = 4;
    player1UI.position = {200, 0};
    player1UI.SetText("Player 1");

    Engine::Entity playerUI = ecsSystem->CreateEntity();
    auto& textUI = ecsSystem->AddComponent<Engine::Text>(playerUI);
    textUI.scale = 4;
    textUI.position = {330, 80};
    textUI.horizontalAlignment = Engine::Text::Center;


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
    controller.respawnTime = Defines::Float("Player1_RespawnTime");

    pause = std::make_shared<Engine::InputActionButton>("Pause");
    pause->AddGamepadBinding({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button});
    pause->AddGamepadBinding({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button});
    pause->AddOnStart(nullptr, PauseGame);
    Engine::Systems::inputSystem->Add(pause);

    ecsSystem->AddComponent<Engine::BoxCollider>(player, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player).layer = static_cast<unsigned char>(CollisionLayer::Player);
    ecsSystem->AddComponent<Health>(player, Health{Defines::Int("Player1_Health")});
    players.first = player;

//#define PLAYER2
#ifdef PLAYER2
    Engine::Entity player2Text = ecsSystem->CreateEntity();
    auto& player2UI = ecsSystem->AddComponent<Engine::Text>(player2Text);
    player2UI.scale = 4;
    player2UI.position = {1400, 0};
    player2UI.SetText("Player 2");

    Engine::Entity playerUI2 = ecsSystem->CreateEntity();
    auto& textUI2 = ecsSystem->AddComponent<Engine::Text>(playerUI2);
    textUI2.scale = 4;
    textUI2.position = {1530, 80};
    textUI2.horizontalAlignment = Engine::Text::Center;


    Engine::Entity player2 = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Player\\Player.glb")[0];
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
    controller2.respawnTime = Defines::Float("Player2_RespawnTime");

    pause->AddGamepadBinding({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button});
    pause->AddGamepadBinding({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button});
    Engine::Systems::inputSystem->Remove(pause);
    Engine::Systems::inputSystem->Add(pause);

    ecsSystem->AddComponent<Engine::BoxCollider>(player2, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player2).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player2).layer = static_cast<unsigned char>(CollisionLayer::Player);
    ecsSystem->AddComponent<Health>(player2, Health{Defines::Int("Player2_Health")});
    players.second = player2;
#endif

    Engine::Systems::renderSystem->camera.SetTranslation(glm::vec3(0,2,-12));
    Engine::Systems::renderSystem->camera.SetScale(glm::vec3(1));
    Engine::Systems::renderSystem->camera.SetRotation(glm::vec3(glm::radians(-15.0f),0,0));

    dungeonSystem->InitializeDungeons();
    enemyBehaviourSystem->Initialize(dungeonSystem->wallMap);
}

void OnEndGame()
{

}

void Update(float deltaTime)
{
    playerControllerSystem->Update(deltaTime);
    enemyBehaviourSystem->Update(deltaTime);
    dungeonSystem->Update();
    elevatorSystem->Update();
    bulletSystem->Update(deltaTime);

    //Number of components needs to be 1 because the prefab will always exist
    if(dungeon->areAllEnemiesDefeated && ecsSystem->GetNumberOfComponents<Loot>() == 1)
        dungeonSystem->UpdateDungeon(ecsSystem->GetEntity(*dungeon));
}

void UpdateWithoutPause()
{
    if(!Engine::GetIsGamePaused()) return;
    float delta = Engine::Systems::timeManager->GetTimeSinceStartupWithoutPauses() - pauseStartTime;
    if(((int)delta) % 2 == 0)
    {
        Engine::Text& text = ecsSystem->GetComponent<Engine::Text>(pauseText);
        text.scale = pauseTextScale;
    }
    else
    {
        Engine::Text& text = ecsSystem->GetComponent<Engine::Text>(pauseText);
        text.scale = 0;
    }
}

static void PauseGame(void*)
{
    if(Engine::GetIsGamePaused())
    {
        Engine::ContinueGame();
        ecsSystem->GetComponent<Engine::Text>(pauseText).scale = 0;
    }
    else
    {
        Engine::PauseGame();
        pauseStartTime = Engine::Systems::timeManager->GetTimeSinceStartupWithoutPauses();
    }
}