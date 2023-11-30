#pragma once
#include "Entity.h"
#include "Name.h"
#include <filesystem>
namespace Engine
{
    Entity FindChild(Entity root, Name name);
    Entity ImportGLTF(std::filesystem::path gltf);
} // Engine
