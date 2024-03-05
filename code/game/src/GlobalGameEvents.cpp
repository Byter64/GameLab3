#include "GlobalGameEvents.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

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
    ecsSystem->AddComponent<Engine::Health>(player, Engine::Health{1});

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