#include "Engine.h"
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <stdexcept>

namespace Engine
{
    void RenderSystem::Render()
    {
        glClearColor(0.172f, 0.243f, 0.313f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(Entity entity : entities)
        {
            MeshRenderer& meshRenderer = ecsSystem->GetComponent<MeshRenderer>(entity);
            if(meshRenderer.renderLayer == WORLD)
                Render(entity);
        }

        glClear(GL_DEPTH_BUFFER_BIT);
        for(Entity entity : entities)
        {
            MeshRenderer& meshRenderer = ecsSystem->GetComponent<MeshRenderer>(entity);
            if(meshRenderer.renderLayer == UI)
                Render(entity);
        }
    }

    void RenderSystem::Render(Entity entity)
    {
        Transform& transform = ecsSystem->GetComponent<Transform>(entity);
        MeshRenderer& meshRenderer = ecsSystem->GetComponent<MeshRenderer>(entity);

        for(int i = 0; i < meshRenderer.mesh->primitives.size(); i++)
        {
            const tinygltf::Primitive &primitive = meshRenderer.mesh->primitives[i];
            MeshRenderer::PrimitiveData &data = meshRenderer.primitiveData[i];
            const Material &material = data.material;

            //I doubt that updating the shader should be handled here. But I don't know any better place right now
            //hello World!!
            if(data.shader.id == 0)
            {
                CreateShaderProgram(data.shader);
            }
            else if(data.shader.hasChanged)
            {
                const auto& bla = std::find(loadedShaders.begin(), loadedShaders.end(), data.shader);
                if(bla != loadedShaders.end())
                {
                    data.shader.id = bla->id;
                }
                else
                {
                    CreateShaderProgram(data.shader);
                }
            }

            GLuint newShader = data.shader.id;
            if(newShader != activeShader)
            {
                glUseProgram(newShader);
                activeShader = newShader;
            }

            GLint projectionViewMatrixLocation = glGetUniformLocation(newShader, "projectionView");
            glm::mat4x4 projectionViewMatrix = projectionMatrix * camera.GetMatrix();
            glUniformMatrix4fv(projectionViewMatrixLocation, 1, false, &projectionViewMatrix[0][0]);

            GLint orthographicMatrixLocation = glGetUniformLocation(newShader, "orthographic");
            glUniformMatrix4fv(orthographicMatrixLocation, 1, false, &this->orthographicMatrix[0][0]);

            GLint modelMatrixLocation = glGetUniformLocation(activeShader, "model");
            glUniformMatrix4fv(modelMatrixLocation, 1, false, &transform.GetGlobalMatrix()[0][0]);

            //█████████████████████████████████████████████████████████████████████████████████████
            //██████████Setting Material data for Rendering████████████████████████████████████████
            //█████████████████████████████████████████████████████████████████████████████████████

            //BaseColor
            GLint baseColorFactorLocation = glGetUniformLocation(activeShader, "baseColorFactor");

            glUniform4fv(baseColorFactorLocation, 1, &material.baseColorFactor.x);
            if (material.baseColorID > 0)
            {
                glActiveTexture(GL_TEXTURE0);
                GLint test = glGetUniformLocation(activeShader, "baseColor");
                glUniform1i(test, 0);
                glBindTexture(GL_TEXTURE_2D, material.baseColorID);
            }

            //MetallicRoughness
            GLint metallicFactorLocation = glGetUniformLocation(activeShader, "metallicFactor");
            glUniform1fv(metallicFactorLocation, 1, &material.metallicFactor);

            GLint roughnessFactorLocation = glGetUniformLocation(activeShader, "roughnessFactor");
            glUniform1fv(roughnessFactorLocation, 1, &material.roughnessFactor);
            if(material.metallicRoughnessID > 0)
            {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, material.metallicRoughnessID);
            }

            //Normal
            GLint normalScaleLocation = glGetUniformLocation(activeShader, "normalScale");
            glUniform1fv(normalScaleLocation, 1, &material.normalScale);
            if(material.normalID > 0)
            {
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, material.normalID);
            }

            //Occlusion
            GLint occlusionStrengthLocation = glGetUniformLocation(activeShader, "occlusionStrength");
            glUniform1fv(occlusionStrengthLocation, 1, &material.occlusionStrength);
            if(material.occlusionID > 0)
            {
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, material.occlusionID);
            }

            //Emissive
            GLint emissiveFactorLocation = glGetUniformLocation(activeShader, "emissiveFactor");
            glUniform3fv(emissiveFactorLocation, 1, &material.emissiveFactor.x);
            if(material.emissiveID > 0)
            {
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, material.emissiveID);
            }

            glBindVertexArray(data.vaoID);
            if(data.material.isDoubleSided)
                glDisable(GL_CULL_FACE);
            else
                glEnable(GL_CULL_FACE);

            if(data.indexBufferID > 0)
            {
                const tinygltf::Accessor& indices = meshRenderer.model->accessors[primitive.indices];
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.indexBufferID);
                glDrawElements(primitive.mode, indices.count, indices.componentType, (void *) indices.byteOffset);
            }
            else
            {
                glDrawArrays(primitive.mode, 0, data.vertexCount);
            }
        }
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

        defaultShader.SetVertexShader(LoadShader(pathToDefaultVertexShader, GL_VERTEX_SHADER));
        defaultShader.SetFragmentShader(LoadShader(pathToDefaultFragmentShader, GL_FRAGMENT_SHADER));

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
                if(material.pbrMetallicRoughness.baseColorTexture.index > -1)
                {
                    const tinygltf::Texture &baseColor = model->textures[material.pbrMetallicRoughness.baseColorTexture.index];
                    LoadTexture(baseColor, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.pbrMetallicRoughness.metallicRoughnessTexture.index > -1)
                {
                    const tinygltf::Texture &metallicRoughness = model->textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
                    LoadTexture(metallicRoughness, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.normalTexture.index > -1)
                {
                    const tinygltf::Texture &normal = model->textures[material.normalTexture.index];
                    LoadTexture(normal, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.occlusionTexture.index > -1)
                {
                    const tinygltf::Texture &occlusion = model->textures[material.occlusionTexture.index];
                    LoadTexture(occlusion, *model, TINYGLTF_TEXTURE_FORMAT_RGBA, TINYGLTF_TEXTURE_FORMAT_RGBA);
                }
                if(material.emissiveTexture.index > -1)
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
        glGenerateMipmap(GL_TEXTURE_2D);

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
            data.shader = defaultShader;

            if (primitive.material > -1)
            {
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

                material.isDoubleSided = model->materials[primitive.material].doubleSided;
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

    std::unique_ptr<std::string> RenderSystem::ResolveIncludesForGLSL(const std::filesystem::path &filePath, std::unique_ptr<std::string> file)
    {
        static const char* includeIdentifier = "#include";

        unsigned long long int position = 0;
        unsigned long long int startOfInclude = 0;

        while(position < file->length())
        {
            position = file->find(includeIdentifier);
            startOfInclude = position;
            if(position == std::string::npos)
                break;

            while(file->c_str()[position] != '<' && position < file->length())
                position++;
            if(position >= file->length())
            {
                std::string message{"Syntax error in: "};
                message += filePath.string();
                message += "\nidk which line or which file, but you wrote \"#include\" without <filename>";
                throw std::runtime_error(message);
            }

            //position now points to '<'
            position++;
            //position now points to the first character of the filename

            unsigned long long int lastPosOfName = position;
            while(file->c_str()[lastPosOfName] != '>' && lastPosOfName < file->length())
                lastPosOfName++;
            if(position >= file->length())
            {
                std::string message{"Syntax error in: "};
                message += filePath.string();
                message += "\nidk which line or which file, but you wrote \"#include <filename\" without \'>\'";
                throw std::runtime_error(message);
            }

            //lastPosOfName now points to '>'
            unsigned int fileNameLength = lastPosOfName - position;
            unsigned int includeStatementLength = lastPosOfName - startOfInclude + 1;

            std::string fileName = file->substr(position, fileNameLength);
            std::filesystem::path includedFilePath = filePath.parent_path() / fileName;
            std::ifstream fileStream(includedFilePath);
            if(!fileStream)
            {
                std::string message{ "Include statement, File not found: "};
                message += includedFilePath.string();
                throw std::runtime_error(message);
            }

            std::ostringstream stringStream;
            stringStream << fileStream.rdbuf();
            file->replace(startOfInclude, includeStatementLength, stringStream.str());

            position = startOfInclude;
        }

        std::string resolvedFileName = "Resolved" + filePath.filename().string();
        std::ofstream outputStream(Engine::Files::ASSETS / "Shaders/ResolvedShaderOutput" / resolvedFileName, std::ios::trunc);
        outputStream << file->c_str();
        outputStream.close();
        return file;
    }

    /**
     *
     * @param pathToShader
     * @param shaderType One of GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
     * @return
     */
    GLuint RenderSystem::LoadShader(const std::filesystem::path &pathToShader, GLenum shaderType)
    {
        std::unique_ptr<std::string> shaderText = ResolveIncludesForGLSL(pathToShader, ReadShaderFromFile(pathToShader));
        const char* shaderRawText = shaderText->c_str();

        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderRawText, nullptr);
        glCompileShader(shader);

        int hasSuccessfullyCompiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasSuccessfullyCompiled);
        if (!hasSuccessfullyCompiled)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "Error in shader " << pathToShader << ": \n";
            std::cout << infoLog;
            exit(-1);
        }
        return shader;
    }

    void RenderSystem::CreateShaderProgram(Shader &shader)
    {
        auto existingShader = std::find(loadedShaders.begin(), loadedShaders.end(), shader);
        if(existingShader != loadedShaders.end())
        {
            shader.id = existingShader->id;
            shader.hasChanged = false;
            return;
        }

        GLuint shaderProgram = glCreateProgram();
        if(shader.vertexID != 0)
            glAttachShader(shaderProgram, shader.vertexID);
        if(shader.tesselationControlID != 0)
            glAttachShader(shaderProgram, shader.tesselationControlID);
        if(shader.tesselationEvaluationID != 0)
            glAttachShader(shaderProgram, shader.tesselationEvaluationID);
        if(shader.geometryID != 0)
            glAttachShader(shaderProgram, shader.geometryID);
        if(shader.fragmentID != 0)
            glAttachShader(shaderProgram, shader.fragmentID);

        glLinkProgram(shaderProgram);

        GLint hasSuccessfullyCompiled;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &hasSuccessfullyCompiled);
        if (!hasSuccessfullyCompiled)
        {
            char infoLog[512];
            std::cout << "Error during linking a shader program. Good luck finding out, what the problem is. Here is a tip: \n";
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << infoLog;
            exit(-1);
        }

        shader.id = shaderProgram;
        shader.hasChanged = false;

        loadedShaders.push_back(shader);
    }

    void RenderSystem::EntityAdded(Entity entity)
    {

    }

    void RenderSystem::EntityRemoved(Entity entity)
    {

    }

} // Engine