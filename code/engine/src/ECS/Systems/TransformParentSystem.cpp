#include "Engine.h"
#include "ECS/Systems/TransformParentSystem.h"
#include <fstream>
#include <list>
#include <cstdint>
extern Engine::ECSSystem* ecsSystem;

extern std::shared_ptr<Engine::TransformParentSystem> transformParentSystem;
namespace Engine
{
    std::string GetHierarchy(Transform& transform, Entity entity, int depth = 0)
    {
        Name& name = ecsSystem->GetComponent<Name>(entity);
        std::string result;
        for (int i = 0; i < depth; i++)
            result += "\t";
        result += name + " Parent: ";

        if(transform.GetParent() == nullptr)
            result += " None";
        else
            result += ecsSystem->GetComponent<Name>(ecsSystem->GetEntity(*transform.GetParent()));
        result += "\n\tChildren: ";

        if(transform.GetChildren().empty())
            result += "None";
        for(Transform* childTr : transform.GetChildren())
        {
            result += ecsSystem->GetComponent<Name>(ecsSystem->GetEntity(*childTr)) + ", ";
        }

        result+= "\n";
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

    void TransformParentSystem::Print(std::string title)
    {
        PrintHierarchy(transformParentSystem->entities, title);
    }

    void TransformParentSystem::EntityAdded(Entity entity)
    {

    }

    void TransformParentSystem::EntityRemoved(Entity entity)
     {
         static int i = 0;
         if(ecsSystem->GetComponent<Name>(entity).find("Plane") == std::string::npos || i >= 6)
         {
             i++;
             Print(ecsSystem->GetComponent<Name>(entity) + " was removed");
         }
        Transform& transform = ecsSystem->GetComponent<Transform>(entity);

        if(transform.GetParent() != nullptr)
        {
            std::list<Transform *> &transformList = transform.GetParent()->GetChildren();
            transformList.remove(&transform);
        }

        std::list<Transform*>& children = transform.GetChildren();
        while(!children.empty())
        {
            children.front()->SetParent(nullptr);
        }
    }

} // Engine