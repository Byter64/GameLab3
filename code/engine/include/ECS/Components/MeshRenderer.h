#pragma once
#include <memory>
#include "../../../../extern/tinygltf/tiny_gltf.h"
#include "Material.h"
#include "Shader.h"
namespace Engine
{

    struct MeshRenderer
    {
        struct PrimitiveData;

        tinygltf::Mesh* mesh;
        std::shared_ptr<tinygltf::Model> model;
        std::vector<PrimitiveData> primitiveData;

        MeshRenderer() = default;

        struct PrimitiveData
        {
            Shader shader;
            GLuint vaoID = 0;
            GLuint indexBufferID = 0;
            Material material;
            unsigned int vertexCount = 0;
        };
    };

} // Engine
