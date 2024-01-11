#pragma once
#include "Engine.h"
#include <string>
#include <memory>
#include <vector>

extern Engine::ECSSystem ecsSystem;
class GameObjectManager;

namespace Engine
{
    class Entity
    {
        friend class GameObjectManager;

        std::uint32_t entity;

        ~Entity();

    public:
        Entity(Entity &gameObject);

        explicit Entity(std::string name);

        explicit Entity(Engine::Entity entity);

        static Entity *CreateGameObjectFromGLTF(std::filesystem::path filePath);

        unsigned int GetChildCount();

        Entity *GetChild(unsigned int index);

        void SetParent(Entity *parent);

        template<typename T>
        T &AddComponent()
        {
            return ecsSystem.AddComponent<T>(entity);
        }

        template<typename T>
        T &GetComponent()
        {
            return ecsSystem.GetComponent<T>(entity);
        }

        template<typename T>
        bool HasComponent()
        {
            return ecsSystem.HasComponent<T>(entity);
        }

        void Destroy();

        void *operator new[](size_t) = delete;

        void operator delete[](void *) = delete;
    };
}