#include "Dungeon.h"
#include "../tinygltf/stb_image.h"
#define STBI_FAILURE_USERMSG
#include <stdexcept>
#include <string>
#include "ECS/Components/PlayerController.h"

Dungeon::Dungeon(std::filesystem::path pathToImage, GameObject* wallPrefab)
{
    gameObject = new GameObject("Dungeon");
    gameObject->GetComponent<Engine::Transform>().SetRotation(glm::quat(glm::vec3(glm::radians(dungeonRotation), 0, 0)));

    int width, height, channels;
    std::string path = pathToImage.string();
    unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if(image == nullptr)
    {
        std::string message{"ERROR while loading the image: "};
        message += path;
        message += "\n";
        message += stbi_failure_reason();
        std::cout << message << "\n";
        throw std::runtime_error(message);
    }


    for(int x = 0; x < width; x++)
    {
        wallMap.push_back(std::vector<bool>());
        for(int y = 0; y < height; y++)
        {
            unsigned char alpha;
            int pos = (((height - y - 1) * width) + x) * 4;
            alpha = *(image + pos + 3);

            bool isWall = alpha >= 128;
            wallMap[x].push_back(isWall);
            if(!isWall) continue;

            std::string name = "Wall(";
            name += std::to_string(x);
            name += "|";
            name += std::to_string(y);
            name += ")";

            GameObject* wall = new GameObject(*wallPrefab);

            if(rand() % 2 == 0)
            {
                Engine::PlayerController& controller = wall->AddComponent<Engine::PlayerController>();
                controller.SetMovementInput(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
                controller.speed = ((rand() % 1000) + 1) / 100.0f;
            }

            wall->GetComponent<Engine::Name>() = name;
            wall->GetComponent<Engine::Transform>().SetTranslation(glm::vec3(x - width / 2, y - height / 2, 0));
            wall->SetParent(gameObject);
        }
    }


}