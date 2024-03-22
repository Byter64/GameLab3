#include "ECS/Helpers/Animation.h"
#include <algorithm>
#include <iostream>

Engine::Animation::Channel::Target Engine::Animation::Channel::StringToTarget(std::string target)
{
    std::transform(target.begin(), target.end(), target.begin(), tolower);

    if(target == "translation")
        return Target::Translation;
    else if (target == "rotation")
        return Target::Rotation;
    else if (target == "scale")
        return Target::Scale;
    else if (target == "weights")
        return Target::Weights;
    std::cout << "malformed gltf file. Animation target: " << target << " not valid" << std::endl;
    return Target::Invalid;
}

Engine::Animation::Channel::Interpolation Engine::Animation::Channel::StringToInterpolation(std::string interpolation)
{
    std::transform(interpolation.begin(), interpolation.end(), interpolation.begin(), tolower);

    if(interpolation == "step")
        return Interpolation::Step;
    else if (interpolation == "linear")
        return Interpolation::Linear;
    else if (interpolation == "cubicspline")
        return Interpolation::CubicSpline;

    std::cout << "malformed gltf file. Animation interpolation: " << interpolation << " not valid" << std::endl;
    return Interpolation::Invalid;
}

Engine::Animation::Channel::~Channel()
{
    hierarchy.~vector();

    if(target == Target::Rotation)
        functionTo4D.~map();
    else
        functionTo3D.~map();
}

Engine::Animation::Channel::Channel(Engine::Animation::Channel const &other)
{
    this->target = other.target;
    this->hierarchy = other.hierarchy;
    this->interpolation = other.interpolation;

    if(target == Target::Rotation)
        this->functionTo4D = other.functionTo4D;
    else
        this->functionTo3D = other.functionTo3D
}
