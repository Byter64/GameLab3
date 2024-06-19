#pragma once
#include <memory>
#include <stack>
#include <unordered_map>
#include <map>
#include <filesystem>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ECS/System.h"
#include "ECSSystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/MeshRenderer.h"
#include "glad/glad.h"
#include "../../../../extern/tinygltf/tiny_gltf.h"

namespace Engine
{
    class RenderSystem : public System
    {
        std::filesystem::path pathToDefaultVertexShader;
        std::filesystem::path pathToDefaultFragmentShader;

        GLuint defaultTexture = 0;
        Shader defaultShader;
        GLuint activeShader = 0;
        std::unordered_map<const tinygltf::BufferView*, GLuint> loadedBufferViews; //as of now, elements of this map will not be deleted, even if the buffer in the GPU does not exist anymore!!!!!!!
        std::unordered_map<const tinygltf::Texture*, GLuint> loadedTextures;
        std::unordered_map<const tinygltf::Primitive*, GLuint[11]> loadedVertexBuffers;
        std::unordered_map<const tinygltf::Primitive*, GLuint> loadedIndexBuffers;
        std::unordered_map<const tinygltf::Primitive*, GLuint> loadedVaos;
        std::set<std::shared_ptr<tinygltf::Model>> usedModels;
        std::list<Shader> loadedShaders;


        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;
        void LoadMesh(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model);
        void UnloadMesh(const tinygltf::Mesh &mesh, const tinygltf::Model & model);
        void LoadVertexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
        void LoadIndexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
        void LoadVAO(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
        void LoadTexture(const tinygltf::Texture &texture, const tinygltf::Model &model, int sourceFormat, int targetFormat);
        void Render(Entity entity);
        void CreateShaderProgram(Shader& shader);

        static unsigned int GetVertexAttributeIndex(const std::string& name);
        static std::unique_ptr<std::string> ReadShaderFromFile(const std::filesystem::path& filePath);
        static std::unique_ptr<std::string> ResolveIncludesForGLSL(const std::filesystem::path &filePath, std::unique_ptr<std::string> file);
    public:
        glm::mat4x4 projectionMatrix = glm::perspective(glm::radians(50.0f), 16.0f / 9, 1.0f, 1000.0f);
        glm::mat4x4 orthographicMatrix = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
        Transform camera;


        RenderSystem();

        void Render();
        MeshRenderer CreateMeshRenderer(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model);
        GLuint LoadShader(const std::filesystem::path& pathToShader, GLenum shaderType);
    };

} // Engine
