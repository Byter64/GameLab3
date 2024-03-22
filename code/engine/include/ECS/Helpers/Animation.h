#pragma once
#include "../../../../extern/tinygltf/tiny_gltf.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>
#include <map>

namespace Engine
{

    struct Animation
    {
        struct Channel
        {
            ~Channel();
            Channel() = default;
            Channel(Channel const& other);

            enum class Target
            {
                Invalid,
                Translation,
                Rotation,
                Scale,
                Weights
            };
            enum class Interpolation
            {
                Invalid,
                Step,
                Linear,
                CubicSpline
            };

            std::vector<unsigned int> hierarchy; //Each int represents a child index. Zero entries mean that this channel effects the root note, onto which this animation is applied to
            Target target; //The target property (either Translation, Rotation or Scale)
            Interpolation interpolation; //The type of interpolation (either Step, Linear or CubicSpline)

            union
            {
                std::map<float, glm::vec3> functionTo3D{}; //Domain and codomain of the animation function if its target is Translation or Scale
                std::map<float, glm::vec4> functionTo4D; //Domain and codomain of the animation function if its target is Rotation

            };

            static Target StringToTarget(std::string target);
            static Interpolation StringToInterpolation(std::string interpolation);
        };

        std::string name;
        std::vector<Channel> channels;
    };

} // Engine
