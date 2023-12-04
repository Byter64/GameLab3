
#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../engine/src/InputSystem.h"
#include "../../engine/src/InputActions/InputActionVec2.h"
#include "../../engine/src/Engine.h"

Engine::InputSystem* inputSystem;

Engine::ECSSystem ecsSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::RenderSystem> renderSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
GLFWwindow *window;
Engine::Entity krawatterich;
Engine::Entity root;

int SetupWindow();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void LoadGLTFTree(const tinygltf::Node& root, Engine::Transform* parent, std::shared_ptr<tinygltf::Model> model);

int main()
{
    if(SetupWindow() == -1) return -1;

    ecsSystem.Init();
    ecsSystem.RegisterComponent<Engine::Name>();
    ecsSystem.RegisterComponent<Engine::Transform>();
    ecsSystem.RegisterComponent<Engine::MeshRenderer>();
    renderSystem = ecsSystem.RegisterSystem<Engine::RenderSystem>();
    Engine::Signature renderSignature;
    renderSignature.set(ecsSystem.GetComponentType<Engine::Transform>());
    ecsSystem.SetSystemSignature<Engine::RenderSystem>(renderSignature);

    auto temp = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 1000.0f);

    std::filesystem::path path = "C:/Users/Yanni/Desktop/Fliegengesicht.gltf";
    root = ecsSystem.CreateEntity();
    ecsSystem.AddComponent<Engine::Name>(root, "Root");
    ecsSystem.AddComponent<Engine::Transform>(root, Engine::Transform());

    for(Engine::Entity entity : Engine::ImportGLTF(path))
    {
        ecsSystem.GetComponent<Engine::Transform>(entity).SetParent(&ecsSystem.GetComponent<Engine::Transform>(root));
    }
    krawatterich = Engine::FindChild(root, "Krawatterich");

    GLuint vertexColorshader = renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Default/FS_Default_VertexColor.frag", GL_FRAGMENT_SHADER);
    Engine::Entity vertexPaintedSphere1 = Engine::FindChild(root, "VertexPaintedSphere");
    ecsSystem.GetComponent<Engine::MeshRenderer>(vertexPaintedSphere1).primitiveData[0].shader.SetFragmentShader(vertexColorshader);
    Engine::Entity vertexPaintedSphere2 = Engine::FindChild(root, "VertexPaintedSphere.001");
    ecsSystem.GetComponent<Engine::MeshRenderer>(vertexPaintedSphere2).primitiveData[0].shader.SetFragmentShader(vertexColorshader);

    glfwSetTime(1.0/60);
    float time = 0;
    renderSystem->camera.SetTranslation(glm::vec3(0,-12,-15));
    while (!glfwWindowShouldClose(window)) {
        auto dir = glm::normalize(glm::vec3(-cosf(time * 1.1f), cosf(time * 1.2f), -sinf(time)));
        auto rot = glm::quatLookAt(dir, glm::vec3(0, 1, 0));
        ecsSystem.GetComponent<Engine::Transform>(krawatterich).SetTranslation(dir * 5.0f);

        auto time1 = std::chrono::high_resolution_clock::now();

        glClearColor(0.172f, 0.243f, 0.313f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderSystem->Render();
        glfwSwapBuffers(window);
        glfwPollEvents();


        auto time2 = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1);
        time +=  delta.count() / 1000.0f;

        float pla = 180 / 3.14159f;
        std::cout << delta.count() << " ms passed" << "\n";
    }

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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}