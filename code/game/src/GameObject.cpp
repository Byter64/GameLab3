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
}

/**
 * Creates a GameObject from an entity. If this entity has a Transform Component with children, a GameObject for each entity in the transform hierarchy will be created
 * @param entity
 */
GameObject::GameObject(Engine::Entity entity)
{
    this->entity = entity;
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
        this->parent->children.erase(std::find(this->parent->children.begin(), this->parent->children.end(), this));

    if(parent != nullptr)
    {
        parent->children.push_back(this);
    }
    this->parent = parent;
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
