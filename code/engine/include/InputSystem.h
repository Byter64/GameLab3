#pragma once
#include "GLFW/glfw3.h"
#include "InputAction.h"
#include <list>
#include <memory>
#include <map>

namespace Engine
{

    class InputSystem
    {
        inline static std::list<InputSystem*> activeInputSystems;


        int keyStates[GLFW_KEY_LAST - 1];
        GLFWwindow* window;

        std::list<std::shared_ptr<InputAction>> inputActions;
        std::map<int, std::list<InputAction*>> keyToInputActions;

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


    public:
        explicit InputSystem(GLFWwindow* window);
        ~InputSystem();
        void Add(std::shared_ptr<InputAction> inputAction);
        std::shared_ptr<InputAction> Remove(std::string inputActionName);
        int GetKeyState(int key);
    };

} // Engine
