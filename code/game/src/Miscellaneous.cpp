#include "Miscellaneous.h"
#include "../tinygltf/stb_image.h"
#include "../tinygltf/stb_image_write.h"
#include <stdexcept>
#include <string>
#define STBI_FAILURE_USERMSG

GameObject *Miscellaneous::CreateDungeonFromImage(std::filesystem::path pathToImage, GameObject* wallPrefab, float dungeonrotation)
{
    GameObject* dungeon = new GameObject("Dungeon");
    dungeon->GetComponent<Engine::Transform>().SetRotation(glm::quat(glm::vec3(glm::radians(dungeonrotation), 0, 0)));

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
        for(int y = 0; y < height; y++)
        {
            unsigned char alpha;
            int pos = (((height - y - 1) * width) + x) * 4;
            alpha = *(image + pos + 3);
            if(alpha < 128) continue;

            std::string name = "Wall(";
            name += std::to_string(x);
            name += "|";
            name += std::to_string(y);
            name += ")";

            GameObject* wall = new GameObject(*wallPrefab);
            wall->GetComponent<Engine::Name>() = name;
            wall->GetComponent<Engine::Transform>().SetTranslation(glm::vec3(x - width / 2, y - height / 2, 0));
            wall->SetParent(dungeon);
        }
    }

    return dungeon;
}
