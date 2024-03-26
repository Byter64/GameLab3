#include "ECS/Systems/TextRenderSystem.h"
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
            gltDrawText2DAligned(text.text.get(), text.position.x * resolutionScale.first, text.position.y * resolutionScale.second,
                                 text.scale * resolutionScale.first, text.horizontalAlignment, text.verticalAlignment);
        }
        gltEndDraw();
    }

    void TextRenderSystem::Initialize(int screenWidth, int screenHeight)
    {
        gltInit();
        targetResolution = {screenWidth, screenHeight};
        gltViewport(screenWidth, screenHeight);
    }

    void TextRenderSystem::SetViewport(int screenWidth, int screenHeight)
    {
        gltViewport(screenWidth, screenHeight);
        targetResolution = {screenWidth, screenHeight};
        resolutionScale = {screenWidth / (float)referenceResolution.first, screenHeight / (float)referenceResolution.second};
    }

    //This reference resolution should have the same aspect ratio as the actual resolution
    void TextRenderSystem::SetReferenceResolution(int width, int height)
    {
        referenceResolution = {width, height};
        resolutionScale = {targetResolution.first / (float)width, targetResolution.second / (float)height};
    }
} // Engine