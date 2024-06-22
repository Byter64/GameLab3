#pragma once

namespace Engine
{

    struct GamepadAxis
    {
        GamepadAxis() = default;

        ///
        /// \param gamepadID This is the index of the gamepad you want to reference. The first gamepad has ID 0, the second ID 1, and so on. Internally, the GLFW_JOYSTICK_... will be searched for this gamepad
        /// \param inputID
        /// \param inputType
        GamepadAxis(unsigned char gamepadID, unsigned char inputID);

        unsigned char joystickID;
        unsigned char inputID; //must be an axis

        bool operator ==(GamepadAxis const& other) const;
        bool operator !=(GamepadAxis const& other) const;
        bool operator <(GamepadAxis const& other) const;
    };

} // Engine
