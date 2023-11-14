#include "MeshRenderer.h"

namespace Engine
{
    MeshRenderer::MeshRenderer(std::shared_ptr<tinygltf::Mesh> mesh, std::shared_ptr<tinygltf::Model> model)
    {
        this->mesh = std::move(mesh);
        this->model = std::move(model);
    }
} // Engine