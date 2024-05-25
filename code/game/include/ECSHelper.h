#pragma once
#include "Engine.h"

class ECSHelper
{
public:
    static void Initialize();
    static Engine::Entity CopyEntity(Engine::Entity entity, bool copyChildren = true);
};
