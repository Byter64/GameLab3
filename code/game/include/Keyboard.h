#pragma once
#include "ECSExtension.h"

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

    std::shared_ptr<Engine::InputActionButton> up;
    std::shared_ptr<Engine::InputActionButton> down;
    std::shared_ptr<Engine::InputActionButton> left;
    std::shared_ptr<Engine::InputActionButton> right;
    std::shared_ptr<Engine::InputActionButton> enter;
    std::shared_ptr<Engine::InputActionButton> del;

    static void OnUp(void* o);
    static void OnDown(void* o);
    static void OnLeft(void* o);
    static void OnRight(void* o);
    static void OnEnter(void* o);
    static void OnDelete(void* o);
public:
    explicit Keyboard(glm::vec2 position, int scale = 3, int charsPerLine = 7);
    explicit Keyboard(std::string startText, int charsPerLine, glm::vec2 position, int scale = 3);
    ~Keyboard();

    std::string GetText();
    void SetPos(int x, int y);
    void AddPos(int x, int y);
    void EnterChar();
    //Always deletes the lastly entered char
    void DeleteChar();

    //Sets the given action as up. This action will be cleaned up on ~Keyboard()
    void SetUpAction(std::shared_ptr<Engine::InputActionButton> action);
    //Sets the given action as down. This action will be cleaned up on ~Keyboard()
    void SetDownAction(std::shared_ptr<Engine::InputActionButton> action);
    //Sets the given action as left. This action will be cleaned up on ~Keyboard()
    void SetLeftAction(std::shared_ptr<Engine::InputActionButton> action);
    //Sets the given action as right. This action will be cleaned up on ~Keyboard()
    void SetRightAction(std::shared_ptr<Engine::InputActionButton> action);
    //Sets the given action as enter. This action will be cleaned up on ~Keyboard()
    void SetEnterAction(std::shared_ptr<Engine::InputActionButton> action);
    //Sets the given action as delete. This action will be cleaned up on ~Keyboard()
    void SetDeleteAction(std::shared_ptr<Engine::InputActionButton> action);

};
