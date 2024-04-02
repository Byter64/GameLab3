#pragma once
#include <memory>
#include "../../../../extern/tinygltf/tiny_gltf.h"
#include "ECS/Helpers/Material.h"
#include "ECS/Helpers/Shader.h"
#include "ECS/Helpers/RenderLayer.h"
namespace Engine
{

    struct MeshRenderer
    {
        struct PrimitiveData;
        bool isActive = true;
        RenderLayer renderLayer = WORLD;
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
