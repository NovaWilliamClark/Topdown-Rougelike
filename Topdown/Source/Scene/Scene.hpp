#pragma once

#pragma once
#include <SFML/Graphics.hpp>

class Scene {
public:
	virtual ~Scene() = default;

	virtual void update(float t_deltaTime) = 0;
	virtual void draw(sf::RenderWindow& t_target) = 0;
	virtual void handleInput(const sf::Event& t_event) = 0;
};