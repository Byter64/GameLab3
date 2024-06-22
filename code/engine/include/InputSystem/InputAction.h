#pragma once
#include <string>
#include <list>
#include "GamepadButton.h"
#include "InputSystem/GamepadAxis.h"
namespace Engine
{
    class InputSystem;

    class InputAction
    {
        friend class InputSystem;

    protected:
        std::string name;
        std::list<int> keyboardBindings;
        std::list<GamepadButton> gamepadButtons;
        std::list<GamepadAxis> gamepadAxes;
        InputSystem* inputSystem;

        explicit InputAction(std::string name);
        /**
         * Will be called by the InputSystem to update the state of this action.
         * @param key The key that changed
         */
        virtual void Update(int key) = 0;

        /**
        * Will be called by the InputSystem to update the state of this action.
        * @param input The input that changed
        */
        virtual void Update(GamepadButton& input) = 0;

        /**
        * Will be called by the InputSystem to update the state of this action.
        * @param input The input that changed
        */
        virtual void Update(GamepadAxis& input) = 0;
    };

} // Engine
