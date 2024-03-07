#include "GlobalGameEvents.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include "CollisionLayer.h"

extern Engine::InputSystem* inputSystem;

extern std::shared_ptr<Engine::RenderSystem> renderSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::CollisionSystem> collisionSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::DungeonSystem> dungeonSystem;
extern std::shared_ptr<Engine::TextRenderSystem> textRenderSystem;
extern GLFWwindow *window;

void Engine::OnStartGame(int screenWidth, int screenHeight)
{
    inputSystem = new Engine::InputSystem(window);
    textRenderSystem->Initialize(screenWidth, screenHeight);

    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Enemy, CollisionLayer::Collectible, false);
    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Bullet, CollisionLayer::Collectible, false);
    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Dungeon, CollisionLayer::Collectible, false);
    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Dungeon, CollisionLayer::Enemy, false);
    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Dungeon, CollisionLayer::Dungeon, false);
    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Collectible, CollisionLayer::Collectible, false);
    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Bullet, CollisionLayer::Bullet, false);
    collisionSystem->SetCollisionBetweenLayers(CollisionLayer::Enemy, CollisionLayer::Enemy, false);

    Engine::Entity dungeon = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Name>(dungeon, "Dungeon");
    ecsSystem->AddComponent(dungeon, Engine::Transform());
    ecsSystem->AddComponent<Engine::Dungeon>(dungeon, Engine::Dungeon(Engine::Files::ASSETS / "Dungeons", "Dungeon_"));

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
    Engine::PlayerController& controller = ecsSystem->AddComponent<Engine::PlayerController>(player);
    controller.speed = 2;
    controller.uiTextScore = playerUI;
    controller.SetMovementInput(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_Q, GLFW_KEY_E);
    controller.SetFireInput(GLFW_KEY_SPACE);
    ecsSystem->AddComponent<Engine::BoxCollider>(player, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player).layer = CollisionLayer::Player;
    ecsSystem->AddComponent<Engine::Health>(player, Engine::Health{1});

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
    ecsSystem->GetComponent<Engine::Transform>(player).SetTranslation({-1, 0, 0});
    Engine::PlayerController& controller2 = ecsSystem->AddComponent<Engine::PlayerController>(player2);
    controller2.speed = 2;
    controller2.uiTextScore = playerUI2;
    controller2.SetMovementInput(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN);
    controller2.SetFireInput(GLFW_KEY_KP_0);
    ecsSystem->AddComponent<Engine::BoxCollider>(player2, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player2).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player2).layer = CollisionLayer::Player;
    ecsSystem->AddComponent<Engine::Health>(player2, Engine::Health{1});
#endif

    renderSystem->camera.SetTranslation(glm::vec3(0,0,-12));
    renderSystem->camera.SetScale(glm::vec3(1));
    renderSystem->camera.SetRotation(glm::vec3(glm::radians(-15.0f),0,0));
}

void Engine::OnEndGame()
{

}

void Engine::Update(float deltaTime)
{

}