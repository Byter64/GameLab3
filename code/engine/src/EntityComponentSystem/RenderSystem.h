#pragma once
#include <memory>
#include <stack>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "System.h"
#include "ECSSystem.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "glad/glad.h"
#include "../../extern/tinygltf/tiny_gltf.h"

namespace Engine
{

    class RenderSystem : public System
    {
        GLuint defaultShader;
        std::unordered_map<const tinygltf::Primitive*, GLuint> loadedVertexBuffers;
        std::unordered_map<const tinygltf::Primitive*, GLuint> loadedIndexBuffers;
        std::unordered_map<const tinygltf::Primitive*, GLuint> loadedVaos;
        std::set<std::shared_ptr<tinygltf::Model>> usedModels;

        /**
         * Loads a buffer onto the GPU and adds an entry to loadedVertexBuffers. If the buffer is already loaded, this method does nothing.
         * @param buffer
         */
        void LoadVertexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
        void LoadIndexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
        void LoadVAO(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
        void Render(Entity entity, std::stack<glm::mat4x4>& matrixStack);

        static unsigned int GetVertexAttributeIndex(const std::string& name);
    public:
        glm::mat4x4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 1000.0f);
        Transform camera;


        RenderSystem();

        void Render();
        void LoadMesh(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model);
        MeshRenderer CreateMeshRenderer(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model);
        void UnloadMesh(tinygltf::Mesh& mesh);
    };

} // Engine
