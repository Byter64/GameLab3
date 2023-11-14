#pragma once
#include <memory>
#include "../../extern/tinygltf/tiny_gltf.h"

namespace Engine
{

    struct MeshRenderer
    {
        std::shared_ptr<tinygltf::Mesh> mesh;
        std::shared_ptr<tinygltf::Model> model;

        MeshRenderer(std::shared_ptr<tinygltf::Mesh> mesh, std::shared_ptr<tinygltf::Model> model);
    };

} // Engine
