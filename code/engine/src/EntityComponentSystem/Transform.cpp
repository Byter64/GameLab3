#include "Transform.h"
#include "glm/gtx/quaternion.hpp"
#include <iostream>
namespace Engine
{
    Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::quat rotation) : translation(position), scale(scale), rotation(rotation)
    {
        parent = nullptr;
    }

    glm::vec3 Transform::GetTranslation()
    {
        return translation;
    }

    void Transform::SetTranslation(const glm::vec3 &translation)
    {
        this->translation = translation;
        hasTransformChanged = true;
    }

    glm::vec3 Transform::GetScale()
    {
        return scale;
    }

    void Transform::SetScale(const glm::vec3 &scale)
    {
        this->scale = scale;
        hasTransformChanged = true;
    }

    glm::quat Transform::GetRotation()
    {
        return rotation;
    }

    void Transform::SetRotation(const glm::quat &rotation)
    {
        this->rotation = rotation;
        hasTransformChanged = true;
    }

    const glm::mat4x4 &Transform::GetMatrix()
    {
        if(hasTransformChanged)
        {
            matrix = glm::scale(glm::mat4x4(1), scale);
            matrix = glm::toMat4(rotation) * matrix;
            matrix = glm::translate(matrix, translation);

            hasTransformChanged = false;
        }

        return matrix;
    }

    void Transform::SetParent(Transform *parent)
    {
        this->parent = parent;
    }

    Transform *Transform::GetParent()
    {
        return parent;
    }

    void Transform::AddChild(Transform *child)
    {
        bool isAlreadyAChild = std::find(children.begin(), children.end(),child) != children.end();
        if(isAlreadyAChild) return;

        children.push_back(child);
    }

    void Transform::RemoveChild(Transform *child)
    {
        bool isNotAChild = std::find(children.begin(), children.end(),child) == children.end();
        if(isNotAChild) return;

        children.remove(child);
    }

    const std::list<Transform*> &Transform::GetChildren()
    {
        return children;
    }
} // Engine