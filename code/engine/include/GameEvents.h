#pragma once
#include "Engine.h"

namespace Engine
{
    struct GameEvents
    {
        virtual void Update(float time) = 0;

        virtual void OnCollisionEnter(Engine::BoxCollider &other) = 0;

        virtual void OnCollisionStay(Engine::BoxCollider &other) = 0;

        virtual void OnCollisionExit(Engine::BoxCollider &other) = 0;
    };
}