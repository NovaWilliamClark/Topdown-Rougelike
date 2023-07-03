#pragma once
#include <SFML/System/Vector2.hpp>

static const sf::Vector2i g_windowSize(1280, 720);
static const sf::Vector2i g_tileSize(24, 24);
static const sf::Vector2i g_mapSize(g_windowSize.x / g_tileSize.x, g_windowSize.y / g_tileSize.y);

constexpr int MIN_ROOM_SIZE = 3;
constexpr int MAX_ROOM_SIZE = 12;

constexpr float TIME_PER_FRAME = 1.f / 60.f;
constexpr float DEAD_ZONE = 15.f;

constexpr float EULER = 2.71828182845904523536f;
constexpr float LOG2_E = 1.44269504088896340736f;
constexpr float LOG10_E = 0.434294481903251827651f;
constexpr float LN2 = 0.693147180559945309417f;
constexpr float LN10 = 2.30258509299404568402f;
constexpr float PI = 3.14159265358979323846f;
constexpr float PI_2 = 1.57079632679489661923f;
constexpr float PI_4 = 0.785398163397448309616f;
constexpr float	SQRT_PI = 1.12837916709551257390f;
constexpr float SQRT2 = 1.41421356237309504880f;