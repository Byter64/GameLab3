#include "InputAction.h"
#include <string>
#include <list>

namespace Engine
{
    InputAction::InputAction(std::string name) : name(std::move(name))
    {

    }

    const std::list<int> &InputAction::GetKeyboardBindings()
    {
        return keyboardBindings;
    }
} // Engine