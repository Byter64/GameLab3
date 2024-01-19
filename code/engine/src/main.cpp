
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
// being declared
// somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
GLFWwindow *window;

int SetupWindow();
void InitializeECS();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{

    if(SetupWindow() == -1) return -1;
    InitializeECS();
    Engine::OnStartGame();

    float passedTimeInSeconds = 1.0f/60;
    glfwSetTime(passedTimeInSeconds);
    while (!glfwWindowShouldClose(window)) {
        auto time1 = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        playerControllerSystem->Update(passedTimeInSeconds);
        collisionSystem->CheckCollisions();

        renderSystem->Render();
        glfwSwapBuffers(window);
        ecsSystem->DeletePurgatory();

        auto time2 = std::chrono::high_resolution_clock::now();
        while(std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() < FRAMETIME144FPS)
            time2 = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
        passedTimeInSeconds = ((float)delta.count()) / 1000000;
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
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}