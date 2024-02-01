#include "GlobalGameEvents.h"
#include "Engine.h"
#include "Dungeon.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

extern Engine::InputSystem* inputSystem;

extern std::shared_ptr<Engine::RenderSystem> renderSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::CollisionSystem> collisionSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern GLFWwindow *window;

Engine::Entity krawatterich;
Engine::Entity root;

void Engine::OnStartGame()
{
    inputSystem = new Engine::InputSystem(window);

    Engine::Entity wallPrefab = Engine::ImportGLTF(Engine::Files::ASSETS /
            "Graphics\\Models\\Wall\\Wall.glb")[0];
    Engine::BoxCollider& wallCollider = ecsSystem->AddComponent<Engine::BoxCollider>(wallPrefab);
    wallCollider.size = glm::vec3(1);
    wallCollider.position = glm::vec3 (0);
    wallCollider.isStatic = true;

    Dungeon* dungeon = new Dungeon(std::filesystem::path(Engine::Files::ASSETS / "Dungeon4_3.png"), wallPrefab);
    ecsSystem->GetComponent<Engine::Transform>(dungeon->entity).AddTranslation(glm::vec3(0, 0, 0));
    ecsSystem->DestroyEntity(wallPrefab);
    enemyBehaviourSystem->Initialize(dungeon->wallMap);

    Engine::Entity player = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Player\\Player.glb")[0];
    ecsSystem->GetComponent<Engine::Transform>(player).SetScale(glm::vec3(0.5f));
    Engine::PlayerController& controller = ecsSystem->AddComponent<Engine::PlayerController>(player);
    controller.speed = 2;
    controller.SetMovementInput(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_Q, GLFW_KEY_E);
    controller.SetFireInput(GLFW_KEY_SPACE);
    ecsSystem->AddComponent<Engine::BoxCollider>(player, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player).size = glm::vec3(0.8f);

    Engine::Entity enemy = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Hubertus\\Hubertus.glb")[0];
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->AddComponent<Engine::EnemyBehaviour>(enemy, Engine::EnemyBehaviour());




    renderSystem->camera.SetTranslation(glm::vec3(0,0,-15));
    renderSystem->camera.SetScale(glm::vec3(1));
    renderSystem->camera.SetRotation(glm::vec3(glm::radians(-22.5f),0,0));
}

void Engine::OnEndGame()
{

}

glm::vec3 movement;

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
    root = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Name>(root, "Root");
    ecsSystem->AddComponent<Engine::Transform>(root, Engine::Transform());

    for(Engine::Entity entity : Engine::ImportGLTF(path))
    {
        ecsSystem->GetComponent<Engine::Transform>(entity).SetParent(&ecsSystem->GetComponent<Engine::Transform>(root));
    }
    krawatterich = Engine::FindChild(root, "Krawatterich");

    Engine::BoxCollider& boxCollider = ecsSystem->AddComponent<Engine::BoxCollider>(krawatterich);

    GLuint vertexColorshader = renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Default/FS_Default_VertexColor.frag", GL_FRAGMENT_SHADER);
    Engine::Entity vertexPaintedSphere1 = Engine::FindChild(root, "VertexPaintedSphere");
    ecsSystem->GetComponent<Engine::MeshRenderer>(vertexPaintedSphere1).primitiveData[0].shader.SetFragmentShader
    (vertexColorshader);
    Engine::Entity vertexPaintedSphere2 = Engine::FindChild(root, "VertexPaintedSphere.001");
    ecsSystem->GetComponent<Engine::MeshRenderer>(vertexPaintedSphere2).primitiveData[0].shader.SetFragmentShader
    (vertexColorshader);
}