#pragma once
#include "Engine.h"
#include <string>
#include <memory>

extern Engine::ECSSystem ecsSystem;
class GameObject
{
    bool receivesGameEvents = false;

    Engine::Entity entity;
    std::vector<std::shared_ptr<GameObject>> children;
    GameObject* parent;

public:
    explicit GameObject(std::string name);
    explicit GameObject(Engine::Entity entity);

    unsigned int GetChildCount();
    std::shared_ptr<GameObject> GetChild(unsigned int index);
    void SetParent(std::shared_ptr<GameObject> parent);

    void SetUpdateMethod(void (*updateMethod)(float time));

    template<typename T>
    T& AddComponent()
    {
        return ecsSystem.AddComponent<T>(entity);
    }

    template<typename T>
    T& GetComponent()
    {
        return ecsSystem.GetComponent<T>(entity);
    }

    template<typename T>
    bool HasComponent()
    {
        return ecsSystem.HasComponent<T>(entity);
    }
    ~GameObject();
};
