#pragma once
#include "glm/glm.hpp"
#include "glm/ext/quaternion_float.hpp"
#include <list>
#include <string>

namespace Engine
{

    struct Transform
    {
    private:
        bool hasTransformChanged = true;
        glm::mat4 matrix = glm::mat4(1);
        glm::vec3 translation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
        glm::quat rotation = glm::quat(0, 0, 0, 0);

        bool globalsNeedToBeUpdated = true;
        glm::mat4 globalMatrix = glm::mat4(1);
        glm::vec3 globalTranslation = glm::vec3(0, 0, 0);
        glm::vec3 globalScale = glm::vec3(1, 1, 1);
        glm::quat globalRotation = glm::quat(0, 0, 0, 0);

        Transform* parent = nullptr;
        std::list<Transform*> children;

        void UpdateGlobals();
        void SetGlobalsNeedToBeUpdated();

    public:
        Transform() = default;
        Transform(glm::vec3 position, glm::vec3 scale, glm::quat rotation);

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


        void SetParent(Transform* parent);
        Transform* GetParent();

        const std::list<Transform*>& GetChildren();
        std::string ToString();
    };

} // Engine
