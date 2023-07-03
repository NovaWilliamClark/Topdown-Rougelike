#include "Map.hpp"

#include <algorithm> // fill
#include <utility> // move

Map::Map(const int t_width, const int t_height)
	: width(t_width)
	, height(t_height)
	, m_tiles(t_width * t_height, ETile::UNUSED)
{
}

bool Map::isInBounds(const int t_x, const int t_y) const
{
	return t_x >= 0 && t_x < width && t_y >= 0 && t_y < height;
}

bool Map::isInBounds(const sf::Vector2i& t_pos) const
{
	return isInBounds(t_pos.x, t_pos.y);
}

bool Map::canCollide(const int t_x, const int t_y) const
{
	switch (getTile(t_x, t_y))
	{
	case ETile::UNUSED:
	case ETile::WALL:
	case ETile::CAVE_WALL:
	case ETile::LAVA: return true;
	case ETile::FLOOR:
	case ETile::DIRT:
	case ETile::CORRIDOR:
	case ETile::DOWN_STAIRS:
	case ETile::BRIDGE: break;
	}

	return false;
}

void Map::setTile(const int t_x, const int t_y, const ETile t_tile)
{
	m_tiles[t_x + t_y * width] = t_tile;
}

void Map::setTile(const sf::Vector2i& t_pos, const ETile t_tile)
{
	setTile(t_pos.x, t_pos.y, t_tile);
}

ETile Map::getTile(const int t_x, const int t_y) const
{
	return m_tiles[t_x + t_y * width];
}

ETile Map::getTile(const sf::Vector2i& t_pos) const
{
	return getTile(t_pos.x, t_pos.y);
}

void Map::fill(const ETile t_tile)
{
	std::ranges::fill(m_tiles, t_tile);
}

void Map::move(std::vector<ETile>&& t_tiles)
{
	m_tiles = std::move(t_tiles);
}

std::vector<ETile> Map::copy()
{
	return m_tiles;
}
