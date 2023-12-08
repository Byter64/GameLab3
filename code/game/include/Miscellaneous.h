#pragma once
#include "GameObject.h"
#include <filesystem>

class Miscellaneous
{
private:

public:
    static GameObject* CreateDungeonFromImage(std::filesystem::path image, GameObject* wallPrefab, float dungeonrotation);
};
