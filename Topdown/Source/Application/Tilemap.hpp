#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

class Map;

class Tilemap final : public sf::Drawable, public sf::Transformable
{
public:
	Tilemap(const sf::Texture& t_tileset, const sf::Vector2i& t_tileSize);

	void load(const Map& t_map);

private:
	void append(int t_x, int t_y, int t_tileNumber);

	int getTileNumber(const Map& t_map, int t_x, int t_y) const;

	void draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const override;

private:
	const sf::Texture& m_tileset;
	const sf::Vector2i m_tileSize;
	std::vector<sf::Vertex> m_vertices;
};
