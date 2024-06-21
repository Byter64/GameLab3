#include "GlobalGameEvents.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include "CollisionLayer.h"
#include "ECSExtension.h"
#include "Scenes/01_TitleScreen.h"
#include "Scenes/02_Game.h"

extern GLFWwindow *window;

std::pair<Engine::Entity, Engine::Entity> players = {Engine::Entity::INVALID_ENTITY_ID, Engine::Entity::INVALID_ENTITY_ID};
int windowWidth, windowHeight;

void OnStartGame(int screenWidth, int screenHeight)
{
    for(int i = 0; i < GLFW_JOYSTICK_LAST && glfwJoystickPresent(i); i++)
    {
        std::cout << "Input" << std::to_string(i) << ": Detected " << glfwGetJoystickName(i) << (glfwJoystickIsGamepad(i) ? " as gamepad  " : "") << std::endl;
    }

    windowWidth = screenWidth;
    windowHeight = screenHeight;

    srand(std::chrono::system_clock::now().time_since_epoch().count());

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

    for(int i = 0; i <= (int)CollisionLayer::Ignore; i++)
    {
        Engine::Systems::collisionSystem->SetCollisionBetweenLayers((int)CollisionLayer::Ignore, i, false);
    }

    sceneManager->LoadScene<GameLost>();
}

void Update(float deltaTime)
{

}

void UpdateWithoutPause()
{

}

void OnEndGame()
{

}
