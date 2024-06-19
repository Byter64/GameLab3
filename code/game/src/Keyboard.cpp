//
// Created by Yannik Stamm on 19.06.2024.
//

#include "Keyboard.h"


Keyboard::Keyboard(glm::vec2 position, int scale)
{
    this->scale = scale;

    int i = 0;
    int lines = (int)::ceilf(26.0f / charsPerLine);
    int xPixel = scale * ???;
    int yPixel = scale * ???;
    for(int y = 0; y < lines; y++)
        for(int x = 0; x < charsPerLine; x++)
        {
            Engine::Entity key = ecsSystem->CreateEntity();
            Engine::Text& text = ecsSystem->AddComponent<Engine::Text>(key);
            text.SetText(std::string("") + (char)('a' + i));
            text.scale = scale;
            text.position = position + glm::vec2(xPixel * x, yPixel * y);

            i++;
        }
}

Keyboard::Keyboard(std::string startText, int charsPerLine, glm::vec2 position, int scale) : Keyboard(position, scale)
{
    text = startText;
    this->charsPerLine = charsPerLine;
}

std::string Keyboard::GetText()
{
    return text;
}

void Keyboard::SetPos(int x, int y)
{
    pos = x + y * charsPerLine;
    pos = std::clamp(pos, 0, 26);
}

void Keyboard::AddPos(int x, int y)
{
    pos += x + y * charsPerLine;
    pos = std::clamp(pos, 0, 26);
}

void Keyboard::EnterChar()
{
    text += (char)('a' + pos);
}

void Keyboard::DeleteChar()
{
    text.pop_back();
}
