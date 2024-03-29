#pragma once
#include "GLFW/glfw3.h"
#include "InputAction.h"
#include <list>
#include <memory>
#include <map>
#include "InputSystem/GamepadInputID.h"

namespace Engine
{

    class InputSystem
    {
        inline static InputSystem* instance;


        int keyStates[GLFW_KEY_LAST - 1];
        GLFWgamepadstate gamePadStates[GLFW_JOYSTICK_LAST + 1]{};
        GLFWwindow* window;

        std::list<std::shared_ptr<InputAction>> inputActions;
        std::map<int, std::list<InputAction*>> keyToInputActions;
        std::map<GamepadInputID, std::list<InputAction*>> inputToInputActions;
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


    public:
        explicit InputSystem(GLFWwindow* window);
        ~InputSystem();
        void Update(unsigned char maxJoysticks = 2);
        void Add(std::shared_ptr<InputAction> inputAction);
        std::shared_ptr<InputAction> Remove(std::shared_ptr<InputAction> inputAction);
        std::shared_ptr<InputAction> Remove(std::string inputActionName);
        int GetKeyState(int key);
        int GetButtonState(unsigned char joystick, unsigned char button);
        float GetAxisState(unsigned char joystick, unsigned char axis);
    };

} // Engine
