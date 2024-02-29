#pragma once
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "../glText/gltext.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>

namespace Engine
{

    struct Text
    {
        float scale;
        glm::vec2 position{};

    private:
        std::unique_ptr<GLTtext> text;
        std::string string;
    public:
        Text();
        explicit Text(std::string string);
        Text (Text& other);

        Text& operator=(Text& other);

        void SetText(std::string& string);
        std::string& GetText();
    };

} // Engine
