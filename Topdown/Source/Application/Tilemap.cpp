#include "Tilemap.hpp"
#include "../Map/Map.hpp"
#include "../Utility/Utility.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Tilemap::Tilemap(const sf::Texture& t_tileset, const sf::Vector2i& t_tileSize)
	: m_tileset(t_tileset)
	, m_tileSize(t_tileSize)
{
}

void Tilemap::load(const Map& t_map)
{
	m_vertices.clear();
	m_vertices.resize(t_map.width * t_map.height * 4);
	// vertices.shrink_to_fit();

	for (int y = 0; y < t_map.height; ++y)
		for (int x = 0; x < t_map.width; ++x)
		{
			const int tileNumber = getTileNumber(t_map, x, y);

			if (tileNumber < 0)
				continue;

			const int tu = tileNumber % (m_tileset.getSize().x / m_tileSize.x);
			const int tv = tileNumber / (m_tileset.getSize().x / m_tileSize.x);

			sf::Vertex* quad = &m_vertices[(x + y * t_map.width) * 4];

			quad[0].position = sf::Vector2f((x + 0.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y);
			quad[1].position = sf::Vector2f((x + 1.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y);
			quad[2].position = sf::Vector2f((x + 1.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y);
			quad[3].position = sf::Vector2f((x + 0.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y);

			quad[0].texCoords = sf::Vector2f((tu + 0.f) * m_tileSize.x, (tv + 0.f) * m_tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1.f) * m_tileSize.x, (tv + 0.f) * m_tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1.f) * m_tileSize.x, (tv + 1.f) * m_tileSize.y);
			quad[3].texCoords = sf::Vector2f((tu + 0.f) * m_tileSize.x, (tv + 1.f) * m_tileSize.y);
		}
}

void Tilemap::append(const int t_x, const int t_y, const int t_tileNumber)
{
	const int tu = t_tileNumber % (m_tileset.getSize().x / m_tileSize.x);
	const int tv = t_tileNumber / (m_tileset.getSize().x / m_tileSize.x);

	m_vertices.emplace_back(sf::Vector2f((t_x + 0.f) * m_tileSize.x, (t_y + 0.f) * m_tileSize.y), sf::Vector2f((tu + 0.f) * m_tileSize.x, (tv + 0.f) * m_tileSize.y));
	m_vertices.emplace_back(sf::Vector2f((t_x + 1.f) * m_tileSize.x, (t_y + 0.f) * m_tileSize.y), sf::Vector2f((tu + 1.f) * m_tileSize.x, (tv + 0.f) * m_tileSize.y));
	m_vertices.emplace_back(sf::Vector2f((t_x + 1.f) * m_tileSize.x, (t_y + 1.f) * m_tileSize.y), sf::Vector2f((tu + 1.f) * m_tileSize.x, (tv + 1.f) * m_tileSize.y));
	m_vertices.emplace_back(sf::Vector2f((t_x + 0.f) * m_tileSize.x, (t_y + 1.f) * m_tileSize.y), sf::Vector2f((tu + 0.f) * m_tileSize.x, (tv + 1.f) * m_tileSize.y));
}

int Tilemap::getTileNumber(const Map& t_map, const int t_x, const int t_y) const
{

	int tileNumber = -1;

	switch (t_map.getTile(t_x, t_y))
	{
	case ETile::UNUSED:
		tileNumber = 0x60;
		break;

	case ETile::FLOOR:
		tileNumber = 0x62;
		break;

	case ETile::CORRIDOR:
		tileNumber = 0x63;
		break;

	case ETile::WALL:
		if (t_y == t_map.height - 1 || t_map.getTile(t_x, t_y + 1) != ETile::WALL)
		{
			tileNumber = 0x0;
		}

		else
			tileNumber = 0x6;

		if (randomInt(6) == 0)
		{
			if (randomInt(6) > 0)
				tileNumber += randomInt(1, 3);

			else
			{
				// tileNumber += randomInt(4, 5);

				if (tileNumber == 0x0)
					tileNumber = 0x5;
				else if (tileNumber == 0x6)
					tileNumber = 0xa;
			}
		}
		break;

	case ETile::DIRT:
		if (randomInt(3) > 0)
			tileNumber = 0x50;
		else if (randomInt(3) > 0)
			tileNumber = 0x50 + randomInt(1, 3);
		else
			tileNumber = 0x50 + randomInt(4, 5);
		break;

	case ETile::CAVE_WALL:
		if (t_y == t_map.height - 1 || t_map.getTile(t_x, t_y + 1) != ETile::CAVE_WALL)
		{
			tileNumber = 0x10;
		}

		else
			tileNumber = 0x16;

		if (randomInt(6) == 0)
		{
			if (randomInt(6) > 0)
				tileNumber += randomInt(1, 3);
			else
				tileNumber += randomInt(4, 5);
		}
		break;

	case ETile::LAVA:
		if (t_y > 0 && t_map.getTile(t_x, t_y - 1) != ETile::LAVA)
			tileNumber = 0x98 + randomInt(2);
		else
			tileNumber = 0x9a + randomInt(2);
		break;

	case ETile::BRIDGE:
		tileNumber = 0xbc + randomInt(3);
		break;
	case ETile::DOWN_STAIRS:
		tileNumber = 0xc;
		break;
	}

	return tileNumber;
}

void Tilemap::draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const
{
	t_states.transform *= getTransform();
	t_states.texture = &m_tileset;
	t_target.draw(m_vertices.data(), m_vertices.size(), sf::Quads, t_states);
}
