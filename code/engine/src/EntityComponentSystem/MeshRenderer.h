#pragma once
#include <memory>
#include "../../extern/tinygltf/tiny_gltf.h"
#include "GLFW/glfw3.h"
#include "Material.h"
namespace Engine
{

    struct MeshRenderer
    {
        struct PrimitiveData;

        std::shared_ptr<tinygltf::Mesh> mesh;
        std::shared_ptr<tinygltf::Model> model;
        std::vector<PrimitiveData> primitiveData;
        GLuint shaderID;

        MeshRenderer(std::shared_ptr<tinygltf::Mesh> mesh, std::shared_ptr<tinygltf::Model> model);

        struct PrimitiveData
        {
            GLuint vaoID;
            GLuint indexBufferID;
            Material material;
        };
    };

} // Engine
