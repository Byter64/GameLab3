#include "RenderSystem.h"
#include <string>
#include <fstream>
#include <sstream>
#include <array>
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
        matrixStack.push(matrixStack.top() * transform.GetMatrix());

        if(ecsSystem.GetSignature(entity)[ecsSystem.GetComponentType<MeshRenderer>()] == 0) goto Recursion;
        MeshRenderer& meshRenderer = ecsSystem.GetComponent<MeshRenderer>(entity);

        GLuint newShader = meshRenderer.shaderID > 0 ? meshRenderer.shaderID : defaultShader;
        if(newShader != activeShader)
        {
            glUseProgram(newShader);
            activeShader = newShader;
        }

        GLuint projectionViewMatrixLocation = glGetUniformLocation(defaultShader, "projectionView");
        glm::mat4x4 projectionViewMatrix = projectionMatrix * camera.GetMatrix();
        glUniformMatrix4fv(projectionViewMatrixLocation, 1, false, &projectionViewMatrix[0][0]);

        for(int i = 0; i < meshRenderer.mesh->primitives.size(); i++)
        {
            const tinygltf::Primitive &primitive = meshRenderer.mesh->primitives[i];
            const MeshRenderer::PrimitiveData &data = meshRenderer.primitiveData[i];
            const Material &material = data.material;

            GLuint modelMatrixLocation = glGetUniformLocation(activeShader, "model");
            glUniformMatrix4fv(modelMatrixLocation, 1, false, &matrixStack.top()[0][0]);

            //█████████████████████████████████████████████████████████████████████████████████████
            //██████████Setting Material data for Rendering████████████████████████████████████████
            //█████████████████████████████████████████████████████████████████████████████████████

            //BaseColor
            GLuint baseColorFactorLocation = glGetUniformLocation(activeShader, "baseColorFactor");
			
			//Please check, if material.baseColorFactor actually needs a "&" before it.
            glUniform4fv(baseColorFactorLocation, 1, &material.baseColorFactor.x);
            if (material.baseColorID > 0)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, material.baseColorID);
            }

            //MetallicRoughness
            GLuint metallicFactorLocation = glGetUniformLocation(activeShader, "metallicFactor");
            glUniform1fv(metallicFactorLocation, 1, &material.metallicFactor);

            GLuint roughnessFactorLocation = glGetUniformLocation(activeShader, "roughnessFactor");
            glUniform1fv(roughnessFactorLocation, 1, &material.roughnessFactor);
            if(material.metallicRoughnessID > 0)
            {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, material.metallicRoughnessID);
            }

            //Normal
            GLuint normalScaleLocation = glGetUniformLocation(activeShader, "normalScale");
            glUniform1fv(normalScaleLocation, 1, &material.normalScale);
            if(material.normalID > 0)
            {
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, material.normalID);
            }

            //Occlusion
            GLuint occlusionStrengthLocation = glGetUniformLocation(activeShader, "occlusionStrength");
            glUniform1fv(occlusionStrengthLocation, 1, &material.occlusionStrength);
            if(material.occlusionID > 0)
            {
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, material.occlusionID);
            }

            //Emissive
            GLuint emissiveFactorLocation = glGetUniformLocation(activeShader, "emissiveFactor");
            glUniform4fv(emissiveFactorLocation, 1, &material.emissiveFactor.x);
            if(material.emissiveID > 0)
            {
                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D, material.emissiveID);
            }

            const tinygltf::Accessor& indices = meshRenderer.model->accessors[primitive.indices];
            glBindVertexArray(data.vaoID);
            if(data.indexBufferID > 0)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.indexBufferID);
                glDrawElements(primitive.mode, indices.count, indices.componentType, (void *) indices.byteOffset);
            }
            else
            {
                //Den Fall abfangen, dass ein Primitiv nicht indiziert ist
                assert(false && "Tadaa, hier ist ein nicht indiziertes Modell. Jetzt musst du leider doch nichtindizierte Modelle unterstützen ;-)");

                glDrawArrays(primitive.mode, 0, data.vertexCount);
            }
        }

Recursion:

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

        camera.SetTranslation(glm::vec3(0));
        camera.SetScale(glm::vec3(1));
        camera.SetRotation(glm::identity<glm::quat>());

        pathToDefaultVertexShader = Engine::Files::ASSETS / "Shaders/Default/VS_Default.vert";
        pathToDefaultFragmentShader = Engine::Files::ASSETS / "Shaders/Default/FS_Default.frag";

        defaultShader = CreateShaderProgram(pathToDefaultVertexShader, pathToDefaultFragmentShader);

        float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glGenTextures(1, &defaultTexture);
        glBindTexture(GL_TEXTURE_2D, defaultTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, white);
    }

    void RenderSystem::LoadMesh(const tinygltf::Mesh& mesh, std::shared_ptr<tinygltf::Model> model)
    {
        for(const tinygltf::Primitive& primitive : mesh.primitives)
        {
            LoadVertexBuffer(primitive, *model);
            if(primitive.indices > -1) { LoadIndexBuffer(primitive, *model); }
            LoadVAO(primitive, *model);

            if(primitive.material > -1)
            {
                const tinygltf::Material& material = model->materials[primitive.material];
                //By default the source format, which is delivered by tinyGLTF, is always RGBA
                if(material.pbrMetallicRoughness.baseColorTexture.index > 0)
                {
                    const tinygltf::Texture &baseColor = model->textures[material.pbrMetallicRoughness.baseColorTexture.index];
                    LoadTexture(baseColor, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.pbrMetallicRoughness.metallicRoughnessTexture.index > 0)
                {
                    const tinygltf::Texture &metallicRoughness = model->textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
                    LoadTexture(metallicRoughness, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.normalTexture.index > 0)
                {
                    const tinygltf::Texture &normal = model->textures[material.normalTexture.index];
                    LoadTexture(normal, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.occlusionTexture.index > 0)
                {
                    const tinygltf::Texture &occlusion = model->textures[material.occlusionTexture.index];
                    LoadTexture(occlusion, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.emissiveTexture.index > 0)
                {
                    const tinygltf::Texture &emissive = model->textures[material.emissiveTexture.index];
                    LoadTexture(emissive, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
            }
        }


        usedModels.insert(std::move(model));
    }

    void RenderSystem::LoadVertexBuffer(const tinygltf::Primitive& primitive, const tinygltf::Model& model)
    {
        if (loadedVertexBuffers.find(&primitive) != loadedVertexBuffers.end()) return;

        for(auto& pair : primitive.attributes)
        {
            int accessorIndex = pair.second;
            unsigned int attributeIndex = GetVertexAttributeIndex(pair.first);
            const tinygltf::BufferView &bufferView = model.bufferViews[model.accessors[accessorIndex].bufferView];
            const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

            if(loadedBufferViews.find(&bufferView) != loadedBufferViews.end())
            {
                loadedVertexBuffers[&primitive][attributeIndex] = loadedBufferViews[&bufferView];
            }
            else
            {
                GLuint bufferID;
                glGenBuffers(1, &bufferID);
                glBindBuffer(GL_ARRAY_BUFFER, bufferID);
                glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);
                loadedVertexBuffers[&primitive][attributeIndex] = bufferID;
                loadedBufferViews[&bufferView] = bufferID;
            }
        }
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
        if (loadedVaos.find(&primitive) != loadedVaos.end()) return;

        GLuint vaoID;
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);

        for(auto& pair: primitive.attributes)
        {
            const tinygltf::Accessor &data = model.accessors[pair.second];
            const tinygltf::BufferView &bufferView = model.bufferViews[data.bufferView];
            unsigned int index = GetVertexAttributeIndex(pair.first);

            glBindBuffer(GL_ARRAY_BUFFER, loadedBufferViews[&bufferView]);
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, data.type, data.componentType, data.normalized, bufferView.byteStride, (void *) data.byteOffset);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedIndexBuffers[&primitive]);
            loadedVaos[&primitive] = vaoID;
        }
    }

    void RenderSystem::LoadTexture(const tinygltf::Texture &texture, const tinygltf::Model &model, int sourceFormat, int targetFormat)
    {
        if(loadedTextures.find(&texture) != loadedTextures.end()) return;

        const tinygltf::Image& image = model.images[texture.source];
        const tinygltf::Sampler& sampler = model.samplers[texture.sampler];
        GLuint textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, sourceFormat, image.width, image.height, 0, targetFormat, image.pixel_type, &image.image[0]);

        loadedTextures[&texture] = textureID;
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
    void RenderSystem::UnloadMesh(const tinygltf::Mesh &mesh, const tinygltf::Model &model)
    {
        for (const tinygltf::Primitive &primitive: mesh.primitives)
        {
            for (auto &pair: primitive.attributes)
            {
                const tinygltf::BufferView &bufferView = model.bufferViews[model.accessors[pair.second].bufferView];
                glDeleteBuffers(1, &loadedBufferViews[&bufferView]);
            }
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

            if (primitive.material > -1)
            {
                data.material.material = &model->materials[primitive.material];
                data.vertexCount = model->accessors[((tinygltf::Primitive &) primitive).attributes["POSITION"]].count;

                Material &material = data.material;
                material.baseColorFactor.x = (float) model->materials[primitive.material].pbrMetallicRoughness.baseColorFactor[0];
                material.baseColorFactor.y = (float) model->materials[primitive.material].pbrMetallicRoughness.baseColorFactor[1];
                material.baseColorFactor.z = (float) model->materials[primitive.material].pbrMetallicRoughness.baseColorFactor[2];
                material.baseColorFactor.w = (float) model->materials[primitive.material].pbrMetallicRoughness.baseColorFactor[3];

                material.metallicFactor = (float) model->materials[primitive.material].pbrMetallicRoughness.metallicFactor;
                material.roughnessFactor = (float) model->materials[primitive.material].pbrMetallicRoughness.roughnessFactor;
                material.normalScale = (float) model->materials[primitive.material].normalTexture.scale;
                material.occlusionStrength = (float) model->materials[primitive.material].occlusionTexture.strength;

                material.emissiveFactor.x = (float) model->materials[primitive.material].emissiveFactor[0];
                material.emissiveFactor.y = (float) model->materials[primitive.material].emissiveFactor[1];
                material.emissiveFactor.z = (float) model->materials[primitive.material].emissiveFactor[2];

                int baseColorIndex = model->materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index;
                material.baseColorID = baseColorIndex > -1 ? loadedTextures[&model->textures[baseColorIndex]]: defaultTexture;

                int metallicRoughnessIndex = model->materials[primitive.material].pbrMetallicRoughness.metallicRoughnessTexture.index;
                material.metallicRoughnessID = metallicRoughnessIndex > -1 ? loadedTextures[&model->textures[metallicRoughnessIndex]] : defaultTexture;

                int normalIndex = model->materials[primitive.material].normalTexture.index;
                material.normalID = normalIndex > -1 ? loadedTextures[&model->textures[normalIndex]] : defaultTexture;

                int occlusionIndex = model->materials[primitive.material].occlusionTexture.index;
                material.occlusionID = occlusionIndex > -1 ? loadedTextures[&model->textures[occlusionIndex]] : defaultTexture;

                int emissiveIndex = model->materials[primitive.material].emissiveTexture.index;
                material.emissiveID =emissiveIndex > -1 ? loadedTextures[&model->textures[emissiveIndex]] : defaultTexture;
            }

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

        return shaderProgram;
    }

} // Engine