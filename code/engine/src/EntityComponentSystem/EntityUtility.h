#pragma once
#include "Entity.h"
#include "Name.h"
#include <filesystem>
#include <vector>
namespace Engine
{
    Entity FindChild(Entity root, Name name);
    std::vector<Entity> ImportGLTF(std::filesystem::path gltf);
} // Engine
