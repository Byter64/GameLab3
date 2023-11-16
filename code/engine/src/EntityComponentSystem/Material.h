#pragma once
#include "../../extern/tinygltf/tiny_gltf.h"
#include "glad/glad.h"

namespace Engine
{

    struct Material
    {
        tinygltf::Material material;

        //Texture IDs
        GLuint baseColorID;
        GLuint metallicRoughnessID;
        GLuint normalID;
        GLuint occlusionID;
        GLuint emissiveID;
    };

} // Engine
