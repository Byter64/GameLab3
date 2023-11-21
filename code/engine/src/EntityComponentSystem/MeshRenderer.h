#pragma once
#include <memory>
#include "../../extern/tinygltf/tiny_gltf.h"
#include "Material.h"
namespace Engine
{

    struct MeshRenderer
    {
        struct PrimitiveData;

        tinygltf::Mesh* mesh;
        std::shared_ptr<tinygltf::Model> model;
        std::vector<PrimitiveData> primitiveData;
        GLuint shaderID = 0;

        MeshRenderer() = default;

        struct PrimitiveData
        {
            GLuint vaoID = 0;
            GLuint indexBufferID = 0;
            Material material;
            unsigned int vertexCount = 0;
        };
    };

} // Engine
