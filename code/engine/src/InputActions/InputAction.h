#pragma once
#include <string>
#include <list>
#include "../InputSystem.h"
namespace Engine
{

    class InputAction
    {
        friend class InputSystem;

    protected:
        std::string name;
        std::list<int> keyboardBindings;
        InputSystem* inputSystem;

        explicit InputAction(std::string name);

    public:
        const std::list<int>& GetKeyboardBindings();
        /**
         * Updates the state of the action.
         * @param key The key that changed
         */
        virtual void UpdateAction(int key) = 0;
    };

} // Engine
