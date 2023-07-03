#include "Generator.hpp"
#include "../Map/Map.hpp"
#include "../Utility/Rng.hpp"
#include "../Utility/Direction.hpp"
#include "../Utility/Utility.hpp"

#include <queue>
#include <list>
#include <utility> // swap
#include <algorithm> // find
#include <cstdlib> // abs
#include <climits> // INT_MAX
#include <cassert>

const std::string& Generator::getName() const
{
	return m_name;
}

void Generator::generate(Map& t_map, Rng& t_rng)
{
	this->map = &t_map;
	this->rng = &t_rng;

	width = t_map.width;
	height = t_map.height;

	onGenerate();
	placeStaircase();
}

void Generator::setName(const std::string& t_name)
{
	this->m_name = t_name;
}

void Generator::generation(int t_r1Cutoff)
{
	std::vector<ETile> tiles(width * height, wall);

	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			int r1 = countTiles(wall, x, y);

			if (r1 >= t_r1Cutoff)
				tiles[x + y * width] = wall;
			else
				tiles[x + y * width] = floor;
		}

	map->move(std::move(tiles));
}

void Generator::generation(int t_r1Cutoff, int t_r2Cutoff)
{
	std::vector<ETile> tiles(width * height, wall);

	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			int r1 = 0;
			int r2 = 0;

			for (int dy = -2; dy <= 2; ++dy)
				for (int dx = -2; dx <= 2; ++dx)
				{
					int ax = std::abs(dx);
					int ay = std::abs(dy);

					if (ax == 2 && ay == 2)
						continue;

					if (map->isInBounds(x + dx, y + dy) &&
						map->getTile(x + dx, y + dy) == wall)
					{
						if (ax <= 1 && ay <= 1)
							r1 += 1;

						r2 += 1;
					}
				}

			if (r1 >= t_r1Cutoff || r2 <= t_r2Cutoff)
				tiles[x + y * width] = wall;
			else
				tiles[x + y * width] = floor;
		}

	map->move(std::move(tiles));
}

void Generator::removeRegions(int t_removeProb, int t_minSize)
{
	int currentRegion = -1;
	std::vector<int> regions(width * height, currentRegion);
	std::vector<int> regionsSizes;

	// Non-recursive flood fill
	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) == wall || regions[x + y * width] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				Point pos = queue.front();
				queue.pop();

				if (map->getTile(pos) == wall || regions[pos.x + pos.y * width] >= 0)
					continue;

				regions[pos.x + pos.y * width] = currentRegion;
				regionsSizes[currentRegion] += 1;

				for (const auto& dir : Direction::Cardinal)
				{
					if (!map->isInBounds(pos + dir))
						continue;

					queue.emplace(pos + dir);
				}
			}
		}

	// Find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 1, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsSizes[i] > regionsSizes[biggestRegion])
			biggestRegion = i;

		if (rng->getInt(100) < t_removeProb || regionsSizes[i] < t_minSize)
			regionsForRemoval[i] = true;
	}

	// Always do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	// Remove marked regions
	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) == wall)
				continue;

			int i = regions[x + y * width];

			if (regionsForRemoval[i])
				map->setTile(x, y, wall);
		}
}

void Generator::connectRegions(int t_minSize, EPathType t_type, bool t_allowDiagonalSteps)
{
	int currentRegion = -1;
	std::vector<int> regions(width * height, currentRegion);
	std::vector<int> regionsSizes;
	std::vector<std::vector<Point>> connectors;

	// Non-recursive flood fill
	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) == wall || regions[x + y * width] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);
			connectors.emplace_back();

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				Point pos = queue.front();
				queue.pop();

				if (map->getTile(pos) == wall || regions[pos.x + pos.y * width] >= 0)
					continue;

				regions[pos.x + pos.y * width] = currentRegion;
				regionsSizes[currentRegion] += 1;

				bool isConnector = false;

				for (const auto& dir : Direction::Cardinal)
				{
					if (!map->isInBounds(pos + dir))
						continue;

					queue.emplace(pos + dir);

					if (map->getTile(pos + dir) == wall)
						isConnector = true;
				}

				if (isConnector)
					connectors[currentRegion].emplace_back(pos);
			}
		}

	// Find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 1, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsSizes[i] > regionsSizes[biggestRegion])
			biggestRegion = i;

		if (regionsSizes[i] < t_minSize)
			regionsForRemoval[i] = true;
	}

	// Always do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	// Remove marked regions
	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) == wall)
				continue;

			int i = regions[x + y * width];

			if (regionsForRemoval[i])
				map->setTile(x, y, wall);
		}

	std::vector<int> connected;
	std::list<int> unconnected;

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsForRemoval[i])
			continue;

		if (i == biggestRegion)
			connected.emplace_back(i);
		else
			unconnected.emplace_back(i);
	}

	while (!unconnected.empty())
	{
		std::vector<std::pair<Point, Point>> bestConnectors; // from, to
		int bestDistance = INT_MAX;

		for (int from : connected)
		{
			for (const auto& connectorFrom : connectors[from])
			{
				for (int to : unconnected)
				{
					for (const auto& connectorTo : connectors[to])
					{
						Point delta = connectorTo - connectorFrom;
						int distance = INT_MAX;

						switch (t_type)
						{
						case EPathType::STRAIGHT:
							distance = lengthSquared(delta);
							break;
						
						case EPathType::CORRIDOR:
						case EPathType::WINDING_ROAD:
							if (t_allowDiagonalSteps)
								distance = std::max(std::abs(delta.x), std::abs(delta.y));
							else
								distance = std::abs(delta.x) + std::abs(delta.y);
							break;
						}

						if (distance < bestDistance)
						{
							bestConnectors.clear();
							bestConnectors.emplace_back(connectorFrom, connectorTo);
							bestDistance = distance;
						}

						else if (distance == bestDistance)
							bestConnectors.emplace_back(connectorFrom, connectorTo);
					}
				}
			}
		}

		assert(!bestConnectors.empty());

		auto bestConnector = rng->getOne(bestConnectors);
		int bestToIndex = regions[bestConnector.second.x + bestConnector.second.y * width];

		switch (t_type)
		{
		case EPathType::STRAIGHT: carvePath(bestConnector.first, bestConnector.second); break;
		case EPathType::CORRIDOR: carveCorridor(bestConnector.first, bestConnector.second); break;
		case EPathType::WINDING_ROAD: carveWindingRoad(bestConnector.first, bestConnector.second); break;
		}

		connected.emplace_back(bestToIndex);
		unconnected.remove(bestToIndex);
	}
}

void Generator::constructBridges(int t_minSize)
{
	struct Connector
	{
		Connector(const Point& begin, const Direction& dir)
			: begin(begin), dir(dir)
		{
		}

		Point begin;
		Direction dir;
		int length = 0;
	};

	int currentRegion = -1;
	std::vector<int> regions(width * height, currentRegion);
	std::vector<int> regionsSizes;
	std::vector<std::vector<Connector>> connectors;

	// Non-recursive flood fill
	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) != floor || regions[x + y * width] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);
			connectors.emplace_back();

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				Point pos = queue.front();
				queue.pop();

				if (map->getTile(pos) != floor || regions[pos.x + pos.y * width] >= 0)
					continue;

				regions[pos.x + pos.y * width] = currentRegion;
				regionsSizes[currentRegion] += 1;

				for (const auto& dir : Direction::Cardinal)
				{
					queue.emplace(pos + dir);

					if (map->getTile(pos + dir) != floor)
						connectors[currentRegion].emplace_back(pos, dir);
				}
			}
		}

	// Find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 1, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (connectors[i].size() > connectors[biggestRegion].size())
			biggestRegion = i;

		if (regionsSizes[i] < t_minSize)
			regionsForRemoval[i] = true;
	}

	// Always do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	std::vector<int> connected;
	std::list<int> unconnected;

	// Remove marked regions
	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) != floor)
				continue;

			int i = regions[x + y * width];

			if (regionsForRemoval[i])
				map->setTile(x, y, water);
		}

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsForRemoval[i])
			continue;

		if (i == biggestRegion)
			connected.emplace_back(i);
		else
			unconnected.emplace_back(i);
	}

	while (!unconnected.empty())
	{
		std::vector<Connector*> bestConnectors;
		int bestDistance = INT_MAX;

		for (int from : connected)
		{
			for (auto& connector : connectors[from])
			{
				if (connector.length < 0)
					continue;

				Point pos = connector.begin;
				connector.length = 0;

				while (true)
				{
					pos += connector.dir;
					connector.length += 1;

					if (!map->isInBounds(pos))
					{
						connector.length = -1; // disable the connector
						break;
					}

					int to = regions[pos.x + pos.y * width];

					if (to < 0)
						continue;

					auto found = std::find(unconnected.begin(), unconnected.end(), to);

					if (found != unconnected.end())
					{
						if (connector.length < bestDistance)
						{
							bestConnectors.clear();
							bestConnectors.emplace_back(&connector);
							bestDistance = connector.length;
						}

						else if (connector.length == bestDistance)
							bestConnectors.emplace_back(&connector);
					}

					else
						connector.length = -1;

					break;
				}
			}
		}

		if (bestConnectors.empty())
		{
			// NOTE: This function construct only straight bridges.
			//       So, it can't connect between diagonally separated areas.
			//       This is not a problem in most cases, but it can potentially cause bugs.

			std::vector<bool> regionsForRemoval(currentRegion + 1, false);

			for (int i : unconnected)
				regionsForRemoval[i] = true;

			for (int y = 1; y < height - 1; ++y)
				for (int x = 1; x < width - 1; ++x)
				{
					if (map->getTile(x, y) == wall)
						continue;

					int i = regions[x + y * width];

					if (regionsForRemoval[i])
						map->setTile(x, y, wall);
				}

			break;
		}

		Connector* bestFrom = rng->getOne(bestConnectors);
		Point bestToPos = bestFrom->begin + bestFrom->dir * bestFrom->length;
		int bestToIndex = regions[bestToPos.x + bestToPos.y * width];

		for (int i = 1; i < bestFrom->length; ++i)
		{
			Point pos = bestFrom->begin + bestFrom->dir * i;

			if (map->getTile(pos) == water)
				map->setTile(pos, bridge);
			else
				map->setTile(pos, corridor);

			regions[pos.x + pos.y * width] = bestToIndex;

			// Add new connectors from the constructed bridges/corridors
			for (const auto& dir : Direction::Cardinal)
			{
				if (map->getTile(pos + dir) != floor)
					connectors[bestToIndex].emplace_back(pos, dir);
			}
		}

		connected.emplace_back(bestToIndex);
		unconnected.remove(bestToIndex);
	}
}

void Generator::relaxation(std::vector<Point>& t_points)
{
	std::vector<std::pair<Point, int>> regions;

	for (const auto& point : t_points)
		regions.emplace_back(point, 1);

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			Point pos(x, y);
			int nearest = -1;
			int nearestDistance = INT_MAX;

			for (std::size_t i = 0; i < t_points.size(); ++i)
			{
				int distance = lengthSquared(t_points[i] - pos);

				if (distance < nearestDistance)
				{
					nearest = i;
					nearestDistance = distance;
				}
			}

			regions[nearest].first += pos;
			regions[nearest].second += 1;
		}

	for (std::size_t i = 0; i < t_points.size(); ++i)
		t_points[i] = regions[i].first / regions[i].second;
}

void Generator::connectPoints(std::vector<Point>& t_points, EPathType t_type)
{
	std::vector<Point> connected;

	connected.emplace_back(t_points.back());
	t_points.pop_back();

	while (!t_points.empty())
	{
		Point bestFrom;
		int bestToIndex = -1;
		int bestDistance = INT_MAX;

		for (const auto& from : connected)
		{
			for (std::size_t i = 0; i < t_points.size(); ++i)
			{
				int distance = lengthSquared(t_points[i] - from);

				if (distance < bestDistance)
				{
					bestFrom = from;
					bestToIndex = i;
					bestDistance = distance;
				}
			}
		}

		Point to = t_points[bestToIndex];

		switch (t_type)
		{
		case EPathType::STRAIGHT: carvePath(bestFrom, to); break;
		case EPathType::CORRIDOR: carveCorridor(bestFrom, to); break;
		case EPathType::WINDING_ROAD: carveWindingRoad(bestFrom, to); break;
		}

		connected.emplace_back(to);
		t_points.erase(t_points.begin() + bestToIndex);
	}
}

void Generator::fill(int t_wallProb)
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
				map->setTile(x, y, wall);
			else if (rng->getInt(100) < t_wallProb)
				map->setTile(x, y, wall);
			else
				map->setTile(x, y, floor);
		}
}

bool Generator::canCarve(const Room& t_room) const
{
	for (int y = t_room.top - 1; y < t_room.top + t_room.height + 1; ++y)
		for (int x = t_room.left - 1; x < t_room.left + t_room.width + 1; ++x)
		{
			if (map->getTile(x, y) != wall)
				return false;
		}

	return true;
}

void Generator::carveRoom(const Room& t_room)
{
	for (int y = t_room.top; y < t_room.top + t_room.height; ++y)
		for (int x = t_room.left; x < t_room.left + t_room.width; ++x)
			map->setTile(x, y, floor);
}

void Generator::carvePath(const Point& t_from, const Point& t_to)
{
	std::vector<Point> line = getLine(t_from, t_to);

	for (auto& i : line)
	{
		map->setTile(i, floor);

		if (i.x + 1 < width - 1)
			map->setTile(i.x + 1, i.y, floor);
		if (i.y + 1 < height - 1)
			map->setTile(i.x, i.y + 1, floor);
	}
}

void Generator::carveCorridor(const Point& t_from, const Point& t_to)
{
	Point delta = t_to - t_from;
	Point primaryIncrement(sign(delta.x), 0);
	Point secondaryIncrement(0, sign(delta.y));
	int primary = std::abs(delta.x);
	int secondary = std::abs(delta.y);

	if (rng->getBool())
	{
		std::swap(primary, secondary);
		std::swap(primaryIncrement, secondaryIncrement);
	}

	std::vector<Point> line;
	Point current = t_from;
	int windingPoint = -1;

	if (primary > 1 && rng->getBool())
	{
		// windingPoint = rng->getInt(primary);
		
		// HACK: To avoid unpretty corridors
		windingPoint = even(rng->getInt(primary));
	}

	while (true)
	{
		line.emplace_back(current);

		if (primary > 0 && (primary != windingPoint || secondary == 0))
		{
			current += primaryIncrement;
			primary -= 1;
		}

		else if (secondary > 0)
		{
			current += secondaryIncrement;
			secondary -= 1;
		}

		else
		{
			assert(current == t_to);
			break;
		}
	}

	for (std::size_t i = 1; i < line.size() - 1; ++i)
	{
		if (map->getTile(line[i]) != floor) // REMOVE:
			map->setTile(line[i], corridor);
	}
}

void Generator::carveCircle(const Point& t_center, const int t_radius)
{
	const int left = std::max(1, t_center.x - t_radius);
	const int top = std::max(1, t_center.y - t_radius);
	const int right = std::min(t_center.x + t_radius, width - 2);
	const int bottom = std::min(t_center.y + t_radius, height - 2);

	for (int y = top; y <= bottom; ++y)
		for (int x = left; x <= right; ++x)
		{
			// NOTE: < or <=
			if (lengthSquared(Point(x, y) - t_center) <= t_radius * t_radius)
				map->setTile(x, y, floor);
		}
}

void Generator::carveWindingRoad(const Point& t_from, const Point& t_to, const int t_perturbation) const
{

	// The square of the cosine of the angle between vectors p0p1 and p1p2,
	// with the sign of the cosine, in permil (1.0 = 1000).
	auto signcos2 = [] (const Point& p0, const Point& p1, const Point& p2)
	{
		const int sqlen01 = lengthSquared(p1 - p0);
		const int sqlen12 = lengthSquared(p2 - p1);
		const int prod = (p1.x - p0.x) * (p2.x - p1.x) + (p1.y - p0.y) * (p2.y - p1.y);
		const int val = 1000 * (prod * prod / sqlen01) / sqlen12;

		return prod < 0 ? -val : val;
	};

	std::vector<Point> line = getLine(t_from, t_to);

	if (line.size() >= 5)
	{
		std::size_t j = 0;

		for (std::size_t i = 0; i < line.size(); )
		{
			line[j++] = line[i];

			if (i < line.size() - 5 || i >= line.size() - 1)
				i += rng->getInt(2, 3);
			else if (i == line.size() - 5)
				i += 2;
			else
				i = line.size() - 1;
		}

		line.resize(j);

		if (line.size() >= 3)
		{
			constexpr int mind2 = 2 * 2; // mindist = 2
			constexpr int maxd2 = 5 * 5; // maxdist = 5
			constexpr int mincos2 = 500; // cos^2 in 1/1000, for angles < 45 degrees

			for (std::size_t i = 0; i < j * t_perturbation; ++i)
			{
				const std::size_t ri = 1 + rng->getInt(j - 2);
				Direction rdir = Direction::All[rng->getInt(8)];
				Point rpos = line[ri] + rdir;

				const int lod2 = lengthSquared(rpos - line[ri - 1]);
				const int hid2 = lengthSquared(rpos - line[ri + 1]);

				if (!map->isInBounds(rpos) ||
					lod2 < mind2 || lod2 > maxd2 ||
					hid2 < mind2 || hid2 > maxd2)
					continue;

				if (signcos2(line[ri - 1], rpos, line[ri + 1]) < mincos2)
					continue;

				if (ri > 1 && signcos2(line[ri - 2], line[ri - 1], rpos) < mincos2)
					continue;

				if (ri < line.size() - 2 && signcos2(rpos, line[ri + 1], line[ri + 2]) < mincos2)
					continue;

				line[ri] = rpos;
			}
		}
	}

	for (std::size_t i = 0; i < line.size() - 1; ++i)
	{
		auto subline = getLine(line[i], line[i + 1]);

		for (const auto& point : subline)
		{
			constexpr int radius = 1;
			const int left = std::max(0, point.x - radius);
			const int top = std::max(0, point.y - radius);
			const int right = std::min(point.x + radius, width - 1);
			const int bottom = std::min(point.y + radius, height - 1);

			for (int y = top; y <= bottom; ++y)
				for (int x = left; x <= right; ++x)
				{
					if (lengthSquared(Point(x, y) - point) <= radius * radius)
						map->setTile(x, y, floor);
				}
		}
	}
}

void Generator::extendLine(Point& t_from, Point& t_to)
{
	Point delta = t_to - t_from;
	Point primaryIncrement(sign(delta.x), 0);
	Point secondaryIncrement(0, sign(delta.y));
	int primary = std::abs(delta.x);
	int secondary = std::abs(delta.y);

	if (secondary > primary)
	{
		std::swap(primary, secondary);
		std::swap(primaryIncrement, secondaryIncrement);
	}

	int error = 0;

	while (t_from.x > 0 && t_from.x < width - 1 && t_from.y > 0 && t_from.y < height - 1)
	{
		t_from -= primaryIncrement;
		error += secondary;

		if (error * 2 >= primary)
		{
			t_from -= secondaryIncrement;
			error -= primary;
		}
	}

	error = 0;

	while (t_to.x > 0 && t_to.x < width - 1 && t_to.y > 0 && t_to.y < height - 1)
	{
		t_to += primaryIncrement;
		error += secondary;

		if (error * 2 >= primary)
		{
			t_to += secondaryIncrement;
			error -= primary;
		}
	}
}

void Generator::erode(int t_iterations)
{
	for (int i = 0; i < t_iterations; ++i)
	{
		int x = rng->getInt(1, width - 2);
		int y = rng->getInt(1, height - 2);

		if (map->getTile(x, y) != wall)
			continue;

		int floors = 0;

		for (const auto& dir : Direction::All)
		{
			if (map->getTile(x + dir.x, y + dir.y) == floor)
				floors += 1;
		}

		if (floors >= 2 && rng->getInt(9 - floors) == 0)
			map->setTile(x, y, floor);
	}
}

void Generator::erodeTiles(ETile t_from, ETile t_to, int t_r1Cutoff)
{
	std::vector<bool> tiles(width * height, false);

	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (map->getTile(x, y) != t_from)
				continue;

			int r1 = countTiles(t_to, x, y);

			if (r1 >= t_r1Cutoff)
				tiles[x + y * width] = true;
		}

	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			if (tiles[x + y * width])
				map->setTile(x, y, t_to);
		}
}

void Generator::removeWalls()
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			if (map->getTile(x, y) != wall)
				continue;

			bool removeWall = true;

			for (const auto& dir : Direction::All)
			{
				if (map->isInBounds(x + dir.x, y + dir.y) &&
					map->getTile(x + dir.x, y + dir.y) != wall &&
					map->getTile(x + dir.x, y + dir.y) != bridge &&
					map->getTile(x + dir.x, y + dir.y) != unused)
				{
					removeWall = false;
					break;
				}
			}

			if (removeWall)
				map->setTile(x, y, unused);
		}
}

void Generator::placeStaircase() const
{
	// select a random floor tile, if the tile is not surrounded by walls, replace it with a staircase

	bool isStaircasePlaced = false;
	while (!isStaircasePlaced)
	{
		// Randomly select a position for the staircase
		int x = rng->getInt(0, width - 1);
		int y = rng->getInt(0, height - 1);
	
		// Check if the selected position is a floor tile
		if (map->getTile(x, y) == ETile::FLOOR || map->getTile(x, y) == ETile::DIRT)
		{
			// Place the staircase
			map->setTile(x, y, staircase);
	
			// Indicate that the staircase has been placed
			isStaircasePlaced = true;
		}
	}
}

int Generator::countTiles(ETile t_tile, int t_x, int t_y) const
{
	int count = 0;

	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx)
		{
			if (map->getTile(t_x + dx, t_y + dy) == t_tile)
				count += 1;
		}

	return count;
}
