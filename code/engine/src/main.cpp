
#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Engine.h"
#include "GlobalGameEvents.h"

#define FRAMETIME60FPS 16667 //In microseconds, this is around 60 fps
#define FRAMETIME144FPS 6944 //In microseconds, this is around 144 fps

int screenWidth = 1920;
int screenHeight = 1080;
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
    Engine::Systems::inputSystem = std::make_shared<Engine::InputSystem>(window);

    Engine::OnStartGame(screenWidth, screenHeight);
    float passedTimeInSeconds = 1.0f/60;
    glfwSetTime(passedTimeInSeconds);

    while (!glfwWindowShouldClose(window)) {
        time1 = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        Engine::Systems::inputSystem->Update();

        Engine::Update(passedTimeInSeconds);

        Engine::Systems::collisionSystem->CheckCollisions();
        Engine::Systems::renderSystem->Render();
        Engine::Systems::textRenderSystem->Render();
        glfwSwapBuffers(window);
        ecsSystem->DeletePurgatory();

        auto time2 = std::chrono::high_resolution_clock::now();
        while(std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() < FRAMETIME144FPS)
            time2 = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
        passedTimeInSeconds = ((float)delta.count());
        passedTimeInSeconds = passedTimeInSeconds > FRAMETIME144FPS * 14 ? FRAMETIME144FPS * 14 : passedTimeInSeconds;
        passedTimeInSeconds /= 1000000;

    }

    Engine::OnEndGame();
    glfwTerminate();
    return 0;
}

int SetupWindow()
{
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(screenWidth, screenHeight, "GameLabIII", nullptr, nullptr);
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
    ecsSystem = std::make_shared<Engine::ECSSystem>();
    ecsSystem->Init();
    ecsSystem->RegisterComponent<Engine::Name>();
    ecsSystem->RegisterComponent<Engine::Transform>();
    ecsSystem->RegisterComponent<Engine::MeshRenderer>();
    ecsSystem->RegisterComponent<Engine::BoxCollider>();
    ecsSystem->RegisterComponent<Engine::Text>();

    ecsSystem->RegisterSystem<Engine::TransformParentSystem>();
    Engine::Signature transformSignature;
    transformSignature.set(ecsSystem->GetComponentType<Engine::Transform>());

    Engine::Systems::renderSystem = ecsSystem->RegisterSystem<Engine::RenderSystem>();
    Engine::Signature renderSignature;
    renderSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    renderSignature.set(ecsSystem->GetComponentType<Engine::MeshRenderer>());
    ecsSystem->SetSystemSignature<Engine::RenderSystem>(renderSignature);

    Engine::Systems::collisionSystem = ecsSystem->RegisterSystem<Engine::CollisionSystem>();
    Engine::Signature collisionSignature;
    collisionSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    collisionSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->SetSystemSignature<Engine::CollisionSystem>(collisionSignature);

    Engine::Systems::textRenderSystem = ecsSystem->RegisterSystem<Engine::TextRenderSystem>();
    Engine::Signature textRenderSignature;
    textRenderSignature.set(ecsSystem->GetComponentType<Engine::Text>());
    ecsSystem->SetSystemSignature<Engine::TextRenderSystem>(textRenderSignature);
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    time1 = std::chrono::high_resolution_clock::now();
    glViewport(0, 0, width, height);
    Engine::Systems::textRenderSystem->SetViewport(width, height);
}

void window_pos_callback(GLFWwindow *window, int x, int y)
{
    time1 = std::chrono::high_resolution_clock::now();
}