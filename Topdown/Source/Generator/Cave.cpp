#include "Cave.hpp"
#include "../Map/Map.hpp"
#include "../Utility/Rng.hpp"
#include "../Utility/Utility.hpp"

Cave::Cave()
{
	floor = ETile::DIRT;
	wall = ETile::CAVE_WALL;
	corridor = ETile::DIRT;
	water = ETile::LAVA;
}

void Cave::removeWallChunks(int t_minSize)
{
	std::swap(floor, wall);

	removeRegions(0, t_minSize);

	std::swap(floor, wall);
}

void BigCave::onGenerate()
{
	setName("A big cave");

	fill(40);

	for (int i = 0; i < 4; ++i)
		generation(5, 2);

	for (int i = 0; i < 3; ++i)
		generation(5);

	removeRegions();
	removeWallChunks();
}

void ConnectedCaves::onGenerate()
{
	setName("Connected caves");

	fill(55);

	for (int i = 0; i < 4; ++i)
		generation(5, 2);

	for (int i = 0; i < 3; ++i)
		generation(5);

	connectRegions(10, EPathType::CORRIDOR);
	removeWallChunks();
}

void CavesAndRooms::onGenerate()
{
	setName("Caves and rooms");

	fill(55);
	
	for (int i = 0; i < 4; ++i)
		generation(5, 2);

	for (int i = 0; i < 3; ++i)
		generation(5);

	for (int i = 0; i < 1000; ++i)
	{
		Room room;
		room.width = odd(rng->rollDice(3, 3));
		room.height = odd(rng->rollDice(3, 3));
		room.left = odd(rng->getInt(width - room.width - 1));
		room.top = odd(rng->getInt(height - room.height - 1));

		if (canCarve(room))
			carveRoom(room);
	}
	
	connectRegions(10, EPathType::CORRIDOR);
	removeWallChunks();
}

// TODO: Clean up code
void CavesAndLava::onGenerate()
{
	setName("Caves and lava");

	fill(50);

	for (int i = 0; i < 4; ++i)
		generation(5, 2);

	for (int i = 0; i < 3; ++i)
		generation(5);

	std::vector<ETile> caves = map->copy();


	std::swap(floor, water);

	fill(55);

	for (int i = 0; i < 4; ++i)
		generation(5, 2);

	for (int i = 0; i < 3; ++i)
		generation(5);

	removeRegions(75, 10);

	int numRivers = 2;

	if (rng->getInt(3) == 0)
		numRivers += 1;

	for (int i = 0; i < numRivers; ++i)
	{
		Point from(rng->getInt(width), rng->getInt(height));
		Point to(rng->getInt(width), rng->getInt(height));

		extendLine(from, to);
		carveWindingRoad(from, to, 10);
	}

	removeRegions(0, 80); // REMOVE: just for test

	std::swap(floor, water);

	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) != wall)
				continue;

			if (caves[x + y * width] == floor)
			{
				map->setTile(x, y, floor);
				continue;
			}

			int waterWeight = 0;

			for (int dy = -2; dy <= 2; ++dy)
				for (int dx = -2; dx <= 2; ++dx)
				{
					int ax = std::abs(dx);
					int ay = std::abs(dy);

					if (ax == 2 && ay == 2)
						continue;

					if (!map->isInBounds(x + dx, y + dy))
						continue;

					if (map->getTile(x + dx, y + dy) == water)
						waterWeight += 4 / (ax + ay); // ax + ay = distance
				}

			if (waterWeight >= 4)
				map->setTile(x, y, floor);

			else if (waterWeight > 0 && rng->getInt(9) == 0)
				map->setTile(x, y, floor);
		}

	// HACK: Erode sharp floor corners near water
	for (int i = 0; i < 5; ++i)
		erodeTiles(floor, water, 5);

	for (int i = 0; i < 5; ++i)
		erodeTiles(wall, floor, 5);

	connectRegions(10);
	constructBridges(5); // TODO: if the length of a bridge is 1, put a floor tile instead a bridge tile

	removeWallChunks();
}
