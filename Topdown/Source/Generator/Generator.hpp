#pragma once

#include "../Map/Tile.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <string>
#include <vector>

class Map;
class Rng;

class Generator
{
public:
	using Room = sf::IntRect;
	using Point = sf::Vector2i;

	enum class EPathType
	{
		STRAIGHT,
		CORRIDOR,
		WINDING_ROAD,
	};

public:
	virtual ~Generator() = default;

	[[nodiscard]] const std::string& getName() const;

	void generate(Map& t_map, Rng& t_rng);

protected:
	void setName(const std::string& t_name);

	// Cellular automata
	void generation(int t_r1Cutoff);
	void generation(int t_r1Cutoff, int t_r2Cutoff);

	// Remove or connect unconnected regions
	void removeRegions(int t_removeProb = 100, int t_minSize = 0);
	void connectRegions(int t_minSize = 0, EPathType t_type = EPathType::STRAIGHT, bool t_allowDiagonalSteps = true);
	void constructBridges(int t_minSize = 0);

	// Lloyd's algorithm (Voronoi iteration)
	void relaxation(std::vector<Point>& t_points);
	void connectPoints(std::vector<Point>& t_points, EPathType t_type = EPathType::STRAIGHT);

	void fill(int t_wallProb);

	[[nodiscard]] bool canCarve(const Room& t_room) const;
	void carveRoom(const Room& t_room);

	void carvePath(const Point& t_from, const Point& t_to);
	void carveCorridor(const Point& t_from, const Point& t_to);
	void carveCircle(const Point& t_center, int t_radius);

	// Used for lava river generation
	void carveWindingRoad(const Point& t_from, const Point& t_to, int t_perturbation = 10) const;
	void extendLine(Point& t_from, Point& t_to);

	void erode(int t_iterations);
	void erodeTiles(ETile t_from, ETile t_to, int t_r1Cutoff);

	void removeWalls(); // remove unused walls

	void placeStaircase() const;

private:
	virtual void onGenerate() = 0;

	[[nodiscard]] int countTiles(ETile t_tile, int t_x, int t_y) const; // count adjacent tiles

// private:
protected:
	Map* map = nullptr;
	Rng* rng = nullptr;

	int width = 0;
	int height = 0;

	ETile unused = ETile::UNUSED;
	ETile floor = ETile::FLOOR;
	ETile corridor = ETile::CORRIDOR;
	ETile wall = ETile::WALL;
	ETile water = ETile::LAVA;
	ETile bridge = ETile::BRIDGE;
	ETile staircase = ETile::DOWN_STAIRS;

private:
	std::string m_name;
};
