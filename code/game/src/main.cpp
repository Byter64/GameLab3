
#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Dungeon.h"

#define FRAMETIME60FPS 16667 //In microseconds, this is around 60 fps
#define FRAMETIME144FPS 6944 //In microseconds, this is around 144 fps

Engine::InputSystem* inputSystem;

Engine::ECSSystem ecsSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::RenderSystem> renderSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::GameEventSystem> gameEventSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
std::shared_ptr<Engine::CollisionSystem> collisionSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
GameObjectManager gameObjectManager;
GLFWwindow *window;
GameObject *krawatterich;
Engine::Entity root;

int SetupWindow();
void InitializeECS();
void LoadDemo();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
    if(SetupWindow() == -1) return -1;
    InitializeECS();

    //LoadDemo();
    GameObject* wallPrefab = GameObject::CreateGameObjectFromGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Wall\\Wall.glb");
    Engine::BoxCollider& wallCollider = wallPrefab->AddComponent<Engine::BoxCollider>();
    wallCollider.size = glm::vec3(1);
    wallCollider.position = glm::vec3 (0);
    wallCollider.isStatic = true;

    Dungeon* dungeon = new Dungeon(std::filesystem::path(Engine::Files::ASSETS / "Dungeon4_3.png"), wallPrefab);
    dungeon->gameObject->GetComponent<Engine::Transform>().AddTranslation(glm::vec3(0, 0,0));

    wallPrefab->Destroy();

    glfwSetTime(1.0/60);
    float passedTimeInSeconds = 1.0f/60;
    renderSystem->camera.SetTranslation(glm::vec3(0,0,-15));
    renderSystem->camera.SetScale(glm::vec3(1));
    while (!glfwWindowShouldClose(window)) {
        auto time1 = std::chrono::high_resolution_clock::now();


        glfwPollEvents();
        gameEventSystem->InvokeUpdate(passedTimeInSeconds);
        collisionSystem->CheckCollisions();

        renderSystem->Render();
        glfwSwapBuffers(window);
        gameObjectManager.DeletePurgatory();

        auto time2 = std::chrono::high_resolution_clock::now();
        while(std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() < FRAMETIME144FPS)
            time2 = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1);
        passedTimeInSeconds = ((float)delta.count()) / 1000000;
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

void InitializeECS()
{
    ecsSystem.Init();
    ecsSystem.RegisterComponent<Engine::Name>();
    ecsSystem.RegisterComponent<Engine::Transform>();
    ecsSystem.RegisterComponent<Engine::MeshRenderer>();
    ecsSystem.RegisterComponent<Engine::GameEvents>();
    ecsSystem.RegisterComponent<Engine::BoxCollider>();

    renderSystem = ecsSystem.RegisterSystem<Engine::RenderSystem>();
    Engine::Signature renderSignature;
    renderSignature.set(ecsSystem.GetComponentType<Engine::Transform>());
    renderSignature.set(ecsSystem.GetComponentType<Engine::MeshRenderer>());
    ecsSystem.SetSystemSignature<Engine::RenderSystem>(renderSignature);

    gameEventSystem = ecsSystem.RegisterSystem<Engine::GameEventSystem>();
    Engine::Signature gameEventSignature;
    gameEventSignature.set(ecsSystem.GetComponentType<Engine::GameEvents>());
    ecsSystem.SetSystemSignature<Engine::GameEventSystem>(gameEventSignature);

    collisionSystem = ecsSystem.RegisterSystem<Engine::CollisionSystem>();
    Engine::Signature collisionSignature;
    collisionSignature.set(ecsSystem.GetComponentType<Engine::Transform>());
    collisionSignature.set(ecsSystem.GetComponentType<Engine::BoxCollider>());
    ecsSystem.SetSystemSignature<Engine::CollisionSystem>(collisionSignature);
}

float myTime = 0;
glm::vec3 movement;
void UpdateTest(float time)
{
    myTime += time;
    auto dir = glm::normalize(glm::vec3(-cosf(myTime * 1.1f), cosf(myTime * 1.2f), -sinf(myTime)));
    auto rot = glm::quatLookAt(dir, glm::vec3(0, 1, 0));
    krawatterich->GetComponent<Engine::Transform>().AddTranslation(movement * time);
}

void Enter(Engine::BoxCollider& coll1, Engine::BoxCollider& coll2)
{
    std::cout << ecsSystem.GetComponent<Engine::Name>(ecsSystem.GetEntity(coll1)) << " entered collision with "
              << ecsSystem.GetComponent<Engine::Name>(ecsSystem.GetEntity(coll2)) << "\n";
}
void Stay(Engine::BoxCollider& coll1, Engine::BoxCollider& coll2)
{
    std::cout << ecsSystem.GetComponent<Engine::Name>(ecsSystem.GetEntity(coll1)) << " stays in collision with "
              << ecsSystem.GetComponent<Engine::Name>(ecsSystem.GetEntity(coll2)) << "\n";
}
void Exit(Engine::BoxCollider& coll1, Engine::BoxCollider& coll2)
{
    std::cout << ecsSystem.GetComponent<Engine::Name>(ecsSystem.GetEntity(coll1)) << " exited collision with "
              << ecsSystem.GetComponent<Engine::Name>(ecsSystem.GetEntity(coll2)) << "\n";
}

void MovementXY(glm::vec2 input)
{
    movement.x = input.x;
    movement.y = input.y;

    if(movement != glm::vec3(0,0,0))
        movement = glm::normalize(movement) * 8.0f;
}

void MovementZ(glm::vec2 input)
{
    movement.z = input.x;

    if(movement != glm::vec3(0,0,0))
        movement = glm::normalize(movement) * 8.0f;
}

void LoadDemo()
{
    inputSystem = new Engine::InputSystem(window);

    std::shared_ptr<Engine::InputActionVec2> movementXY = std::make_shared<Engine::InputActionVec2>("MovementXY");
    movementXY->AddKeyboardBinding(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    movementXY->AddOnValueChange(MovementXY);

    inputSystem->Add(movementXY);


    std::shared_ptr<Engine::InputActionVec2> movementZ = std::make_shared<Engine::InputActionVec2>("MovementZ");
    movementZ->AddKeyboardBinding(GLFW_KEY_E, GLFW_KEY_Q, 0, 0);
    movementZ->AddOnValueChange(MovementZ);
    inputSystem->Add(movementZ);

    std::filesystem::path path = "C:/Users/Yanni/Desktop/Fliegengesicht.gltf";
    root = ecsSystem.CreateEntity();
    ecsSystem.AddComponent<Engine::Name>(root, "Root");
    ecsSystem.AddComponent<Engine::Transform>(root, Engine::Transform());

    for(Engine::Entity entity : Engine::ImportGLTF(path))
    {
        ecsSystem.GetComponent<Engine::Transform>(entity).SetParent(&ecsSystem.GetComponent<Engine::Transform>(root));
    }
    krawatterich = new GameObject(Engine::FindChild(root, "Krawatterich"));
    krawatterich->SetUpdateMethod(UpdateTest);
    Engine::BoxCollider& boxCollider = krawatterich->AddComponent<Engine::BoxCollider>();
    boxCollider.onCollisionStay = Stay;
    boxCollider.onCollisionEnter = Enter;
    boxCollider.onCollisionExit = Exit;

    GLuint vertexColorshader = renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Default/FS_Default_VertexColor.frag", GL_FRAGMENT_SHADER);
    Engine::Entity vertexPaintedSphere1 = Engine::FindChild(root, "VertexPaintedSphere");
    ecsSystem.GetComponent<Engine::MeshRenderer>(vertexPaintedSphere1).primitiveData[0].shader.SetFragmentShader(vertexColorshader);
    Engine::Entity vertexPaintedSphere2 = Engine::FindChild(root, "VertexPaintedSphere.001");
    ecsSystem.GetComponent<Engine::MeshRenderer>(vertexPaintedSphere2).primitiveData[0].shader.SetFragmentShader(vertexColorshader);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}