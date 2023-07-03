#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class GameplayScene;
class Map;

class Player final : public Entity {
public:
    explicit Player(const sf::Vector2f& t_position, Map* t_map, GameplayScene* t_scene);
    ~Player() override;

    void handleInput();
    void handleCollision(const Map& t_map);
    void update(float t_deltaTime) override;

    bool isColliding(const Map& t_map, const sf::Vector2f& t_nextPosition);
    void setSpawnPosition(const Map& t_map);

    void setInvincible(bool t_isInvincible);
    void setHasKey(bool t_hasKey);

    bool getHasKey() const;
    bool getIsInvincible() const;

    void setSpeed(float t_val);
    float getSpeed();

private:
    bool isCollidingWithStaircase(const Map& t_map) const;
    bool isCollidingWithEnemy(const Map& t_map) const;
    bool isCollidingWithKey(const Map& t_map) const;

    sf::Vector2f m_velocity{};
    float m_speed{};
    Map* m_map;
    GameplayScene* m_scene;

    bool m_hasKey = false;
    bool m_isInvincible = false;

};
