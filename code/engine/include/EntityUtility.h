#pragma once
#include "ECS/Entity.h"
#include "ECS/Components/Name.h"
#include <filesystem>
#include <vector>
namespace Engine
{
    Entity FindChild(Entity root, Name name);
    std::vector<Entity> ImportGLTF(std::filesystem::path gltf);
} // Engine
