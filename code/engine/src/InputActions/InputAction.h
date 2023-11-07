#pragma once
#include <string>
#include <list>
namespace Engine
{
    class InputSystem;

    class InputAction
    {
        friend class InputSystem;

    protected:
        std::string name;
        std::list<int> keyboardBindings;
        InputSystem* inputSystem;

        explicit InputAction(std::string name);

    public:
        /**
         * Updates the state of the action.
         * @param key The key that changed
         */
        virtual void Update(int key) = 0;
    };

} // Engine
