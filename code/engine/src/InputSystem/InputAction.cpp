#include "InputSystem/InputAction.h"
#include <string>
#include <list>

namespace Engine
{
    InputAction::InputAction(std::string name) : name(std::move(name))
    {

    }

    void InputAction::Update(int key)
    {

    }
} // Engine