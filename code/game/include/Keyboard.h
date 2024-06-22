#pragma once
#include "Engine.h"

class Keyboard
{
    std::string text = "";
    int scale = 1;
    int charsPerLine = 7;
    int xPixel;
    int yPixel;
    glm::vec2 position;

    std::map<char, Engine::Entity> keys;

    int selectorPos = 0;
    Engine::Entity selector;

    std::shared_ptr<Engine::InputActionVec2> movement;
    std::shared_ptr<Engine::InputActionButton> enter;
    std::shared_ptr<Engine::InputActionButton> del;

    static void OnMovementStart(void* o, glm::vec2 input);
    static void OnEnter(void* o);
    static void OnDelete(void* o);
public:
    int maxChars = 16;

    explicit Keyboard(glm::vec2 position, int scale = 3, int charsPerLine = 7);
    explicit Keyboard(std::string startText, int charsPerLine, glm::vec2 position, int scale = 3);
    ~Keyboard();

    std::string GetText();
    void SetText(std::string text);
    void SetPos(int x, int y);
    void AddPos(int x, int y);
    void EnterChar();
    //Always deletes the lastly entered char
    void DeleteChar();

    //Sets the given action as movement. This action will be cleaned up on ~Keyboard()
    void SetMovementAction(std::shared_ptr<Engine::InputActionVec2> action);
    //Sets the given action as enter. This action will be cleaned up on ~Keyboard()
    void SetEnterAction(std::shared_ptr<Engine::InputActionButton> action);
    //Sets the given action as delete. This action will be cleaned up on ~Keyboard()
    void SetDeleteAction(std::shared_ptr<Engine::InputActionButton> action);

};
