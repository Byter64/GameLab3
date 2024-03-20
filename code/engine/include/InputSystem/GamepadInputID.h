#pragma once

namespace Engine
{

    struct GamepadInputID
    {
        enum InputType : unsigned char
        {
            Button,
            Axis
        };

        unsigned char joystickID;
        unsigned char inputID; //either button ID or axis ID
        InputType inputType;

        bool operator ==(GamepadInputID const& other) const;
        bool operator !=(GamepadInputID const& other) const;
        bool operator <(GamepadInputID const& other) const;
    };

} // Engine
