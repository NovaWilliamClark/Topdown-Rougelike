#include "Player.hpp"

#include <iostream>

#include "Input/InputManager.hpp"
#include "Map/Map.hpp"

#include "Scene/SceneManager.hpp"
#include "Scene/Gameplay/GameplayScene.hpp"
#include "Utility/Constants.hpp"
#include "Utility/Rng.hpp"

Player::Player(const sf::Vector2f& t_position, Map* t_map, GameplayScene* t_scene)
{
    // Set player texture, size, and origin
    sf::Texture texture;
    texture.loadFromFile("Content/Sprites/8d_character.png");
    setTexture(texture);

    setSize(sf::Vector2f(g_tileSize)); // Set player size
    setOrigin(sf::Vector2f(g_tileSize) / 2.f); // Set player origin to center
    setPosition(t_position);

    m_speed = 0.1f;

    m_map = t_map;
    m_scene = t_scene;
}

Player::~Player()
= default;


void Player::handleInput()
{
    m_velocity = sf::Vector2f(0.f, 0.f); // Reset velocity

    // Check if a joystick is connected
    if (sf::Joystick::isConnected(0)) {
        // Get joystick axis positions
        const float joystickX = InputManager::getAxisPosition(0, sf::Joystick::X);
        const float joystickY = InputManager::getAxisPosition(0, sf::Joystick::Y);

        // Set velocity based on joystick values
        if (joystickX < -DEAD_ZONE) {
            m_velocity.x = -m_speed;
        }
        else if (joystickX > DEAD_ZONE) {
            m_velocity.x = m_speed;
        }

        if (joystickY < -DEAD_ZONE) {
            m_velocity.y = -m_speed;
        }
        else if (joystickY > DEAD_ZONE) {
            m_velocity.y = m_speed;
        }
    }
}

void Player::update(const float t_deltaTime)
{
    handleCollision(*m_map);
    setPosition(position + m_velocity);
}

// VERY Bloated, badly structured 5 min collision detection ;-; I am sorry (it gets the job done)
bool Player::isColliding(const Map& t_map, const sf::Vector2f& t_nextPosition)
{
    const sf::Rect<float> nextPositionX(position.x + m_velocity.x - 8.f / g_tileSize.x, position.y - 2.f / g_tileSize.y, 16.f / g_tileSize.x, 18.f / g_tileSize.y);
    const sf::Rect<float> nextPositionY(position.x - 8.f / g_tileSize.x, position.y + m_velocity.y - 2.f / g_tileSize.y, 16.f / g_tileSize.x, 18.f / g_tileSize.y);

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

bool Player::isCollidingWithStaircase(const Map& t_map) const
{
    // Create a rectangle for the player's current position
    sf::Rect<float> playerRect(position.x - 12.f / g_tileSize.x, position.y - 12.f / g_tileSize.y, 24.f / g_tileSize.x, 24.f / g_tileSize.y);

    return (t_map.getTile(playerRect.left, playerRect.top) == ETile::DOWN_STAIRS ||
        t_map.getTile(playerRect.left + playerRect.width, playerRect.top) == ETile::DOWN_STAIRS ||
        t_map.getTile(playerRect.left, playerRect.top + playerRect.height) == ETile::DOWN_STAIRS ||
        t_map.getTile(playerRect.left + playerRect.width, playerRect.top + playerRect.height) == ETile::DOWN_STAIRS);
}

bool Player::isCollidingWithEnemy(const Map& t_map) const
{
    const auto& enemies = m_scene->getEnemies();

    // Create a rectangle for the player's current position
    sf::Rect<float> playerRect(position.x - 12.f / g_tileSize.x, position.y - 12.f / g_tileSize.y, 24.f / g_tileSize.x, 24.f / g_tileSize.y);

    // Check each enemy
    return std::ranges::any_of(enemies, [&playerRect](const auto& t_enemy) {
        // Create a rectangle for the enemy's current position
        const sf::Vector2f enemyPos = t_enemy->getPosition();
        const sf::Rect<float> enemyRect(enemyPos.x - 12.f / g_tileSize.x, enemyPos.y - 12.f / g_tileSize.y, 24.f / g_tileSize.x, 24.f / g_tileSize.y);

        // Check if the player's rectangle intersects with the enemy's rectangle
        return playerRect.intersects(enemyRect);
        });
}

bool Player::isCollidingWithKey(const Map& t_map) const
{
    const auto& key = m_scene->getKey();

    // Create a rectangle for the player's current position
    const sf::Rect<float> playerRect(position.x - 12.f / g_tileSize.x, position.y - 12.f / g_tileSize.y, 24.f / g_tileSize.x, 24.f / g_tileSize.y);
    // Create a rectangle for the enemy's current position
    const sf::Vector2f keyPos = key->getPosition();
    const sf::Rect<float> enemyRect(keyPos.x - 12.f / g_tileSize.x, keyPos.y - 12.f / g_tileSize.y, 24.f / g_tileSize.x, 24.f / g_tileSize.y);
    
    // Check if the player's rectangle intersects with the enemy's rectangle
    return playerRect.intersects(enemyRect);
}

void Player::setSpawnPosition(const Map& t_map)
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

void Player::setInvincible(const bool t_isInvincible)
{
    m_isInvincible = t_isInvincible;
}

void Player::setHasKey(const bool t_hasKey)
{
    m_hasKey = t_hasKey;
}

bool Player::getHasKey() const
{
    return m_hasKey;
}

bool Player::getIsInvincible() const
{
    return m_isInvincible;
}

void Player::setSpeed(const float t_val)
{
    m_speed = t_val;
}

float Player::getSpeed()
{
    return m_speed;
}

void Player::handleCollision(const Map& t_map)
{
    // Get the player's next position
    const auto nextPosition = position + m_velocity;

    if (isColliding(t_map, nextPosition))
    {

    }
    if (isCollidingWithStaircase(t_map))
    {
        if (m_hasKey)
        {
            m_scene->selectRandomGenerator();
            m_scene->generateMap();
            setSpawnPosition(t_map);
        }
    }
    if (isCollidingWithEnemy(t_map))
    {
        if (!m_isInvincible)
        {
            SceneManager::getInstance().switchTo("Gameover");
        }
        std::cout << "Collided with enemy" << std::endl;
    }
    if (isCollidingWithKey(t_map))
    {
        m_hasKey = true;
        m_scene->getKey()->setCollected(true);
    }
}
