#include "Dungeon.hpp"
#include "../Map/Map.hpp"
#include "../Utility/Rng.hpp"
#include "../Utility/Utility.hpp"
#include "../Utility/Direction.hpp"

#include <queue>


void Dungeon::removeCorridors() const
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			if (map->getTile(x, y) == corridor)
				map->setTile(x, y, floor);
		}
}

void ClassicDungeon::onGenerate()
{
	// NOTE: Original Rogue-like dungeon

	setName("Classic dungeon");

	map->fill(wall);

	std::vector<Point> points;

	for (int i = 0; i < 15; ++i)
	{
		int x = rng->getInt(1, width - 2);
		int y = rng->getInt(1, height - 2);

		points.emplace_back(x, y);
	}

	for (int i = 0; i < 5; ++i)
		relaxation(points);

	for (auto it = points.begin(); it != points.end(); )
	{
		auto& point = *it;

		Room room;
		room.width = odd(rng->rollDice(4, 3));
		room.height = odd(rng->rollDice(3, 3));
		room.left = odd(std::min(std::max(1, point.x - room.width / 2), width - room.width - 2));
		room.top = odd(std::min(std::max(1, point.y - room.height / 2), height - room.height - 2));
		
		point.x = odd(room.left + room.width / 2);
		point.y = odd(room.top + room.height / 2);

		if (canCarve(room))
		{
			carveRoom(room);
			++it;
		}

		else
			it = points.erase(it);
	}

	connectPoints(points, EPathType::CORRIDOR);
	removeWalls();

	removeCorridors();
}
