#pragma once

#include "Tile.hpp"

#include <SFML/System/Vector2.hpp>

#include <vector>

class Map
{
public:
	Map(int t_width, int t_height);

	[[nodiscard]] bool isInBounds(int t_x, int t_y) const;
	[[nodiscard]] bool isInBounds(const sf::Vector2i& t_pos) const;
	[[nodiscard]] bool canCollide(int t_x, int t_y) const;

	void setTile(int t_x, int t_y, ETile t_tile);
	void setTile(const sf::Vector2i& t_pos, ETile t_tile);

	[[nodiscard]] ETile getTile(int t_x, int t_y) const;
	[[nodiscard]] ETile getTile(const sf::Vector2i& t_pos) const;

	void fill(ETile t_tile);

	void move(std::vector<ETile>&& t_tiles);
	std::vector<ETile> copy();

public:
	const int width, height;

private:
	std::vector<ETile> m_tiles;
};
