#pragma once
#include "ECS/Entity.h"
#include "ECS/Components/Name.h"
#include <filesystem>
#include <vector>
#include "glm/glm.hpp"

namespace Engine
{
    Entity FindChild(Entity root, Name name);
    Entity CopyEntity(Entity entity);
    std::vector<Entity> ImportGLTF(std::filesystem::path gltf, bool addParent = false);
    Entity SpawnBullet(Entity spawner, glm::vec3 position, glm::vec3 direction, float speed = 4.0f);
} // Engine
