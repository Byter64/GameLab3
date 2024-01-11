#include "GameObject.h"
#include <stdexcept>
#include <string>
#include "GameObjectManager.h"

extern Engine::ECSSystem ecsSystem;
extern GameObjectManager gameObjectManager;

GameObject::GameObject(std::string name)
{
    entity = ecsSystem.CreateEntity();
    ecsSystem.AddComponent<Engine::Name>(entity, name);
    Engine::Transform& transform = ecsSystem.AddComponent<Engine::Transform>(entity);
    transform = Engine::Transform();
    parent = nullptr;
}

/**
 * Creates a GameObject from an entity. If this entity has a Transform Component with children, a GameObject for each entity in the transform hierarchy will be created
 * @param entity
 */
GameObject::GameObject(Engine::Entity entity)
{
    this->entity = entity;
    parent = nullptr;
    if(ecsSystem.HasComponent<Engine::Transform>(entity))
    {
        Engine::Transform& transform = ecsSystem.GetComponent<Engine::Transform>(entity);
        for(Engine::Transform *childTransform: transform.GetChildren())
        {
            GameObject* child = new GameObject(ecsSystem.GetEntity(*childTransform));
            children.push_back(child);
            child->parent = this;
        }
    }
}


GameObject::GameObject(GameObject &gameObject)
{
    entity = ecsSystem.CreateEntity();

    ecsSystem.AddComponent<Engine::Name>(entity, ecsSystem.GetComponent<Engine::Name>(gameObject.entity));

    Engine::Transform &transform = ecsSystem.AddComponent<Engine::Transform>(entity);
    transform = gameObject.GetComponent<Engine::Transform>();
    parent = nullptr;
    SetParent(gameObject.parent);

    if(ecsSystem.HasComponent<Engine::MeshRenderer>(gameObject.entity))
        ecsSystem.AddComponent<Engine::MeshRenderer>(entity, ecsSystem.GetComponent<Engine::MeshRenderer>(gameObject.entity));

    if(ecsSystem.HasComponent<Engine::BoxCollider>(gameObject.entity))
        ecsSystem.AddComponent<Engine::BoxCollider>(entity, ecsSystem.GetComponent<Engine::BoxCollider>(gameObject.entity));


    for (GameObject *originalChild: gameObject.children)
    {
        GameObject *child = new GameObject(*originalChild);
        child->SetParent(this);
    }
}


GameObject::~GameObject()
{
    if(parent != nullptr)
    {
        parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
    }

    for (GameObject* child :children)
    {
        delete child;
    }

    ecsSystem.DestroyEntity(entity);
}

unsigned int GameObject::GetChildCount()
{
    return children.size();
}

GameObject* GameObject::GetChild(unsigned int index)
{
    if(index >= children.size())
    {
        std::string message{"Index "};
        message += std::to_string(index);
        message += "was out of bounds";
        throw std::runtime_error(message);
    }

    return children[index];
}

void GameObject::SetParent(GameObject* parent)
{
    if(this->parent != nullptr)
    {
        auto reference = std::find(this->parent->children.begin(), this->parent->children.end(), this);
        if(reference != this->parent->children.end())
        {
            this->parent->children.erase(reference);
        }
    }
    if(parent != nullptr)
    {
        parent->children.push_back(this);
    }
    this->parent = parent;

    Engine::Transform* parentTransform = nullptr;
    if(parent != nullptr)
        parentTransform = &parent->GetComponent<Engine::Transform>();
    GetComponent<Engine::Transform>().SetParent(parentTransform);
}

void GameObject::SetUpdateMethod(void (*updateMethod)(float))
{
    if(!receivesGameEvents)
    {
        receivesGameEvents = true;
        Engine::GameEvents& updateEvent = ecsSystem.AddComponent<Engine::GameEvents>(entity);
        updateEvent.update = updateMethod;
    }
    else
    {
        Engine::GameEvents& updateEvent = ecsSystem.GetComponent<Engine::GameEvents>(entity);
        updateEvent.update = updateMethod;
    }

}

void GameObject::Destroy()
{
    gameObjectManager.AddToPurgatory(this);
}

/**
 * Creates a GameObject from a .gltf or .glb file. If the format is gltf, all data needs to be embedded into the .gltf
 * If the file only contains one root node, the GameObject for this root node is returned. If more than one root node exists,
 * a parent root node will be created and this one will be returned instead.
 * @param filePath The path to the gltf
 */
GameObject* GameObject::CreateGameObjectFromGLTF(std::filesystem::path filePath)
{
    std::vector<Engine::Entity> entities = Engine::ImportGLTF(filePath);

    if(entities.size() == 1)
    {
        GameObject* child = new GameObject(entities[0]);
        return child;
    }

    GameObject* gameObject = new GameObject(filePath.stem().string());
    for(Engine::Entity entity : entities)
    {
        GameObject* child = new GameObject(entity);
        child->SetParent(gameObject);
    }

    return gameObject;
}
