#pragma once
#include "Entities/Entity.hpp"

class Map;
class GameplayScene;

class Key : public Entity
{
public:
    explicit Key(const sf::Vector2f& t_position, Map* t_map, GameplayScene* t_scene);
    ~Key() override = default;

    bool isColliding(const Map& t_map, const sf::Vector2f& t_nextPosition);
    void setSpawnPosition(const Map& t_map);

    void update(float t_deltaTime) override;

    void setCollected(bool t_isCollected);

    void draw(sf::RenderWindow& t_window) const override;

private:
    Map* m_map;
    GameplayScene* m_scene;

    bool m_isCollected = false;
};