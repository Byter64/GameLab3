#include "ECS/Components/Text.h"

namespace Engine
{
    Text::Text()
    {
        text.reset(gltCreateText());
        scale = 1;
        position = {0,0};
        std::string empty = "";
        SetText(empty);
    }

    Text::Text(std::string string)
    {
        text.reset(gltCreateText());
        scale = 1;
        position = {0,0};

        SetText(string);
    }

    void Text::SetText(std::string string)
    {
        this->string = std::move(string);
        gltSetText(text.get(), this->string.c_str());
    }

    std::string &Text::GetText()
    {
        return string;
    }

    Text::Text(Text &other)
    {
        text.reset(gltCreateText());
        scale = other.scale;
        position = other.position;

        SetText(other.string);
    }

    Text &Text::operator=(Text &other)
    {
        scale = other.scale;
        position = other.position;
        SetText(other.string);

        return *this;
    }
} // Engine