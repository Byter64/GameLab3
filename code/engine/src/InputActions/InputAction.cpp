#include "InputAction.h"

namespace Engine
{
    InputAction::InputAction()
    {

    }

    const std::vector<int> &InputAction::GetKeyboardBindings()
    {
        return keyboardBindings;
    }
} // Engine