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
            std::map<float, std::vector<float>> function; //Domain and codomain of the animation function, codomain is 3D for Translation and Scale, and 4D for Rotation

            static Target StringToTarget(std::string target);
            static Interpolation StringToInterpolation(std::string interpolation);
        };

        std::string name;
        std::vector<Channel> channels;
    };

} // Engine
