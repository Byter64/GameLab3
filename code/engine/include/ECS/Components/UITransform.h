#pragma once
#include "ECS/Components/Transform.h"

namespace Engine
{

    struct UITransform
    {
    private:
        bool hasTransformChanged = true;
        glm::mat4 matrix = glm::mat4(1);
        glm::vec3 translation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
        glm::quat rotation = glm::identity<glm::quat>();

        bool isGlobalTranslationOutdated = true;
        bool isGlobalScaleOutdated = true;
        bool isGlobalRotationOutdated = true;
        bool isGlobalMatrixOutdated = true;
        glm::mat4 globalMatrix = glm::mat4(1);
        glm::vec3 globalTranslation = glm::vec3(0, 0, 0);
        glm::vec3 globalScale = glm::vec3(1, 1, 1);
        glm::quat globalRotation = glm::identity<glm::quat>();

        UITransform* parent = nullptr;
        std::list<UITransform*> children;

        void SetIsGlobalTranslationOutdated();
        void SetIsGlobalScaleOutdated();
        void SetIsGlobalRotationOutdated();

    public:
        UITransform() = default;
        UITransform(glm::vec3 position, glm::vec3 scale, glm::quat rotation);
        UITransform(UITransform& other) = default;

        UITransform& operator= (UITransform& other) = default;
        UITransform& operator= (UITransform&& other);

        glm::vec3 GetTranslation();
        glm::vec3 GetGlobalTranslation();
        void SetTranslation(const glm::vec3& translation);
        void AddTranslation(const glm::vec3& translation);

        glm::vec3 GetScale();
        glm::vec3 GetGlobalScale();
        void SetScale(const glm::vec3& scale);
        void AddScale(const glm::vec3& scale);

        glm::quat GetRotation();
        glm::quat GetGlobalRotation();
        void SetRotation(const glm::quat& rotation);

        const glm::mat4x4& GetMatrix();
        const glm::mat4x4& GetGlobalMatrix();


        void SetParent(UITransform* parent);
        UITransform* GetParent();

        std::list<UITransform*>& GetChildren();
        std::string ToString();
        std::string MatrixToString();
        std::string GlobalTranslationToString();
    };

} // Engine
