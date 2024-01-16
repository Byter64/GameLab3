#include "GlobalGameEvents.h"
#include "Engine.h"
#include "GameObject.h"
#include "Dungeon.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

extern Engine::InputSystem* inputSystem;

extern Engine::ECSSystem ecsSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::RenderSystem> renderSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::CollisionSystem> collisionSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern GLFWwindow *window;

GameObject *krawatterich;
Engine::Entity root;
void LoadDemo();

void Engine::OnStartGame()
{
    //LoadDemo();
    inputSystem = new Engine::InputSystem(window);

    GameObject* wallPrefab = GameObject::CreateGameObjectFromGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Wall\\Wall.glb");
    Engine::BoxCollider& wallCollider = wallPrefab->AddComponent<Engine::BoxCollider>();
    wallCollider.size = glm::vec3(1);
    wallCollider.position = glm::vec3 (0);
    wallCollider.isStatic = true;

    Dungeon* dungeon = new Dungeon(std::filesystem::path(Engine::Files::ASSETS / "Dungeon4_3.png"), wallPrefab);
    dungeon->gameObject->GetComponent<Engine::Transform>().AddTranslation(glm::vec3(0, 0,0));

    wallPrefab->Destroy();
    renderSystem->camera.SetTranslation(glm::vec3(0,0,-15));
    renderSystem->camera.SetScale(glm::vec3(1));
}

void Engine::OnEndGame()
{

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

void MovementXY(void*, glm::vec2 input)
{
    movement.x = input.x;
    movement.y = input.y;

    if(movement != glm::vec3(0,0,0))
        movement = glm::normalize(movement) * 8.0f;
}

void MovementZ(void*, glm::vec2 input)
{
    movement.z = input.x;

    if(movement != glm::vec3(0,0,0))
        movement = glm::normalize(movement) * 8.0f;
}

void LoadDemo()
{

    std::shared_ptr<Engine::InputActionVec2> movementXY = std::make_shared<Engine::InputActionVec2>("MovementXY");
    movementXY->AddKeyboardBinding(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    movementXY->AddOnValueChange(nullptr, MovementXY);

    inputSystem->Add(movementXY);


    std::shared_ptr<Engine::InputActionVec2> movementZ = std::make_shared<Engine::InputActionVec2>("MovementZ");
    movementZ->AddKeyboardBinding(GLFW_KEY_E, GLFW_KEY_Q, 0, 0);
    movementZ->AddOnValueChange(nullptr, MovementZ);
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
    //boxCollider.onCollisionStay = Stay;
    //boxCollider.onCollisionEnter = Enter;
    //boxCollider.onCollisionExit = Exit;

    GLuint vertexColorshader = renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Default/FS_Default_VertexColor.frag", GL_FRAGMENT_SHADER);
    Engine::Entity vertexPaintedSphere1 = Engine::FindChild(root, "VertexPaintedSphere");
    ecsSystem.GetComponent<Engine::MeshRenderer>(vertexPaintedSphere1).primitiveData[0].shader.SetFragmentShader(vertexColorshader);
    Engine::Entity vertexPaintedSphere2 = Engine::FindChild(root, "VertexPaintedSphere.001");
    ecsSystem.GetComponent<Engine::MeshRenderer>(vertexPaintedSphere2).primitiveData[0].shader.SetFragmentShader(vertexColorshader);
}