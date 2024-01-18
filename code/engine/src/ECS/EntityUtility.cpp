#include "../../include/EntityUtility.h"
#include "ECS/Components/Name.h"
#include "ECS/Components/Transform.h"
#include "../../include/ECSSystem.h"
#include "ECS/Systems/RenderSystem.h"
//For whatever reason, these defines are not allowed to be written before glad is included (glad is also included in Engine.h)
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include "../../extern/tinygltf/tiny_gltf.h"
#include <iostream>
#include <queue>

extern Engine::ECSSystem ecsSystem;
extern std::shared_ptr<Engine::RenderSystem> renderSystem;
namespace Engine
{
    Entity GenerateEntities(const tinygltf::Node& root, Engine::Transform* parent, std::shared_ptr<tinygltf::Model> model);

    /**
     * Finds a child with a name name in root and returns it. Prints a warning. if rot has not both a Name and a Transform component.
     * Uses breadth-first search.
     * @param root A parent of the looked for child. Does not need to be a direct parent.
     * @param name The name of the child which is looked for
     * @return The found entity, or INVALID_ENTITY_ID if none was found.
     */
    Entity FindChild(Entity root, Name name)
    {
        if(!ecsSystem.HasComponent<Name>(root))
        {
            std::cout << "ERROR: FindChild(Entity root, Name name) was called with a root that does not have a Name component. \n"
                      << "EntityID of root is: " << root << "\n";
            return Entity::INVALID_ENTITY_ID;
        }

        if(!ecsSystem.HasComponent<Transform>(root))
        {
            std::cout << "ERROR: FindChild(Entity root, Name name) was called with a root that does not have a Transform component. \n"
                      << "EntityID of root is: " << root << "\n"
                      << "Entity name of root is: " << ecsSystem.GetComponent<Name>(root).c_str() << "\n";
            return Entity::INVALID_ENTITY_ID;
        }

        if(ecsSystem.GetComponent<Name>(root) == name)
        {
            return root;
        }

        std::queue<Entity> entityQueue;
        entityQueue.push(root);

        while(!entityQueue.empty())
        {
            Entity subject = entityQueue.front();
            entityQueue.pop();

            if(ecsSystem.GetComponent<Name>(subject) == name)
            {
                return subject;
            }

            for(Transform* childTransform : ecsSystem.GetComponent<Transform>(subject).GetChildren())
            {
                entityQueue.push(ecsSystem.GetEntity(*childTransform));
            }
        }

        std::cout << "WARNING: No child with name \"" << name << "\" in " << root << " found. \n"
                  << "Root name is: \"" << ecsSystem.GetComponent<Name>(root) << "\".\n";
        return Entity::INVALID_ENTITY_ID;
    }

    /**
     * Loads a .gltf or .glb from the given path and returns an entity that is the root of all upper nodes in the first scene.
     * Name, Transform and MeshRenderer components are automatically assigned to the entities in correspondence to the gltf.
     * Only the first scene is loaded, all others are ignored.
     * If the file format is .gltf, all resources need to be embedded into this file.
     * @param gltf
     * @return
     */
    std::vector<Entity> ImportGLTF(std::filesystem::path filePath)
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
            Transform &transform = ecsSystem.GetComponent<Transform>(entity);
            transform.SetParent(nullptr);
            entities.push_back(entity);
        }

        return entities;
    }

    Entity GenerateEntities(const tinygltf::Node& root, Engine::Transform* parent, std::shared_ptr<tinygltf::Model> model)
    {
        Engine::Entity entity = ecsSystem.CreateEntity();

        Engine::Name name = root.name;
        ecsSystem.AddComponent(entity, name);

        Engine::Transform transform;
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
        ecsSystem.AddComponent(entity, transform);
        ecsSystem.GetComponent<Engine::Transform>(entity).SetParent(parent);

        if (root.mesh != -1)
        {
            Engine::MeshRenderer meshRenderer = renderSystem->CreateMeshRenderer(model->meshes[root.mesh], model);
            ecsSystem.AddComponent(entity, meshRenderer);
        }

        for (int childIndex: root.children)
        {
            const tinygltf::Node &child = model->nodes[childIndex];
            GenerateEntities(child, &ecsSystem.GetComponent<Engine::Transform>(entity), model);
        }

        return entity;
    }
} // Engine