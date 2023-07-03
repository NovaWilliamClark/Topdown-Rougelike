#pragma once
#include "Entities/Entity.hpp"
#include <SFML/System/Time.hpp>

class GameplayScene;
class Map;

class Enemy final : public Entity
{
public:
    explicit Enemy(const sf::Vector2f& t_position, Map* t_map, GameplayScene* t_scene);
    ~Enemy() = default;

    void handleCollision(const Map& t_map);
    void update(float t_deltaTime) override;

    bool isColliding(const Map& t_map, const sf::Vector2f& t_nextPosition);
    void setSpawnPosition(const Map& t_map);
    void setVelocity(const sf::Vector2f& t_velocity);

private:
    void wander();
    sf::Vector2f m_velocity{};
    float m_speed{};
    Map* m_map;
    GameplayScene* m_scene;
    bool m_hasKey = true;

           // Set initial delay values
    sf::Time m_changeDirectionDelay;
    sf::Time m_elapsedChangeDirection;
};

