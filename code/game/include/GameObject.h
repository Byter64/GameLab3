#pragma once
#include "Engine.h"
#include <string>
#include <memory>
#include <vector>

extern Engine::ECSSystem ecsSystem;
class GameObjectManager;

class GameObject
{
    friend class GameObjectManager;

    bool receivesGameEvents = false;

    Engine::Entity entity;
    std::vector<GameObject*> children;
    GameObject* parent;

    ~GameObject();
public:
    GameObject(GameObject& gameObject);
    explicit GameObject(std::string name);
    explicit GameObject(Engine::Entity entity);
    static GameObject* CreateGameObjectFromGLTF(std::filesystem::path filePath);

    unsigned int GetChildCount();
    GameObject* GetChild(unsigned int index);
    void SetParent(GameObject* parent);

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

    void Destroy();

    void * operator new[](size_t) = delete;
    void operator delete [](void*) = delete;
};
