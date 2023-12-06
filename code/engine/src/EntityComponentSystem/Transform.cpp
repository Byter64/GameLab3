#include "Transform.h"
#include "glm/gtx/quaternion.hpp"
#include <iostream>
#include "../Engine.h"
extern Engine::ECSSystem ecsSystem;
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

        SetGlobalsNeedToBeUpdated();
    }

    void Transform::AddTranslation(const glm::vec3 &translation)
    {
        this->translation += translation;
        hasTransformChanged = true;

        SetGlobalsNeedToBeUpdated();
    }

    glm::vec3 Transform::GetScale()
    {
        return scale;
    }

    void Transform::SetScale(const glm::vec3 &scale)
    {
        this->scale = scale;
        hasTransformChanged = true;

        SetGlobalsNeedToBeUpdated();
    }

    void Transform::AddScale(const glm::vec3 &scale)
    {
        this->scale += scale;
        hasTransformChanged = true;

        SetGlobalsNeedToBeUpdated();
    }

    glm::quat Transform::GetRotation()
    {
        return rotation;
    }

    void Transform::SetRotation(const glm::quat &rotation)
    {
        this->rotation = rotation;
        hasTransformChanged = true;

        SetGlobalsNeedToBeUpdated();
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

    /**
     * Creates (and overwrites the old) parent-child relationship of this transform. Only use this method on components created by the ECS-System.
     * @param parent
     */
    void Transform::SetParent(Transform *parent)
    {
        //Remove old parent-child relationship
        if(this->parent != nullptr)
            this->parent->children.remove(this);

        //Create new parent-child relationship
        this->parent = parent;
        if(parent != nullptr)
            parent->children.push_back(this);

        SetGlobalsNeedToBeUpdated();
    }

    Transform *Transform::GetParent()
    {
        return parent;
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

    glm::vec3 Transform::GetGlobalTranslation()
    {
        if(globalsNeedToBeUpdated)
        {
            UpdateGlobals();
        }

        return globalTranslation;
    }


    glm::vec3 Transform::GetGlobalScale()
    {
        if(globalsNeedToBeUpdated)
        {
            UpdateGlobals();
        }

        return globalScale;
    }

    glm::quat Transform::GetGlobalRotation()
    {
        if(globalsNeedToBeUpdated)
        {
            UpdateGlobals();
        }

        return globalRotation;
    }

    const glm::mat4x4 &Transform::GetGlobalMatrix()
    {
        if(globalsNeedToBeUpdated)
        {
            UpdateGlobals();
        }

        return globalMatrix;
    }

    void Transform::UpdateGlobals()
    {
        if(parent == nullptr)
        {
            globalTranslation = GetTranslation();
            globalRotation = GetRotation();
            globalScale = GetScale();
            globalMatrix = GetMatrix();

            globalsNeedToBeUpdated = false;
            return;
        }

        const glm::mat4& parentMatrix = parent->GetGlobalMatrix();

        glm::mat4 globalTranslationMatrix = parentMatrix * glm::translate(glm::identity<glm::mat4>(), GetTranslation());
        globalTranslation = glm::vec3(globalTranslationMatrix[3]);

        glm::mat4 globalRotationMatrix = parentMatrix * glm::mat4_cast(GetRotation());
        globalRotation = glm::toQuat(globalRotationMatrix);

        glm::mat4 globalScaleMatrix = parentMatrix * glm::scale(glm::identity<glm::mat4>(), GetScale());
        globalScale = glm::vec3 (globalScaleMatrix[0][0],globalScaleMatrix[1][1],globalScaleMatrix[2][2]);


        glm::mat4 scale = glm::scale(glm::mat4(1.0f), globalScale);
        glm::mat4 rotation = glm::toMat4(this->rotation);
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), globalTranslation);
        globalMatrix = translation * rotation * scale;


        globalsNeedToBeUpdated = false;
    }

    void Transform::SetGlobalsNeedToBeUpdated()
    {
        globalsNeedToBeUpdated = true;
        for(Transform* child : children)
        {
            child->SetGlobalsNeedToBeUpdated();
        }
    }
} // Engine