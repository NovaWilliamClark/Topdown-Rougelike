#pragma once
#include <map>
#include <SFML/Window/Joystick.hpp>

enum class EInputDirection
{
	UP,
    DOWN,
    RIGHT,
    LEFT,
    NONE,
};

class InputManager {
public:

    // Button checks
    static bool isButtonPressed(unsigned t_joystickID, unsigned t_button);
    static bool isButtonHeld(unsigned t_joystickID, unsigned t_button);

    // Axis checks
    static bool isAxisMoved(unsigned t_joystickID, sf::Joystick::Axis t_axis);
    static float getAxisPosition(unsigned t_joystickID, sf::Joystick::Axis t_axis);

    static EInputDirection getSingleVerticalInput(unsigned t_joystickID);

    // Update button states, and axis positions and connection
    static void update();
private:
    inline static std::map<unsigned int, std::map<unsigned int, bool>> m_currentButtonStates;
    inline static std::map<unsigned int, std::map<unsigned int, bool>> m_previousButtonStates;

    inline static std::map<unsigned int, std::map<sf::Joystick::Axis, float>> m_currentAxisStates;
    inline static std::map<unsigned int, std::map<sf::Joystick::Axis, float>> m_previousAxisStates;

    inline static std::map<unsigned int, bool> m_awaitingReset;
};

