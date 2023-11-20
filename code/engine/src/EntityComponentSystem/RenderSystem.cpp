#include "RenderSystem.h"
#include <string>
#include <fstream>
#include <sstream>
#include "Entity.h"
#include "ECSSystem.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "../FileSystem.h"

extern Engine::ECSSystem ecsSystem;
namespace Engine
{
    void RenderSystem::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(Entity entity : entities)
        {
            Transform& transform = ecsSystem.GetComponent<Transform>(entity);
            if(transform.GetParent() != nullptr) continue;

            std::stack<glm::mat4x4> matrixStack;
            matrixStack.push(glm::mat4x4(1));
            Render(entity, matrixStack);
        }
    }

    void RenderSystem::Render(Entity entity, std::stack<glm::mat4x4>& matrixStack)
    {
        Transform& transform = ecsSystem.GetComponent<Transform>(entity);
        MeshRenderer& meshRenderer = ecsSystem.GetComponent<MeshRenderer>(entity);
        matrixStack.push(matrixStack.top() * transform.GetMatrix());

        GLuint newShader = meshRenderer.shaderID > 0 ? meshRenderer.shaderID : defaultShader;
        //Hier weiter machen
        //Shader und Projektionsmatrix nur aktualisieren, wenn newShader tatsächlich ein neuer ist
        //Testen: Spuckt Init 2 noch Fehler 1281 aus?
        //if()

        std::cout << "Init 1 " << glGetError() << "\n";
        GLuint projectionViewMatrixLocation = glGetUniformLocation(defaultShader, "projectionView");
        std::cout << "Init 2 " << glGetError() << "\n";
        glm::mat4x4 projectionViewMatrix = projectionMatrix * camera.GetMatrix();
        glUniformMatrix4fv(projectionViewMatrixLocation, 1, false, &projectionViewMatrix[0][0]);


        for(int i = 0; i < meshRenderer.mesh->primitives.size(); i++)
        {
            const tinygltf::Primitive &primitive = meshRenderer.mesh->primitives[i];
            const MeshRenderer::PrimitiveData &data = meshRenderer.primitiveData[i];
            const tinygltf::Material &material = meshRenderer.model->materials[primitive.material];

            std::cout << glGetError() << std::endl;
            GLuint modelMatrixLocation = glGetUniformLocation(defaultShader, "model");
            std::cout << glGetError() << std::endl;
            glUniformMatrix4fv(modelMatrixLocation, 1, false, &matrixStack.top()[0][0]);
            std::cout << glGetError() << std::endl << std::endl;

            //█████████████████████████████████████████████████████████████████████████████████████
            //██████████Setting Material data for Rendering████████████████████████████████████████
            //█████████████████████████████████████████████████████████████████████████████████████

            //BaseColor
            GLuint baseColorFactorLocation = glGetUniformLocation(defaultShader, "baseColorFactor");
            std::vector<float> baseColorFactor{(float)material.pbrMetallicRoughness.baseColorFactor[0],
                                               (float)material.pbrMetallicRoughness.baseColorFactor[1],
                                               (float)material.pbrMetallicRoughness.baseColorFactor[2]};
            glUniform4fv(baseColorFactorLocation, 1, &baseColorFactor[0]);
            if (data.material.baseColorID > 0)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, data.material.baseColorID);
            }

            //MetallicRoughness
            GLuint metallicFactorLocation = glGetUniformLocation(defaultShader, "metallicFactor");
            float metallicFactor = material.pbrMetallicRoughness.metallicFactor;
            glUniform1fv(metallicFactorLocation, 1, &metallicFactor);

            GLuint roughnessFactorLocation = glGetUniformLocation(defaultShader, "roughnessFactor");
            float roughnessFactor = material.pbrMetallicRoughness.roughnessFactor;
            glUniform1fv(roughnessFactorLocation, 1, &roughnessFactor);
            if(data.material.metallicRoughnessID > 0)
            {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, data.material.metallicRoughnessID);
            }

            //Normal
            GLuint normalScaleLocation = glGetUniformLocation(defaultShader, "normalScale");
            float normalScale = material.normalTexture.scale;
            glUniform1fv(normalScaleLocation, 1, &normalScale);
            if(data.material.normalID > 0)
            {
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, data.material.normalID);
            }

            //Occlusion
            GLuint occlusionStrengthLocation = glGetUniformLocation(defaultShader, "occlusionStrength");
            float occlusionStrength = material.occlusionTexture.strength;
            glUniform1fv(occlusionStrengthLocation, 1, &occlusionStrength);
            if(data.material.occlusionID > 0)
            {
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, data.material.occlusionID);
            }

            //Emissive
            GLuint emissiveFactorLocation = glGetUniformLocation(defaultShader, "emissiveFactor");
            std::vector<float> emissiveFactor{(float)material.emissiveFactor[0],
                                              (float)material.emissiveFactor[1],
                                              (float)material.emissiveFactor[2]};
            glUniform4fv(emissiveFactorLocation, 1, &emissiveFactor[0]);
            if(data.material.emissiveID > 0)
            {
                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D, data.material.emissiveID);
            }

            const tinygltf::Accessor& indices = meshRenderer.model->accessors[primitive.indices];
            glBindVertexArray(data.vaoID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.indexBufferID);
            glDrawElements(primitive.mode, indices.count, indices.componentType, (void*)indices.byteOffset);
        }



        auto children = transform.GetChildren();
        for(Transform* childTransform : children)
        {
            Entity childEntity = ecsSystem.GetEntity((*childTransform));
            Render(childEntity, matrixStack);
        }

        matrixStack.pop();
    }

    RenderSystem::RenderSystem()
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_CLAMP);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        pathToDefaultVertexShader = Engine::Files::ASSETS / "Shaders/Default/VS_Default.vert";
        pathToDefaultFragmentShader = Engine::Files::ASSETS / "Shaders/Default/FS_Default.frag";

        defaultShader = CreateShaderProgram(pathToDefaultVertexShader, pathToDefaultFragmentShader);
    }

    void RenderSystem::LoadMesh(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model)
    {
        for(const tinygltf::Primitive& primitive : mesh.primitives)
        {
            LoadVertexBuffer(primitive, *model);
            if(primitive.indices > -1) { LoadIndexBuffer(primitive, *model); }
            LoadVAO(primitive, *model);
        }

        //Textures need to be loaded

        usedModels.insert(std::move(model));
    }

    void RenderSystem::LoadVertexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model)
    {
        if(loadedVertexBuffers.find(&primitive) != loadedVertexBuffers.end()) return;

        int accessorIndex = primitive.attributes.begin()->second;
        const tinygltf::BufferView& bufferView = model.bufferViews[model.accessors[accessorIndex].bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        GLuint bufferID;
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);
        loadedVertexBuffers[&primitive] = bufferID;
    }

    void RenderSystem::LoadIndexBuffer(const tinygltf::Primitive &primitive, const tinygltf::Model &model)
    {
        if(loadedIndexBuffers.find(&primitive) != loadedIndexBuffers.end()) return;

        int accessorIndex = primitive.indices;
        const tinygltf::BufferView& bufferView = model.bufferViews[model.accessors[accessorIndex].bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        GLuint bufferID;
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);

        loadedIndexBuffers[&primitive] = bufferID;
    }

    void RenderSystem::LoadVAO(const tinygltf::Primitive &primitive, const tinygltf::Model &model)
    {
        if(loadedVaos.find(&primitive) != loadedVaos.end()) return;

        GLuint vaoID;
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, loadedVertexBuffers[&primitive]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedIndexBuffers[&primitive]);

        const tinygltf::BufferView& bufferView = model.bufferViews[model.accessors[primitive.attributes.begin()->second].bufferView];
        for(const auto& pair : primitive.attributes)
        {
            const std::string& name = pair.first;
            const tinygltf::Accessor& data = model.accessors[pair.second];

            unsigned int index = GetVertexAttributeIndex(name);

            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, data.type, data.componentType, data.normalized, bufferView.byteStride, (void*)data.byteOffset);
        }
        loadedVaos[&primitive] = vaoID;
    }

    unsigned int RenderSystem::GetVertexAttributeIndex(const std::string &name)
    {
        //Is it ugly?
        //--> Yes >:(
        //Does it work?
        //--> Yes -_-
        static std::unordered_map<std::string, unsigned int> nameToIndex
                {
                        {"POSITION", 0},
                        {"NORMAL", 1},
                        {"TANGENT", 2},
                        {"TEXCOORD_0", 3},
                        {"TEXCOORD_1", 4},
                        {"TEXCOORD_2", 5},
                        {"TEXCOORD_3", 6},
                        {"COLOR_0", 7},
                        {"COLOR_1", 8},
                        {"COLOR_2", 9},
                        {"COLOR_3", 10},
                };

        return nameToIndex[name];
    }

    /**
     * Only unloads vertex atrtibutes and indices. Does not unload Textures
     * @param mesh
     * @param model
     */
    void RenderSystem::UnloadMesh(tinygltf::Mesh &mesh)
    {
        for(const tinygltf::Primitive& primitive : mesh.primitives)
        {
            glDeleteBuffers(1, &loadedVertexBuffers[&primitive]);
            glDeleteBuffers(1, &loadedIndexBuffers[&primitive]);
            glDeleteVertexArrays(1, &loadedVaos[&primitive]);

            loadedVertexBuffers.erase(&primitive);
            loadedIndexBuffers.erase(&primitive);
            loadedVaos.erase(&primitive);
        }
    }

    MeshRenderer RenderSystem::CreateMeshRenderer(const tinygltf::Mesh &mesh, std::shared_ptr<tinygltf::Model> model)
    {
        MeshRenderer meshRenderer;
        LoadMesh(mesh, model);

        meshRenderer.model = model;
        meshRenderer.mesh = &(tinygltf::Mesh&)mesh;

        for(const tinygltf::Primitive& primitive : mesh.primitives)
        {
            MeshRenderer::PrimitiveData data;
            data.vaoID = loadedVaos[&primitive];
            data.indexBufferID = loadedIndexBuffers[&primitive];

            meshRenderer.primitiveData.push_back(data);
        }

        return meshRenderer;
    }

    std::unique_ptr<std::string> RenderSystem::ReadShaderFromFile(const std::filesystem::path &filePath)
    {
        /*Code copied from : https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
        Code has been modified*/

        std::ifstream fileStream(filePath); //taking file as inputstream
        std::unique_ptr<std::string> fileContent(new std::string());
        if (fileStream)
        {
            std::ostringstream stringStream;
            stringStream << fileStream.rdbuf(); // reading data
            std::string text = stringStream.str();
            fileContent->assign(text);
        }

        return fileContent;
    }

    GLuint RenderSystem::CreateShaderProgram(const std::filesystem::path &pathToVertexShader, const std::filesystem::path &pathToFragmentShader)
    {
        std::unique_ptr<std::string> vs_text_str = ReadShaderFromFile(pathToVertexShader);
        std::unique_ptr<std::string> fs_text_str = ReadShaderFromFile(pathToFragmentShader);
        const char* vs_text = vs_text_str->c_str();
        const char* fs_text = fs_text_str->c_str();

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vertexShader, 1, &vs_text, nullptr);
        glShaderSource(fragmentShader, 1, &fs_text, nullptr);
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        int hasSuccessfullyCompiled;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &hasSuccessfullyCompiled);
        if (!hasSuccessfullyCompiled)
        {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cout << "Error in Vertex shader: \n";
            std::cout << infoLog;
            exit(-1);
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &hasSuccessfullyCompiled);
        if (!hasSuccessfullyCompiled)
        {
            char infoLog[512];
            std::cout << "Error in Fragment shader: \n";
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << infoLog;
            exit(-1);
        }

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &hasSuccessfullyCompiled);
        if (!hasSuccessfullyCompiled)
        {
            char infoLog[512];
            std::cout << "Error during linking the shaders: \n";
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << infoLog;
            exit(-1);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

} // Engine