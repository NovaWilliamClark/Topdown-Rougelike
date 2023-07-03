#pragma once
#include "Application/Tilemap.hpp"

#include "Entities/Enemy.hpp"
#include "Entities/Key.hpp"
#include "Entities/Player.hpp"

#include "Generator/Generator.hpp"
#include "Map/Map.hpp"
#include "Scene/Scene.hpp"
#include "Utility/Rng.hpp"

class GameplayScene final : public Scene
{
public:
    explicit GameplayScene(bool t_shouldLoad);

    void update(float t_deltaTime) override;
    void draw(sf::RenderWindow& t_target) override;
    void handleInput(const sf::Event& t_event) override;

    void selectRandomGenerator();
    void generateMap();
    std::vector<std::shared_ptr<Enemy>>& getEnemies();
    void drawDebug(sf::RenderWindow& t_target);

    bool getMapVisibility() const;
    std::shared_ptr<Key>& getKey();

private:

    void registerGenerators();

    sf::Texture m_texture;
    sf::Font m_font;

    sf::Text m_text;

    Map m_map;
    Rng m_rng;

    std::unique_ptr<Tilemap> m_tilemap;

    std::vector<std::unique_ptr<Generator>> m_generators;
    std::size_t m_currentGenerator = 0;

    std::shared_ptr<Key> m_key;
    std::shared_ptr<Player> m_player;
    std::vector<std::shared_ptr<Enemy>> m_enemies;

    std::vector<std::shared_ptr<Entity>> m_entities;

    bool m_mapVisibility;
};

