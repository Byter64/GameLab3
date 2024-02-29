#include "TextRenderSystem.h"
#include "Engine.h"
namespace Engine
{
    void TextRenderSystem::EntityAdded(Engine::Entity entity)
    {

    }

    void TextRenderSystem::EntityRemoved(Engine::Entity entity)
    {

    }

    void TextRenderSystem::Render()
    {
        gltBeginDraw();
        gltColor(1.0f, 1.0f,1.0f ,1.0f);
        for(Entity entity : entities)
        {
            Text& text = ecsSystem->GetComponent<Text>(entity);
            gltDrawText2D(text.text.get(), text.position.x, text.position.y, text.scale);
        }
        gltEndDraw();
    }

    void TextRenderSystem::Initialize(int screenWidth, int screenHeight)
    {
        gltInit();
        gltViewport(screenWidth, screenHeight);
    }

    void TextRenderSystem::SetViewport(int screenWidth, int screenHeight)
    {
        gltViewport(screenWidth, screenHeight);
    }
} // Engine