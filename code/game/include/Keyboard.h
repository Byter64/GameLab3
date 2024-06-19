#pragma once
#include "ECSExtension.h"

class Keyboard
{
    int pos = 0;
    std::string text = "";
    int scale = 1;
    int charsPerLine = 7;

    std::map<char, Engine::Entity> keys;
public:
    explicit Keyboard(glm::vec2 position, int scale = 3);
    explicit Keyboard(std::string startText, int charsPerLine, glm::vec2 position, int scale = 3);

    std::string GetText();
    void SetPos(int x, int y);
    void AddPos(int x, int y);
    void EnterChar();

    //Always deletes lastly entered char
    void DeleteChar();
};
