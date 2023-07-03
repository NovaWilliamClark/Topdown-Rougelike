#include "Enemy.hpp"
#include "Map/Map.hpp"
#include "Scene/Gameplay/GameplayScene.hpp"
#include "Utility/Rng.hpp"

#include "Utility/Constants.hpp"

Enemy::Enemy(const sf::Vector2f& t_position, Map* t_map, GameplayScene* t_scene)
{
    // Set player texture, size, and origin
    sf::Texture texture;
    texture.loadFromFile("Content/Sprites/Bat.png");
    setTexture(texture);

    setSize(sf::Vector2f(g_tileSize)); // Set player size
    setOrigin(sf::Vector2f(g_tileSize) / 2.f); // Set player origin to center
    setPosition(t_position);

    m_speed = 0.08f;

    m_map = t_map;
    m_scene = t_scene;

    m_changeDirectionDelay = sf::seconds(0.3f);
    m_elapsedChangeDirection = sf::Time::Zero;
}

void Enemy::handleCollision(const Map& t_map)
{
    const auto nextPosition = position + m_velocity;
    isColliding(t_map, nextPosition);
}

void Enemy::update(const float t_deltaTime)
{

    // Update the elapsed time for changing direction
    m_elapsedChangeDirection += sf::seconds(t_deltaTime);

    // Check if it's time to change direction
    if (m_elapsedChangeDirection >= m_changeDirectionDelay)
    {
        // Wander randomly
        wander();

        // Reset the elapsed time for changing direction
        m_elapsedChangeDirection = sf::Time::Zero;
    }

    handleCollision(*m_map);
    setPosition(position + m_velocity);
}

// VERY Bloated, badly structured 5 min collision detection ;-; I am sorry (it gets the job done)
bool Enemy::isColliding(const Map& t_map, const sf::Vector2f& t_nextPosition)

{
    const sf::Rect<float> nextPositionX(position.x + m_velocity.x - 8.f / g_tileSize.x, position.y - 2.f / g_tileSize.y, 8.f / g_tileSize.x, 8.f / g_tileSize.y);
    const sf::Rect<float> nextPositionY(position.x - 8.f / g_tileSize.x, position.y + m_velocity.y - 2.f / g_tileSize.y, 8.f / g_tileSize.x, 8.f / g_tileSize.y);

    // Check the four corners for X direction
    if (t_map.canCollide(nextPositionX.left, nextPositionX.top) || t_map.canCollide(nextPositionX.left + nextPositionX.width, nextPositionX.top) ||
        t_map.canCollide(nextPositionX.left, nextPositionX.top + nextPositionX.height) || t_map.canCollide(nextPositionX.left + nextPositionX.width, nextPositionX.top + nextPositionX.height)) {
        // If it would collide, then stop the player's movement in X direction
        m_velocity.x = 0.f;



        if (t_map.canCollide(nextPositionY.left, nextPositionY.top) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top) ||
            t_map.canCollide(nextPositionY.left, nextPositionY.top + nextPositionY.height) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top + nextPositionY.height)) {
            // If it would collide, then stop the player's movement in Y direction
            m_velocity.y = 0.f;
            return true;
        }
    }

    // Check the four corners for Y direction
    if (t_map.canCollide(nextPositionY.left, nextPositionY.top) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top) ||
        t_map.canCollide(nextPositionY.left, nextPositionY.top + nextPositionY.height) || t_map.canCollide(nextPositionY.left + nextPositionY.width, nextPositionY.top + nextPositionY.height)) {
        // If it would collide, then stop the player's movement in Y direction
        m_velocity.y = 0.f;
        return true;
    }

    return false;
}

void Enemy::setSpawnPosition(const Map& t_map)
{// Get a random position on the map
    int x = 0;
    int y = 0;
    while (isColliding(t_map, sf::Vector2f(x, y)))
    {
        x = Rng::getInstance().getInt(0, m_map->width - 1);
        y = Rng::getInstance().getInt(0, m_map->height - 1);
        setPosition(sf::Vector2f(x, y));
    }
}

void Enemy::setVelocity(const sf::Vector2f& t_velocity)
{
    m_velocity = t_velocity;
}

void Enemy::wander()
{
    // Generate random directions for wandering
    int xDir = Rng::getInstance().getInt(-1, 1);
    int yDir = Rng::getInstance().getInt(-1, 1);

    // Set the new velocity based on the random directions
    m_velocity = sf::Vector2f(xDir * m_speed, yDir * m_speed);
}
