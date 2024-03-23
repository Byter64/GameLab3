#include "Engine.h"
//For whatever reason, these defines are not allowed to be written before glad is included (glad is also included in Engine.h)
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include "../../extern/tinygltf/tiny_gltf.h"
#include "EntityUtility.h"

#include <iostream>
#include <queue>
#include "CollisionLayer.h"
#include "ECS/Helpers/Animation.h"

namespace Engine
{
    Entity GenerateEntities(const tinygltf::Node& root, Transform* parent, std::shared_ptr<tinygltf::Model> model);
    Animation GenerateAnimation(const tinygltf::Animation& gltfAnimation, std::shared_ptr<tinygltf::Model> model);
    std::vector<unsigned int> FindHierarchy(const tinygltf::Node& node, std::shared_ptr<tinygltf::Model> model, const tinygltf::Node& currentNode = tinygltf::Node(), std::vector<unsigned int> hierarchy = std::vector<unsigned int>());

    /**
     * Finds a child with a name name in root and returns it. Prints a warning. if rot has not both a Name and a Transform component.
     * Uses breadth-first search.
     * @param root A parent of the looked for child. Does not need to be a direct parent.
     * @param name The name of the child which is looked for
     * @return The found entity, or INVALID_ENTITY_ID if none was found.
     */
    Entity FindChild(Entity root, Name name)
    {
        if(!ecsSystem->HasComponent<Name>(root))
        {
            std::cout << "ERROR: FindChild(Entity root, Name name) was called with a root that does not have a Name component. \n"
                      << "EntityID of root is: " << root << "\n";
            return Entity::INVALID_ENTITY_ID;
        }

        if(!ecsSystem->HasComponent<Transform>(root))
        {
            std::cout << "ERROR: FindChild(Entity root, Name name) was called with a root that does not have a Transform component. \n"
                      << "EntityID of root is: " << root << "\n"
                      << "Entity name of root is: " << ecsSystem->GetComponent<Name>(root).c_str() << "\n";
            return Entity::INVALID_ENTITY_ID;
        }

        if(ecsSystem->GetComponent<Name>(root) == name)
            return root;

        std::queue<Entity> entityQueue;
        entityQueue.push(root);

        while(!entityQueue.empty())
        {
            Entity subject = entityQueue.front();
            entityQueue.pop();

            if(ecsSystem->GetComponent<Name>(subject) == name)
                return subject;

            for(Transform* childTransform : ecsSystem->GetComponent<Transform>(subject).GetChildren())
                entityQueue.push(ecsSystem->GetEntity(*childTransform));
        }

        std::cout << "WARNING: No child with name \"" << name << "\" in " << root << " found. \n"
                  << "Root name is: \"" << ecsSystem->GetComponent<Name>(root) << "\".\n";
        return Entity::INVALID_ENTITY_ID;
    }

    /**
     * Loads a .gltf or .glb from the given path and returns entities for all upper nodes in the first scene.
     * Name, Transform and MeshRenderer components are automatically assigned to the entities in correspondence to the gltf.
     * Only the first scene is loaded, all others are ignored.
     * If the file format is .gltf, all resources need to be embedded into this file.
     * @param filePath
     * @param addParent if true, the returned vector only contains one entity, to which all top entities are parented
     * to.
     * @param animationPrefix the prefix that will be added to animations. E.g. prefix "Player_" + animation "Jump" ==> "Player_Jump"
     * @return
     */
    std::vector<Entity> ImportGLTF(std::filesystem::path filePath, std::string animationPrefix, bool addParent)
    {
        std::vector<Entity> entities;
        bool hasWorked;
        std::string error, warning;
        std::shared_ptr<tinygltf::Model> model = std::make_shared<tinygltf::Model>();
        tinygltf::TinyGLTF loader;

        if(filePath.extension() == ".gltf")
            hasWorked = loader.LoadASCIIFromFile(model.get(), &error, &warning, filePath.string());
        else if(filePath.extension() == ".glb")
            hasWorked = loader.LoadBinaryFromFile(model.get(), &error, &warning, filePath.string());
        else
        {
            std::cout << "Unknown file extension of file: \n" << filePath << "\n";
            return entities;
        }

        if (!hasWorked)
        {
            std::cout << error << std::endl;
            return entities;
        }

        for(int nodeIndex : model->scenes[0].nodes)
        {
            const tinygltf::Node& node = model->nodes[nodeIndex];
            Entity entity = GenerateEntities(node, nullptr, model);
            Transform &transform = ecsSystem->GetComponent<Transform>(entity);
            transform.SetParent(nullptr);
            entities.push_back(entity);
        }

        if(addParent)
        {
            Entity parent = ecsSystem->CreateEntity();
            ecsSystem->AddComponent<Name>(parent, filePath.stem().string());
            Transform &transform = ecsSystem->AddComponent<Transform>(parent);
            transform.SetParent(nullptr);
            transform.SetRotation(glm::identity<glm::quat>());
            transform.SetTranslation(glm::vec3(0));
            transform.SetScale(glm::vec3(1));

            for(int i = 0; i < entities.size(); i++)
            {
                ecsSystem->GetComponent<Transform>(entities[i]).SetParent(&transform);
            }
            entities.clear();
            entities.push_back(parent);
        }

        //Add all animations in the model to the animation system
        for(tinygltf::Animation& gltfAnimation : model->animations)
        {
            Animation animation = GenerateAnimation(gltfAnimation, model);
            animation.name = animationPrefix + animation.name;
            Systems::animationSystem->AddAnimation(animation);
        }

        return entities;
    }

    Entity CopyEntity(Entity entity, bool copyChildren = true)
    {
        Entity newEntity = ecsSystem->CreateEntity();

        if(ecsSystem->HasComponent<Name>(entity))
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Name>(entity) + " Cloned");
        if(ecsSystem->HasComponent<MeshRenderer>(entity))
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<MeshRenderer>(entity));
        if(ecsSystem->HasComponent<BoxCollider>(entity))
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<BoxCollider>(entity));
        if(ecsSystem->HasComponent<Text>(entity))
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Text>(entity));

        if(ecsSystem->HasComponent<Transform>(entity))
        {
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Transform>(entity));
            Transform& newTransform = ecsSystem->GetComponent<Transform>(newEntity);
            newTransform.GetChildren().clear();

            if(copyChildren)
            {
                Transform& transform = ecsSystem->GetComponent<Transform>(entity);
                for(Transform* childTransform : transform.GetChildren())
                {
                    Entity child = ecsSystem->GetEntity(*childTransform);
                    Entity newChild = CopyEntity(child, copyChildren);
                    Transform& newChildTransform = ecsSystem->GetComponent<Transform>(newChild);
                    newChildTransform.SetParent(&newTransform);
                }
            }
        }

        return newEntity;
    }

    /// Removes the given entity plus all children within the transform hierarchy. In case entity does not have a Transform or no children, RemoveEntityWithChildren will behave identical with ECSSystem.RemoveEntity
    /// \param entity
    void RemoveEntityWithChildren(Entity entity, bool removeParent)
    {
        if(ecsSystem->HasComponent<Transform>(entity))
        {
            Transform& transform = ecsSystem->GetComponent<Transform>(entity);
            for(auto& childTransform : transform.GetChildren())
            {
                Entity child = ecsSystem->GetEntity<Transform>(*childTransform);
                RemoveEntityWithChildren(child, true);
            }
        }

        if(removeParent)
            ecsSystem->RemoveEntity(entity);
    }

    Entity GenerateEntities(const tinygltf::Node& root, Transform* parent, std::shared_ptr<tinygltf::Model> model)
    {
        Entity entity = ecsSystem->CreateEntity();

        Name name = root.name;
        ecsSystem->AddComponent(entity, name);

        Transform transform;
        glm::vec3 translation{0};
        glm::vec3 scale{1};
        glm::quat rotation = glm::identity<glm::quat>();
        if (!root.translation.empty())
        { translation = glm::vec3{(float) root.translation[0], (float) root.translation[1], (float) root.translation[2]}; }
        if (!root.scale.empty())
        { scale = glm::vec3{(float) root.scale[0], (float) root.scale[1], (float) root.scale[2]}; }
        if (!root.rotation.empty())
        { rotation = glm::quat{ (float) root.rotation[3], (float) root.rotation[0], (float) root.rotation[1], (float) root.rotation[2]}; }

        transform.SetTranslation(translation);
        transform.SetScale(scale);
        transform.SetRotation(rotation);
        ecsSystem->AddComponent(entity, transform);
        ecsSystem->GetComponent<Transform>(entity).SetParent(parent);

        if (root.mesh != -1)
        {
            MeshRenderer meshRenderer = Systems::renderSystem->CreateMeshRenderer(model->meshes[root.mesh], model);
            ecsSystem->AddComponent(entity, meshRenderer);
        }

        for (int childIndex: root.children)
        {
            const tinygltf::Node &child = model->nodes[childIndex];
            GenerateEntities(child, &ecsSystem->GetComponent<Transform>(entity), model);
        }

        return entity;
    }

    Animation GenerateAnimation(const tinygltf::Animation& gltfAnimation, std::shared_ptr<tinygltf::Model> model)
    {
        Animation animation;
        animation.name = gltfAnimation.name;

        for(int i = 0; i < gltfAnimation.channels.size(); i++)
        {
            const tinygltf::AnimationChannel& gltfChannel = gltfAnimation.channels[i];
            const tinygltf::AnimationSampler& sampler = gltfAnimation.samplers[gltfChannel.sampler];
            const tinygltf::Accessor& domain = model->accessors[sampler.input];
            const tinygltf::Accessor& codomain = model->accessors[sampler.output];
            const tinygltf::BufferView& domainBV = model->bufferViews[domain.bufferView];
            const tinygltf::BufferView& codomainBV = model->bufferViews[codomain.bufferView];
            const tinygltf::Buffer& domainBuf = model->buffers[domainBV.buffer];
            const tinygltf::Buffer& codomainBuf = model->buffers[codomainBV.buffer];

            Animation::Channel channel;

            channel.target = Animation::Channel::StringToTarget(gltfChannel.target_path);
            channel.interpolation = Animation::Channel::StringToInterpolation((sampler.interpolation));

            if(channel.target != Animation::Channel::Target::Rotation) continue;

            const float* domainPointer = reinterpret_cast<const float*>(&domainBuf.data[domainBV.byteOffset + domain.byteOffset]);
            const float* codomainPointer = reinterpret_cast<const float*>(&codomainBuf.data[codomainBV.byteOffset + codomain.byteOffset]);
            if(domain.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT || codomain.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
            {
                throw std::runtime_error("feeec");
            }

            if(codomain.type == TINYGLTF_TYPE_VEC4)
                channel.functionTo4D = std::map<float, glm::quat>();
            else if (codomain.type == TINYGLTF_TYPE_VEC3)
                channel.functionTo3D = std::map<float, glm::vec3>();

            for(int i = 0; i < domain.count; i++)
            {
                if(codomain.type == TINYGLTF_TYPE_VEC4)
                {
                    glm::quat value;
                    value.x = codomainPointer[(i * 4) + 0];
                    value.y = codomainPointer[(i * 4) + 1];
                    value.z = codomainPointer[(i * 4) + 2];
                    value.w = codomainPointer[(i * 4) + 3];
                    channel.functionTo4D[domainPointer[i]] = value;
                }
                else if (codomain.type == TINYGLTF_TYPE_VEC3)
                {
                    glm::vec3 value;
                    value.x = codomainPointer[(i * 4) + 0];
                    value.y = codomainPointer[(i * 4) + 1];
                    value.z = codomainPointer[(i * 4) + 2];
                    channel.functionTo3D[domainPointer[i]] = value;
                }
            }

            channel.hierarchy = FindHierarchy(model->nodes[gltfChannel.target_node], model);
            channel.hierarchy.erase(channel.hierarchy.begin()); //The to be deleted element is the index within the root nodes. This index however is not needed

            if(channel.target == Animation::Channel::Target::Rotation)
            {
                auto iter = channel.functionTo4D.end();
                std::advance(iter, -1);
                if(iter->first > animation.endTime)
                    animation.endTime = iter->first;

                if(channel.functionTo4D.begin()->first < animation.startTime)
                    animation.startTime = channel.functionTo4D.begin()->first;
            }
            else
            {
                auto iter = channel.functionTo3D.end();
                std::advance(iter, -1);
                if(iter->first > animation.endTime)
                    animation.endTime = iter->first;

                if(channel.functionTo3D.begin()->first < animation.startTime)
                    animation.startTime = channel.functionTo3D.begin()->first;
            }

            animation.channels.push_back(channel);
        }

        animation.duration = animation.endTime - animation.startTime;
        return animation;
    }

    std::vector<unsigned int> FindHierarchy(const tinygltf::Node& node, std::shared_ptr<tinygltf::Model> model, const tinygltf::Node& currentNode, std::vector<unsigned int> hierarchy)
    {
        static bool foundNode = false;
        std::vector<int> children = currentNode.children;
        if(hierarchy.empty())
        {
            foundNode = false;
            children = model->scenes[0].nodes;
        }

        if(node == currentNode)
        {
            foundNode = true;
            return hierarchy;
        }

        for(int i = 0; i < children.size(); i++)
        {
            hierarchy.push_back(i);

            std::vector<unsigned int> result = FindHierarchy(node, model, model->nodes[children[i]], hierarchy);
            if(foundNode) return result;

            hierarchy.pop_back();
        }

        return hierarchy;
    }
} // Engine