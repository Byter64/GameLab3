#pragma once
#include <string>
#include <vector>

namespace Engine
{

    class InputAction
    {
        std::string name;
        std::vector<int> keyboardBindings;

        InputAction();
    public:
        const std::vector<int>& GetKeyboardBindings();
    };

} // Engine
