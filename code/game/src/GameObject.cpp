#include "GameObject.h"
#include "../../engine/src/EntityComponentSystem/GameEvents.h"

extern Engine::ECSSystem ecsSystem;

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
            std::shared_ptr<GameObject> child = std::make_shared<GameObject>(ecsSystem.GetEntity(*childTransform));
            children.push_back(child);
            child->parent = this;
        }
    }
}

GameObject::~GameObject()
{

    ecsSystem.DestroyEntity(entity);
}

unsigned int GameObject::GetChildCount()
{
    return children.size();
}

std::shared_ptr<GameObject> GameObject::GetChild(unsigned int index)
{
    assert(index < children.size() && "Index was out of bounds");
    return children[index];
}

void GameObject::SetParent(std::shared_ptr<GameObject> parent)
{
    std::shared_ptr<GameObject> reference = *std::remove_if(this->parent->children.begin(), this->parent->children.end(), [this](std::shared_ptr<GameObject> gameObject){return gameObject.get() == this;});
    if(parent.get() != nullptr)
    {
        parent->children.push_back(reference);
    }
    this->parent = parent.get();
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
