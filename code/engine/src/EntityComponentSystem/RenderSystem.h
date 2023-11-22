#pragma once
#include <memory>
#include <stack>
#include <unordered_map>
#include <filesystem>
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
        std::filesystem::path pathToDefaultVertexShader;
        std::filesystem::path pathToDefaultFragmentShader;

        GLuint defaultShader = 0;
        GLuint activeShader = 0;
        std::unordered_map<const tinygltf::BufferView*, GLuint> loadedBufferViews; //as of now, elements of this map will not be deleted, even if the buffer in the GPU does not exist anymore!!!!!!!
        std::unordered_map<const tinygltf::Primitive*, GLuint[11]> loadedVertexBuffers;
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
        static std::unique_ptr<std::string> ReadShaderFromFile(const std::filesystem::path& filePath);
    public:
        glm::mat4x4 projectionMatrix = glm::perspective(glm::radians(90.0f), 16.0f / 9, 1.0f, 1000.0f);
        Transform camera;


        RenderSystem();

        void Render();
        MeshRenderer CreateMeshRenderer(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model);
        void LoadMesh(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model);
        void UnloadMesh(const tinygltf::Mesh &mesh, const tinygltf::Model & model);
        GLuint CreateShaderProgram(const std::filesystem::path& pathToVertexShader, const std::filesystem::path& pathToFragmentShader);
    };

} // Engine
