#include "InputManager.hpp"
#include <ranges>

#define DEAD_ZONE 25.f


bool InputManager::isButtonPressed(const unsigned t_joystickID, const unsigned t_button) {
	return m_currentButtonStates[t_joystickID][t_button] && !m_previousButtonStates[t_joystickID][t_button];
}

bool InputManager::isButtonHeld(const unsigned t_joystickID, const unsigned t_button) {
	return m_currentButtonStates[t_joystickID][t_button];
}

bool InputManager::isAxisMoved(const unsigned t_joystickID, const sf::Joystick::Axis t_axis) {
	return m_currentAxisStates[t_joystickID][t_axis] != m_previousAxisStates[t_joystickID][t_axis];
}
float InputManager::getAxisPosition(const unsigned t_joystickID, const sf::Joystick::Axis t_axis) {
	return m_currentAxisStates[t_joystickID][t_axis];
}

EInputDirection InputManager::getSingleVerticalInput(const unsigned t_joystickID)
{
	const float povY = getAxisPosition(t_joystickID, sf::Joystick::PovY);
	const float stickY = getAxisPosition(t_joystickID, sf::Joystick::Y);

	// If we're awaiting reset, only allow new input if the joystick has returned to near-resting position
	if (m_awaitingReset[t_joystickID]) {
		if (std::abs(povY) < DEAD_ZONE && std::abs(stickY) < DEAD_ZONE) {
			m_awaitingReset[t_joystickID] = false;
		}
		else {
			return EInputDirection::NONE;
		}
	}

	if ((povY > DEAD_ZONE && isAxisMoved(t_joystickID, sf::Joystick::PovY)) ||
		(stickY < -DEAD_ZONE && isAxisMoved(t_joystickID, sf::Joystick::Y))) {
		m_awaitingReset[t_joystickID] = true;
		return EInputDirection::UP;
	}

	if ((povY < -DEAD_ZONE && isAxisMoved(t_joystickID, sf::Joystick::PovY)) ||
		(stickY > DEAD_ZONE && isAxisMoved(t_joystickID, sf::Joystick::Y))) {
		m_awaitingReset[t_joystickID] = true;
		return EInputDirection::DOWN;
	}
	return EInputDirection::NONE;
}

void InputManager::update() {
	// For each joystick and each button, update the previous state with the current state
	// and then update the current state with the new state from the SFML joystick API.
	// Do the same for axis positions.

	for (auto& joystick : m_currentButtonStates) {
		unsigned int joystickID = joystick.first;
		if (sf::Joystick::isConnected(joystickID)) {
			for (const auto& key : joystick.second | std::views::keys) {
				unsigned int buttonID = key;
				m_previousButtonStates[joystickID][buttonID] = m_currentButtonStates[joystickID][buttonID];
				m_currentButtonStates[joystickID][buttonID] = sf::Joystick::isButtonPressed(joystickID, buttonID);
			}

			for (const auto& key : m_currentAxisStates[joystickID] | std::views::keys) {
				sf::Joystick::Axis axisID = key;
					m_previousAxisStates[joystickID][axisID] = m_currentAxisStates[joystickID][axisID];
					m_currentAxisStates[joystickID][axisID] = sf::Joystick::getAxisPosition(joystickID, axisID);
			}
		}
	}
}

