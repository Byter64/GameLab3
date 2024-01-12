#include "ECS/Components/Transform.h"
#include "glm/gtx/quaternion.hpp"
#include <iostream>
#include "Engine.h"
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
        
        SetIsGlobalTranslationOutdated();
    }

    void Transform::AddTranslation(const glm::vec3 &translation)
    {
        this->translation += translation;
        hasTransformChanged = true;

        SetIsGlobalTranslationOutdated();
    }

    glm::vec3 Transform::GetScale()
    {
        return scale;
    }

    void Transform::SetScale(const glm::vec3 &scale)
    {
        this->scale = scale;
        hasTransformChanged = true;

        SetIsGlobalTranslationOutdated();
        SetIsGlobalScaleOutdated();
    }

    void Transform::AddScale(const glm::vec3 &scale)
    {
        this->scale += scale;
        hasTransformChanged = true;

        SetIsGlobalTranslationOutdated();
        SetIsGlobalScaleOutdated();
    }

    glm::quat Transform::GetRotation()
    {
        return rotation;
    }

    void Transform::SetRotation(const glm::quat &rotation)
    {
        this->rotation = rotation;
        hasTransformChanged = true;

        SetIsGlobalTranslationOutdated();
        SetIsGlobalRotationOutdated();
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

        SetIsGlobalTranslationOutdated();
    }

    Transform *Transform::GetParent()
    {
        return parent;
    }

    std::list<Transform*> &Transform::GetChildren()
    {
        return children;
    }

    std::string Transform::MatrixToString()
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
        if(isGlobalTranslationOutdated)
        {
            if(parent == nullptr)
            {
                globalTranslation = translation;
            }
            else
            {
                glm::mat4 parent = this->parent->GetGlobalMatrix();
                globalTranslation = parent * glm::vec4(translation, 1);
            }
            isGlobalTranslationOutdated = false;
        }

        return globalTranslation;
    }


    glm::vec3 Transform::GetGlobalScale()
    {
        if(isGlobalScaleOutdated)
        {
            if(parent == nullptr)
            {
                globalScale = scale;
            }
            else
            {
                glm::vec3 parent = this->parent->GetGlobalScale();
                globalScale = glm::vec3(scale.x * parent.x, scale.y * parent.y, scale.z * parent.z);
            }
            isGlobalScaleOutdated = false;
        }

        return globalScale;
    }

    glm::quat Transform::GetGlobalRotation()
    {
        if(isGlobalRotationOutdated)
        {
            if(parent == nullptr)
            {
                globalRotation = rotation;
            }
            else
            {
                glm::quat parent = this->parent->GetGlobalRotation();
                globalRotation = parent * rotation;

            }
            isGlobalRotationOutdated = false;
        }

        return globalRotation;
    }

    const glm::mat4x4 &Transform::GetGlobalMatrix()
    {
        if(isGlobalMatrixOutdated)
        {
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), GetGlobalScale());
            glm::mat4 rotation = glm::toMat4(GetGlobalRotation());
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), GetGlobalTranslation());
            globalMatrix = translation * rotation * scale;

            isGlobalMatrixOutdated = false;
        }

        return globalMatrix;
    }

    void Transform::SetIsGlobalTranslationOutdated()
    {
        isGlobalTranslationOutdated = true;
        isGlobalMatrixOutdated = true;
        for(Transform* child : children)
        {
            child->SetIsGlobalTranslationOutdated();
        }
    }

    void Transform::SetIsGlobalScaleOutdated()
    {
        isGlobalScaleOutdated = true;
        isGlobalMatrixOutdated = true;
        for(Transform* child : children)
        {
            child->SetIsGlobalScaleOutdated();
        }
    }

    void Transform::SetIsGlobalRotationOutdated()
    {
        isGlobalRotationOutdated = true;
        isGlobalMatrixOutdated = true;
        for(Transform* child : children)
        {
            child->SetIsGlobalRotationOutdated();
        }
    }

    std::string Transform::ToString()
    {
        std::string message{"LOCAL\n"};
        message += "\tPosition: (";
        message += std::to_string(translation.x);
        message += ", ";
        message += std::to_string(translation.y);
        message += ", ";
        message += std::to_string(translation.z);
        message += ")\n";

        message += "\tScale: (";
        message += std::to_string(scale.x);
        message += ", ";
        message += std::to_string(scale.y);
        message += ", ";
        message += std::to_string(scale.z);
        message += ")\n";

        message += "\tRotation: (";
        message += std::to_string(rotation.x);
        message += ", ";
        message += std::to_string(rotation.y);
        message += ", ";
        message += std::to_string(rotation.z);
        message += ", ";
        message += std::to_string(rotation.w);
        message += ")\n";

        glm::vec3 globalTranslation = GetGlobalTranslation();
        message += "\nGLOBAL\n";
        message += "\tPosition: (";
        message += std::to_string(globalTranslation.x);
        message += ", ";
        message += std::to_string(globalTranslation.y);
        message += ", ";
        message += std::to_string(globalTranslation.z);
        message += ")\n";

        glm::vec3 globalScale = GetGlobalScale();
        message += "\tScale: (";
        message += std::to_string(globalScale.x);
        message += ", ";
        message += std::to_string(globalScale.y);
        message += ", ";
        message += std::to_string(globalScale.z);
        message += ")\n";

        glm::quat globalRotation = GetGlobalRotation();
        message += "\tRotation: (";
        message += std::to_string(globalRotation.x);
        message += ", ";
        message += std::to_string(globalRotation.y);
        message += ", ";
        message += std::to_string(globalRotation.z);
        message += ", ";
        message += std::to_string(globalRotation.w);
        message += ")\n";


        return message;
    }
} // Engine