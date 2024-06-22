#include "Keyboard.h"
#include "ECSExtension.h"

Keyboard::Keyboard(glm::vec2 position, int scale, int charsPerLine)
{
    this->position = position;
    this->scale = scale;
    this->charsPerLine = charsPerLine;
    xPixel = scale * 16;
    yPixel = scale * 16;

    selector = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics/Models/Selector.glb")[0];
    ecsSystem->GetComponent<Engine::Transform>(selector).SetRotation(glm::vec3(glm::radians(90.0f), 0, 0));
    ecsSystem->GetComponent<Engine::Transform>(selector).SetScale(glm::vec3(xPixel));
    ecsSystem->GetComponent<Engine::MeshRenderer>(selector).renderLayer = Engine::UI;
    GLuint shader = Engine::Systems::renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Default/VS_UI_Default.vert", GL_VERTEX_SHADER);
    for(auto const& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(selector))
        renderer->SetVertexShader(shader);


    int i = 0;
    int lines = (int)::ceilf(26.0f / charsPerLine);
    for(int y = 0; y < lines; y++)
        for(int x = 0; x < charsPerLine; x++)
        {
            Engine::Entity key = ecsSystem->CreateEntity();
            char ch = (char)('a' + i);
            keys.insert({ch, key});
            Engine::Text& text = ecsSystem->AddComponent<Engine::Text>(key);
            text.SetText(std::string("") + ch);
            text.scale = scale;
            text.position = position + glm::vec2(xPixel * x, yPixel * y);
            text.horizontalAlignment = Engine::Text::Center;
            text.verticalAlignment = Engine::Text::Center;
            i++;
        }

    SetPos(0, 0);
}

Keyboard::Keyboard(std::string startText, int charsPerLine, glm::vec2 position, int scale) : Keyboard(position, scale, charsPerLine)
{
    text = startText;
}

Keyboard::~Keyboard()
{
    Engine::Systems::inputSystem->Remove(movement);
    Engine::Systems::inputSystem->Remove(enter);
    Engine::Systems::inputSystem->Remove(del);

    for(auto pair: keys)
        ecsSystem->RemoveEntity(pair.second);

    ecsSystem->RemoveEntity(selector);
}


std::string Keyboard::GetText()
{
    return text;
}

void Keyboard::SetText(std::string text)
{
    this->text = text;
}


void Keyboard::SetPos(int x, int y)
{
    selectorPos = x + y * charsPerLine;
    selectorPos = std::clamp(selectorPos, 0, 25);

    ecsSystem->GetComponent<Engine::Transform>(selector).SetTranslation(glm::vec3(position, 0) + glm::vec3(x * xPixel, y * yPixel, 0));
}

void Keyboard::AddPos(int x, int y)
{
    selectorPos += x + y * charsPerLine;
    selectorPos = std::clamp(selectorPos, 0, 25);

    ecsSystem->GetComponent<Engine::Transform>(selector).SetTranslation(glm::vec3(position, 0) + glm::vec3((selectorPos % charsPerLine) * xPixel, (selectorPos / charsPerLine) * yPixel, 0));
}

void Keyboard::EnterChar()
{
    if(text.size() >= maxChars) return;
        text += (char)('a' + selectorPos);
}

void Keyboard::DeleteChar()
{
    if(!text.empty())
        text.pop_back();
}

void Keyboard::SetMovementAction(std::shared_ptr<Engine::InputActionVec2> action)
{
    movement = action;
    movement->AddOnStart(this, OnMovementStart);
    Engine::Systems::inputSystem->Add(movement);
}

void Keyboard::SetEnterAction(std::shared_ptr<Engine::InputActionButton> action)
{
    enter = action;
    enter->AddOnStart(this, OnEnter);
    Engine::Systems::inputSystem->Add(enter);
}

void Keyboard::SetDeleteAction(std::shared_ptr<Engine::InputActionButton> action)
{
    del = action;
    del->AddOnStart(this, OnDelete);
    Engine::Systems::inputSystem->Add(del);
}


void Keyboard::OnMovementStart(void *o, glm::vec2 input)
{
    Keyboard* keyboard = static_cast<Keyboard*>(o);
    int x = input.x > 0 ? 1 : (input.x < 0 ? -1 : 0);
    int y = input.y > 0 ? 1 : (input.y < 0 ? -1 : 0);
    y *= -1;
    keyboard->AddPos(x, y);
}

void Keyboard::OnEnter(void *o)
{
    Keyboard* keyboard = static_cast<Keyboard*>(o);
    keyboard->EnterChar();
}

void Keyboard::OnDelete(void *o)
{
    Keyboard* keyboard = static_cast<Keyboard*>(o);
    keyboard->DeleteChar();
}
