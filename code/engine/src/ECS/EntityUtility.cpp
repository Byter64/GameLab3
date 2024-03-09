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

extern std::shared_ptr<Engine::RenderSystem> renderSystem;
namespace Engine
{
    Entity hubertusPrefab = Entity::INVALID_ENTITY_ID;
    Entity wallPrefab = Entity::INVALID_ENTITY_ID;

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
        {
            return root;
        }

        std::queue<Entity> entityQueue;
        entityQueue.push(root);

        while(!entityQueue.empty())
        {
            Entity subject = entityQueue.front();
            entityQueue.pop();

            if(ecsSystem->GetComponent<Name>(subject) == name)
            {
                return subject;
            }

            for(Transform* childTransform : ecsSystem->GetComponent<Transform>(subject).GetChildren())
            {
                entityQueue.push(ecsSystem->GetEntity(*childTransform));
            }
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
     * @return
     */
    std::vector<Entity> ImportGLTF(std::filesystem::path filePath, bool addParent)
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
        if(ecsSystem->HasComponent<PlayerController>(entity))
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<PlayerController>(entity));
        if(ecsSystem->HasComponent<Health>(entity))
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Health>(entity));
        if(ecsSystem->HasComponent<Dungeon>(entity))
            ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Dungeon>(entity));
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


    Entity SpawnEnemy(std::pair<int, int> startPos, EnemyBehaviour::Behaviour behaviour)
    {
        if(hubertusPrefab == Entity::INVALID_ENTITY_ID)
        {
            hubertusPrefab = ImportGLTF(Files::ASSETS/ "Graphics\\Models\\Hubertus\\Hubertus.glb")[0];
            ecsSystem->GetComponent<Transform>(hubertusPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
            ecsSystem->GetComponent<Transform>(hubertusPrefab).SetScale(glm::vec3(0.0f));
        }
        Entity enemy = CopyEntity(hubertusPrefab, true);
        ecsSystem->GetComponent<Transform>(enemy).SetScale(glm::vec3(1.0f));
        ecsSystem->AddComponent<BoxCollider>(enemy, BoxCollider());
        ecsSystem->GetComponent<BoxCollider>(enemy).size = glm::vec3(0.5f);
        ecsSystem->GetComponent<BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
        ecsSystem->AddComponent<EnemyBehaviour>(enemy, EnemyBehaviour());
        ecsSystem->GetComponent<EnemyBehaviour>(enemy).behaviour = behaviour;
        ecsSystem->GetComponent<EnemyBehaviour>(enemy).startPos = startPos;
        ecsSystem->AddComponent<Health>(enemy, Health());
        ecsSystem->GetComponent<Health>(enemy).health = 3;

        return enemy;
    }

    Entity SpawnWall(glm::vec3 position)
    {
        if(wallPrefab == Entity::INVALID_ENTITY_ID)
        {
            wallPrefab = ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Wall\\Wall.glb")[0];
            ecsSystem->GetComponent<Transform>(wallPrefab).SetScale(glm::vec3(0.0f));
        }

        Entity wall = CopyEntity(wallPrefab);

        ecsSystem->GetComponent<Transform>(wall).SetTranslation(position);
        ecsSystem->GetComponent<Transform>(wall).SetScale(glm::vec3(1.0f));

        Engine::BoxCollider& wallCollider = ecsSystem->AddComponent<Engine::BoxCollider>(wall);
        wallCollider.size = glm::vec3(1,1, 1000);
        wallCollider.position = glm::vec3 (0);
        wallCollider.isStatic = true;
        wallCollider.layer = static_cast<unsigned char>(CollisionLayer::Dungeon);

        return wall;
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

    Entity GenerateEntities(const tinygltf::Node& root, Engine::Transform* parent, std::shared_ptr<tinygltf::Model> model)
    {
        Engine::Entity entity = ecsSystem->CreateEntity();

        Engine::Name name = root.name;
        ecsSystem->AddComponent(entity, name);

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
        ecsSystem->AddComponent(entity, transform);
        ecsSystem->GetComponent<Engine::Transform>(entity).SetParent(parent);

        if (root.mesh != -1)
        {
            Engine::MeshRenderer meshRenderer = renderSystem->CreateMeshRenderer(model->meshes[root.mesh], model);
            ecsSystem->AddComponent(entity, meshRenderer);
        }

        for (int childIndex: root.children)
        {
            const tinygltf::Node &child = model->nodes[childIndex];
            GenerateEntities(child, &ecsSystem->GetComponent<Engine::Transform>(entity), model);
        }

        return entity;
    }
} // Engine