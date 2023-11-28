
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
//For whatever reason, these defines are not allowed to be written before glad is included (glad is also included in Engine.h)
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include "../../extern/tinygltf/tiny_gltf.h"

Engine::InputSystem* inputSystem;

Engine::ECSSystem ecsSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::RenderSystem> renderSystem;
GLFWwindow *window;

int SetupWindow();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void LoadGLTFTree(const tinygltf::Node& root, Engine::Transform* parent, std::shared_ptr<tinygltf::Model> model);

int main()
{
    if(SetupWindow() == -1) return -1;

    ecsSystem.Init();
    ecsSystem.RegisterComponent<Engine::Transform>();
    ecsSystem.RegisterComponent<Engine::MeshRenderer>();
    renderSystem = ecsSystem.RegisterSystem<Engine::RenderSystem>();
    Engine::Signature renderSignature;
    renderSignature.set(ecsSystem.GetComponentType<Engine::Transform>());
    ecsSystem.SetSystemSignature<Engine::RenderSystem>(renderSignature);

    auto temp = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 1000.0f);
    bool hasWorked;
    std::string error, warning;
    std::string path = "C:/Users/Yanni/Desktop/BonkItWand/BonkItWall.gltf";
    //std::string path = "C:/Users/Yanni/Desktop/BonkItWand/BonkItWall_Bar1.gltf";
    //std::string path = "C:/Users/Yanni/Desktop/Fliegengesicht.gltf";
    std::shared_ptr<tinygltf::Model> model = std::make_shared<tinygltf::Model>();
    tinygltf::TinyGLTF loader;

    hasWorked = loader.LoadASCIIFromFile(model.get(), &error, &warning, path);

    if (!hasWorked)
    {
        std::cout << error << std::endl;
        return -1;
    }

    for(int nodeIndex : model->scenes[0].nodes)
    {
        const tinygltf::Node& node = model->nodes[nodeIndex];
        LoadGLTFTree(node, nullptr, model);
    }


    glfwSetTime(1.0/60);
    float time = 0;
    renderSystem->camera.SetTranslation(glm::vec3(0,0,-10));
    while (!glfwWindowShouldClose(window)) {
        auto dir = glm::normalize(glm::vec3(-cosf(time), 0, -sinf(time)));
        auto rot = glm::quatLookAt(dir, glm::vec3(0, 1, 0));
        //renderSystem->camera.SetRotation(rot);

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

void LoadGLTFTree(const tinygltf::Node& root, Engine::Transform* parent, std::shared_ptr<tinygltf::Model> model)
{
    Engine::Entity entity = ecsSystem.CreateEntity();

    Engine::Transform transform;
    glm::vec3 translation{0};
    glm::vec3 scale{1};
    glm::quat rotation = glm::identity<glm::quat>();
    if (!root.translation.empty())
    { translation = glm::vec3{(float) root.translation[0], (float) root.translation[1], (float) root.translation[2]}; }
    if (!root.scale.empty())
    { scale = glm::vec3{(float) root.scale[0], (float) root.scale[1], (float) root.scale[2]}; }
    if (!root.rotation.empty())
    { rotation = glm::quat{ (float) root.rotation[3], (float) root.rotation[0], (float) root.rotation[1], (float) root.rotation[2]}; }

    transform.SetParent(parent);
    transform.SetTranslation(translation);
    transform.SetScale(scale);
    transform.SetRotation(rotation);
    ecsSystem.AddComponent(entity, transform);


    if(parent != nullptr)
        parent->AddChild(&ecsSystem.GetComponent<Engine::Transform>(entity));


    if (root.mesh != -1)
    {
        Engine::MeshRenderer meshRenderer = renderSystem->CreateMeshRenderer(model->meshes[root.mesh], model);
        ecsSystem.AddComponent(entity, meshRenderer);
    }

    for (int childIndex: root.children)
    {
        const tinygltf::Node &child = model->nodes[childIndex];
        LoadGLTFTree(child, &ecsSystem.GetComponent<Engine::Transform>(entity), model);
    }

}