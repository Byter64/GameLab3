#pragma once
#include "../../extern/tinygltf/tiny_gltf.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Engine
{

    struct Material
    {
        //Might be removable
        tinygltf::Material *material;

        //Factors, or whatever they are called
        glm::vec4 baseColorFactor{1};
        float metallicFactor = 1;
        float roughnessFactor = 1;
        float normalScale = 1;
        float occlusionStrength = 1;
        glm::vec3 emissiveFactor{1};

        //Texture IDs
        GLuint baseColorID = 0;
        GLuint metallicRoughnessID = 0;
        GLuint normalID = 0;
        GLuint occlusionID = 0;
        GLuint emissiveID = 0;

        //Miscellaneous
        bool isDoubleSided = false;
    };

} // Engine
