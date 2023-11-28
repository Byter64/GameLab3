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
        bool hasTransformChanged = false;

        glm::mat4x4 matrix = glm::mat4x4(1);
        glm::vec3 translation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
        glm::quat rotation = glm::quat(0, 0, 0, 0);

        Transform* parent;
        std::list<Transform*> children;

    public:
        Transform() = default;
        Transform(glm::vec3 position, glm::vec3 scale, glm::quat rotation);

        glm::vec3 GetTranslation();
        void SetTranslation(const glm::vec3& translation);

        glm::vec3 GetScale();
        void SetScale(const glm::vec3& scale);

        glm::quat GetRotation();
        void SetRotation(const glm::quat& rotation);

        const glm::mat4x4& GetMatrix();


        void SetParent(Transform* parent);
        Transform* GetParent();

        void AddChild(Transform* child);
        void RemoveChild(Transform* child);
        const std::list<Transform*>& GetChildren();
        std::string ToString();
    };

} // Engine
