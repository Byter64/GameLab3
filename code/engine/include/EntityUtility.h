#pragma once
#include "ECS/Entity.h"
#include "ECS/Components/Name.h"
#include <filesystem>
#include <vector>
#include "glm/glm.hpp"
#include "ECS/Components/EnemyBehaviour.h"

namespace Engine
{
    Entity FindChild(Entity root, Name name);
    Entity CopyEntity(Entity entity, bool copyChildren);
    std::vector<Entity> ImportGLTF(std::filesystem::path gltf, bool addParent = false);
    Entity SpawnBullet(Entity spawner, glm::vec3 position, glm::vec3 direction, float speed = 4.0f);
    Entity SpawnEnemy(std::pair<int, int> startPos, Engine::EnemyBehaviour::Behaviour behaviour);
    Entity SpawnWall(glm::vec3 position);
    void RemoveEntityWithChildren(Entity entity, bool removeParent = true);
} // Engine
