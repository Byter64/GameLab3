#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
namespace Engine
{

    struct Material
    {
        glm::vec3 baseColorFactor{1};
        float metallicFactor = 1;
        float roughnessFactor = 1;
        float normalFactor = 1; //called "scale" in gltf
        float occlusionFactor = 1; //called "strength" in gltf
        glm::vec3 emissiveFactor{1};

        GLuint baseColorTextureID = 0;
        GLuint metallicRoughnessTextureID = 0;
        GLuint normalTextureID = 0;
        GLuint occlusionTextureID = 0;
        GLuint emissiveTextureID = 0;
    };

} // Engine
