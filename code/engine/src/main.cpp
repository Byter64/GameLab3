
#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Engine.h"
#include "Dungeon.h"
#include "GlobalGameEvents.h"

#define FRAMETIME60FPS 16667 //In microseconds, this is around 60 fps
#define FRAMETIME144FPS 6944 //In microseconds, this is around 144 fps

Engine::InputSystem* inputSystem;

Engine::ECSSystem* ecsSystem; //Never change this name, as Systems depend on this symbol being declared
// somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::RenderSystem> renderSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::CollisionSystem> collisionSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::PlayerControllerSystem> playerControllerSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<Engine::BulletSystem> bulletSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<Engine::EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol
// being declared
// somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
GLFWwindow *window;

std::chrono::time_point<std::chrono::steady_clock> time1;

int SetupWindow();
void InitializeECS();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void window_pos_callback(GLFWwindow *window, int x, int y);


int main()
{

    if(SetupWindow() == -1) return -1;
    InitializeECS();
    Engine::OnStartGame();

    float passedTimeInSeconds = 1.0f/60;
    glfwSetTime(passedTimeInSeconds);
    while (!glfwWindowShouldClose(window)) {
        time1 = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        playerControllerSystem->Update(passedTimeInSeconds);
        bulletSystem->Update(passedTimeInSeconds);
        enemyBehaviourSystem->Update(passedTimeInSeconds);

        collisionSystem->CheckCollisions();

        renderSystem->Render();
        glfwSwapBuffers(window);
        ecsSystem->DeletePurgatory();

        auto time2 = std::chrono::high_resolution_clock::now();
        while(std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() < FRAMETIME144FPS)
            time2 = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
        passedTimeInSeconds = ((float)delta.count()) / 1000000;
        if(passedTimeInSeconds > FRAMETIME144FPS * 2)
        {
            passedTimeInSeconds = FRAMETIME144FPS * 2;
        }
    }

    Engine::OnEndGame();
    glfwTerminate();
    return 0;
}

int SetupWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(1920, 1080, "GameLabIII", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowPosCallback(window, window_pos_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}

void InitializeECS()
{
    ecsSystem = new Engine::ECSSystem();
    ecsSystem->Init();
    ecsSystem->RegisterComponent<Engine::Name>();
    ecsSystem->RegisterComponent<Engine::Transform>();
    ecsSystem->RegisterComponent<Engine::MeshRenderer>();
    ecsSystem->RegisterComponent<Engine::BoxCollider>();
    ecsSystem->RegisterComponent<Engine::PlayerController>();
    ecsSystem->RegisterComponent<Engine::Bullet>();
    ecsSystem->RegisterComponent<Engine::EnemyBehaviour>();
    ecsSystem->RegisterComponent<Engine::Health>();
    //When adding new components here, don't forget to add them to EntityUtilities::CopyEntity, too!!!!!

    ecsSystem->RegisterSystem<Engine::TransformParentSystem>();
    Engine::Signature transformSignature;
    transformSignature.set(ecsSystem->GetComponentType<Engine::Transform>());

    renderSystem = ecsSystem->RegisterSystem<Engine::RenderSystem>();
    Engine::Signature renderSignature;
    renderSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    renderSignature.set(ecsSystem->GetComponentType<Engine::MeshRenderer>());
    ecsSystem->SetSystemSignature<Engine::RenderSystem>(renderSignature);

    collisionSystem = ecsSystem->RegisterSystem<Engine::CollisionSystem>();
    Engine::Signature collisionSignature;
    collisionSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    collisionSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->SetSystemSignature<Engine::CollisionSystem>(collisionSignature);

    playerControllerSystem = ecsSystem->RegisterSystem<Engine::PlayerControllerSystem>();
    Engine::Signature  playerControllerSignature;
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::PlayerController>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->SetSystemSignature<Engine::PlayerControllerSystem>(playerControllerSignature);

    bulletSystem = ecsSystem->RegisterSystem<Engine::BulletSystem>();
    Engine::Signature bulletSignature;
    bulletSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    bulletSignature.set(ecsSystem->GetComponentType<Engine::Bullet>());
    bulletSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->SetSystemSignature<Engine::BulletSystem>(bulletSignature);

    enemyBehaviourSystem = ecsSystem->RegisterSystem<Engine::EnemyBehaviourSystem>();
    Engine::Signature enemyBehaviourSignature;
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::EnemyBehaviour>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::Health>());
    ecsSystem->SetSystemSignature<Engine::EnemyBehaviourSystem>(enemyBehaviourSignature);
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    time1 = std::chrono::high_resolution_clock::now();
    glViewport(0, 0, width, height);
}

void window_pos_callback(GLFWwindow *window, int x, int y)
{
    time1 = std::chrono::high_resolution_clock::now();
}