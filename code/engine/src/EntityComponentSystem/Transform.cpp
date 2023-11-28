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
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
            glm::mat4 rotation = glm::toMat4(this->rotation);
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), this->translation);


            matrix = translation * rotation * scale;
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

    std::string Transform::ToString()
    {
        const glm::mat4x4 &matrix = GetMatrix();
        std::string result = std::to_string(matrix[0][0]) + ",\t ";
        result += std::to_string(matrix[1][0]) + ",\t ";
        result += std::to_string(matrix[2][0]) + ",\t ";
        result += std::to_string(matrix[3][0]) + ",\t\n";

        result += std::to_string(matrix[0][1]) + ",\t ";
        result += std::to_string(matrix[1][1]) + ",\t ";
        result += std::to_string(matrix[2][1]) + ",\t ";
        result += std::to_string(matrix[3][1]) + ",\t\n";

        result += std::to_string(matrix[0][2]) + ",\t ";
        result += std::to_string(matrix[1][2]) + ",\t ";
        result += std::to_string(matrix[2][2]) + ",\t ";
        result += std::to_string(matrix[3][2]) + ",\t\n";

        result += std::to_string(matrix[0][3]) + ",\t ";
        result += std::to_string(matrix[1][3]) + ",\t ";
        result += std::to_string(matrix[2][3]) + ",\t ";
        result += std::to_string(matrix[3][3]) + ",\t\n";

        return result;
    }
} // Engine