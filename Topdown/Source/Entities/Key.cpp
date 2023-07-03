#include "Key.hpp"

#include "Map/Map.hpp"
#include "Scene/Gameplay/GameplayScene.hpp"
#include "Utility/Constants.hpp"

Key::Key(const sf::Vector2f& t_position, Map* t_map, GameplayScene* t_scene)
{
    // Set player texture, size, and origin
    sf::Texture texture;
    texture.loadFromFile("Content/Sprites/Key.png");
    setTexture(texture);

    setSize(sf::Vector2f(g_tileSize)); // Set player size
    setOrigin(sf::Vector2f(g_tileSize) / 2.f); // Set player origin to center
    setPosition(t_position);

    m_map = t_map;
    m_scene = t_scene;
}

bool Key::isColliding(const Map& t_map, const sf::Vector2f& t_nextPosition)
{
    const sf::Rect<float> nextPositionX(position.x + 0 - 8.f / g_tileSize.x, position.y - 2.f / g_tileSize.y, 16.f / g_tileSize.x, 18.f / g_tileSize.y);
    const sf::Rect<float> nextPositionY(position.x - 8.f / g_tileSize.x, position.y + 0 - 2.f / g_tileSize.y, 16.f / g_tileSize.x, 18.f / g_tileSize.y);

    // Check the four corners for X direction
    if (t_map.canCollide(nextPositionX.left, nextPositionX.top) || t_map.canCollide(nextPositionX.left + nextPositionX.width, nextPositionX.top) ||
        t_map.canCollide(nextPositionX.left, nextPositionX.top + nextPositionX.height) || t_map.canCollide(nextPositionX.left + nextPositionX.width, nextPositionX.top + nextPositionX.height)) {



        if (t_map.canCollide(nextPositionY.left, nextPositionY.top) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top) ||
            t_map.canCollide(nextPositionY.left, nextPositionY.top + nextPositionY.height) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top + nextPositionY.height)) {
            // If it would collide, then stop the player's movement in Y direction
            return true;
        }
    }

    // Check the four corners for Y direction
    if (t_map.canCollide(nextPositionY.left, nextPositionY.top) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top) ||
        t_map.canCollide(nextPositionY.left, nextPositionY.top + nextPositionY.height) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top + nextPositionY.height)) {
        // If it would collide, then stop the player's movement in Y direction
        return true;
    }

    return false;
}

void Key::setSpawnPosition(const Map& t_map)
{

    int x = 0;
    int y = 0;
    while (isColliding(t_map, sf::Vector2f(x, y)))
    {
        x = Rng::getInstance().getInt(0, m_map->width - 1);
        y = Rng::getInstance().getInt(0, m_map->height - 1);
        setPosition(sf::Vector2f(x, y));
    }
}

void Key::update(float t_deltaTime)
{
}

void Key::setCollected(const bool t_isCollected)
{
    m_isCollected = t_isCollected;
}

void Key::draw(sf::RenderWindow& t_window) const
{
    if (!m_isCollected)
    {
        Entity::draw(t_window);
    }
    return;
}
