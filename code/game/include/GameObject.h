#pragma once
#include "Engine.h"
#include <string>
#include <memory>
#include <vector>

extern Engine::ECSSystem ecsSystem;

class GameObject
{

public:


    static GameObject* CreateGameObjectFromGLTF(std::filesystem::path filePath);

};
