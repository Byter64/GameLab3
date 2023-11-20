#pragma once
#include "../../extern/tinygltf/tiny_gltf.h"
#include "glad/glad.h"

namespace Engine
{

    struct Material
    {
         tinygltf::Material* material;

        //Texture IDs
        GLuint baseColorID = 0;
        GLuint metallicRoughnessID = 0;
        GLuint normalID = 0;
        GLuint occlusionID = 0;
        GLuint emissiveID = 0;
    };

} // Engine
