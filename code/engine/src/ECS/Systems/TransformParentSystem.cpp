#include "Engine.h"
#include "ECS/Systems/TransformParentSystem.h"
#include <fstream>
#include <list>
#include <cstdint>
extern Engine::ECSSystem* ecsSystem;
namespace Engine
{

    std::string GetHierarchy(Transform& transform, Entity entity, int depth = 0)
    {
        Name& name = ecsSystem->GetComponent<Name>(entity);
        std::string result;
        result += std::to_string((std::uintptr_t)&transform);
        for (int i = -1; i < depth; i++)
            result += "\t";
        result += name + " Parent: ";

        if(transform.GetParent() == nullptr)
            result += " None";
        else
            result += ecsSystem->GetComponent<Name>(ecsSystem->GetEntity(*transform.GetParent()));
        result += "\n";

        for(Transform* childTr : transform.GetChildren())
        {
            result += GetHierarchy(*childTr, ecsSystem->GetEntity(*childTr), depth + 1);
        }

        return result;
    }

    void PrintHierarchy(std::set<Entity>& entities, std::string title)
    {
        static int i = 0;
        if(i == 0)
        {
            for(auto bla : std::filesystem::directory_iterator("C:\\Users\\Yanni\\Desktop\\Ake"))
                std::filesystem::remove_all(bla);
        }
        std::ofstream stream;
        stream.open("C:\\Users\\Yanni\\Desktop\\Ake\\Hierarchy_" + std::to_string(i) + ".txt");

        stream << title << std::endl;
        for(Entity entity : entities)
        {
            Transform& transform = ecsSystem->GetComponent<Transform>(entity);
            Name& name = ecsSystem->GetComponent<Name>(entity);
            if(name == "Dungeon") continue;
            if(transform.GetParent() != nullptr)
            {
                //stream << name << " has parent " << ecsSystem->GetComponent<Name>(ecsSystem->GetEntity(*transform.GetParent())) << std::endl;
                continue;
            }

            stream << GetHierarchy(transform, entity);
        }
        stream.flush();
        stream.close();
        i++;
    }
    void TransformParentSystem::EntityAdded(Entity entity)
    {

    }

    void TransformParentSystem::EntityRemoved(Entity entity)
     {
        PrintHierarchy(entities, ecsSystem->GetComponent<Name>(entity) + " is going to be removed");
        Transform& transform = ecsSystem->GetComponent<Transform>(entity);

        if(transform.GetParent() != nullptr)
        {
            std::list<Transform *> &transformList = transform.GetParent()->GetChildren();
            transformList.remove(&transform);
        }

        std::list<Transform*>& children = transform.GetChildren();
        std::cout << ecsSystem->GetComponent<Name>(entity);
        for(Transform* bla : children)
        {
            std::cout << bla << std::endl;
        }
        std::cout << std::endl;

        while(!transform.GetChildren().empty())
        {
            children.front()->SetParent(nullptr);
        }

         PrintHierarchy(entities, ecsSystem->GetComponent<Name>(entity) + " was removed");

        if("Hand_R Cloned 1" == ecsSystem->GetComponent<Name>(entity))
            int x = 0;
    }

    void TransformParentSystem::Print()
    {
        static int i = 0;
        i++;

        if(i == 144)
        {
            //PrintHierarchy(entities);
            i = 0;
        }
    }
} // Engine