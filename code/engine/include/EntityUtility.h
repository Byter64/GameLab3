#pragma once
#include "ECS/Entity.h"
#include "ECS/Components/Name.h"
#include <filesystem>
#include <vector>
#include "glm/glm.hpp"
#include "ECS/Components/EnemyBehaviour.h"
#include "ECSSystem.h"
#include "ECS/Components/Transform.h"

namespace Engine
{
    Entity FindChild(Entity root, Name name);
    Entity CopyEntity(Entity entity, bool copyChildren);
    std::vector<Entity> ImportGLTF(std::filesystem::path gltf, bool addParent = false);
    Entity SpawnEnemy(std::pair<int, int> startPos, EnemyBehaviour::Behaviour behaviour);
    Entity SpawnWall(glm::vec3 position);
    void RemoveEntityWithChildren(Entity entity, bool removeParent = true);

    template<typename T>
    std::vector<T*> GetComponentsInChildren(Entity entity)
    {
        std::vector<T*> components;

        if(ecsSystem->HasComponent<T>(entity))
            components.push_back(&ecsSystem->GetComponent<T>(entity));
        Transform& transform = ecsSystem->GetComponent<Transform>(entity);
        for(Transform* childTransform : transform.GetChildren())
        {
            Entity child = ecsSystem->GetEntity(*childTransform);
            std::vector<T*> childComponents = GetComponentsInChildren<T>(child);
            components.insert(components.end(), childComponents.begin(), childComponents.end());
        }

        return components;
    }
} // Engine
