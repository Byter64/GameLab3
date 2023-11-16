#include "RenderSystem.h"
#include "Entity.h"
#include "ECSSystem.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "glad/glad.h"

extern Engine::ECSSystem ecsSystem;
namespace Engine
{

    void RenderSystem::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLuint projectionViewMatrixLocation = glGetUniformLocation(defaultShader, "projectionView");
        glm::mat4x4 projectionViewMatrix = projectionMatrix * camera.GetMatrix();
        glUniformMatrix4fv(projectionViewMatrixLocation, 1, false, &projectionViewMatrix[0][0]);

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

        for(int i = 0; i < meshRenderer.mesh->primitives.size(); i++)
        {
            const tinygltf::Primitive &primitive = meshRenderer.mesh->primitives[i];
            const MeshRenderer::PrimitiveData &data = meshRenderer.primitiveData[i];
            const tinygltf::Material &material = meshRenderer.model->materials[primitive.material];

            GLuint modelMatrixLocation = glGetUniformLocation(defaultShader, "model");
            glUniformMatrix4fv(modelMatrixLocation, 1, false, &matrixStack.top()[0][0]);


            //█████████████████████████████████████████████████████████████████████████████████████
            //██████████Setting Material data for Rendering████████████████████████████████████████
            //█████████████████████████████████████████████████████████████████████████████████████

            //BaseColor
            GLuint baseColorFactorLocation = glGetUniformLocation(defaultShader, "baseColorFactor");
            std::vector<float> baseColorFactor{material.pbrMetallicRoughness.baseColorFactor.begin(),
                                               material.pbrMetallicRoughness.baseColorFactor.end()};
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
            std::vector<float> emissiveFactor{material.emissiveFactor.begin(), material.emissiveFactor.end()};
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

    }
} // Engine