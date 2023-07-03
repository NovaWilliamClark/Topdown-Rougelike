#pragma once

#include "Tilemap.hpp"
#include "../Map/Map.hpp"
#include "../Generator/Generator.hpp"
#include "../Utility/Rng.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory>

class Application
{
public:
	Application();

	void run();

private:
	void processInput();
	void update(sf::Time t_dt);
	void render();

	// Move to gameplay scene
	// void selectRandomGenerator();
	// void generateMap();
	//
	// void registerGenerators();

private:
	sf::RenderWindow m_window;

	//sf::Texture m_texture;
	//sf::Font m_font;
	//
	//sf::Text m_text;
	//sf::Text m_textMousePos;
	//
	//Map m_map;
	//Rng m_rng;
	//
	//std::unique_ptr<Tilemap> m_tilemap;
	//
	//std::vector<std::unique_ptr<Generator>> m_generators;
	//std::size_t m_currentGenerator = 0;
};
