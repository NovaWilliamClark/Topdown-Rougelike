#include "GameplayScene.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

#include "Generator/Cave.hpp"
#include "Generator/Dungeon.hpp"

#include "Scene/SceneManager.hpp"

#include "Utility/Constants.hpp"

GameplayScene::GameplayScene(const bool t_shouldLoad) : m_map(g_mapSize.x, g_mapSize.y)
{
    // Load Map correct map via seed
    if (t_shouldLoad) // Load from save Data, i.e read player data and seed
    {
        // TODO: READ save data Load from save data, seed the generator
    }
    else
    {
        m_rng.setSeed(std::random_device()());
    }
    m_texture.loadFromFile("Content/Tiles/tiny_dungeon_world.png");
    m_font.loadFromFile("Content/Fonts/PixelAr.ttf");

    m_text.setFont(m_font);
    m_text.setCharacterSize(16);
    m_text.setOutlineThickness(1.f);
    m_text.setScale(0.5f, 0.5f);

    m_tilemap = std::make_unique<Tilemap>(m_texture, g_tileSize);
    m_player = std::make_shared<Player>(sf::Vector2f{ 0, 0 }, &m_map, this);

    m_entities.push_back(m_player);

    registerGenerators();
    selectRandomGenerator();
    generateMap();


    // TODO fix spawn, spawning occasionally in a wall
    // Create player
    if (t_shouldLoad)
    {
        // TODO: Generate player from save data and place on map
    }
    else
    {
        m_player->setSpawnPosition(m_map);
    }
}

void GameplayScene::update(const float t_deltaTime)
{
    if (m_player)
    {
        m_player->update(t_deltaTime);
    }

    for (const auto& enemy : m_enemies)
    {
        if (enemy)
        {
            enemy->update(t_deltaTime);
        }
    }

    m_text.setString("Floor: " + std::to_string(SceneManager::getInstance().getLevelReached()));
}

void GameplayScene::draw(sf::RenderWindow& t_target)
{
    t_target.draw(*m_tilemap);

    if (m_player)
    {
        m_player->draw(t_target);
    }

    if (m_key)
    {
        m_key->draw(t_target);
    }

    for (const auto& enemy : m_enemies)
    {
        if (enemy)
        {
            enemy->draw(t_target);
        }
    }

    sf::View view;
    if(!m_mapVisibility)
    {
        view = sf::View(sf::FloatRect(0.f, 0.f, g_windowSize.x / 3, g_windowSize.y / 3));
        view.setCenter({ m_player->getPosition().x * g_tileSize.x, m_player->getPosition().y * g_tileSize.y });
        m_text.setCharacterSize(15);
        m_text.setPosition(view.getCenter().x - g_windowSize.x / 6 + 5, view.getCenter().y - g_windowSize.y / 6 + 5);
    }
    else
    {
        view = sf::View(sf::FloatRect(0.f, 0.f, g_windowSize.x, g_windowSize.y));
        view.setCenter(g_windowSize.x / 2, g_windowSize.y / 2);
        m_text.setCharacterSize(32);
        m_text.setPosition(view.getCenter().x - g_windowSize.x / 2 + 20, view.getCenter().y - g_windowSize.y / 2 + 20);
    }

    t_target.setView(view);
    t_target.draw(m_text);
    drawDebug(t_target);
}

void GameplayScene::handleInput(const sf::Event& t_event)
{
    if (m_player)
    {
        m_player->handleInput();
    }
}

void GameplayScene::selectRandomGenerator()
{
    m_currentGenerator = m_rng.getInt(0, m_generators.size() - 1);
}

void GameplayScene::generateMap()
{
    SceneManager::getInstance().addLevelReached();

    m_enemies.clear();
    m_entities.clear();
    m_generators[m_currentGenerator]->generate(m_map, m_rng);
    m_tilemap->load(m_map);

    // Push Back Player to entities again
    m_entities.push_back(m_player);

    // Generate a random number of enemies
    const int enemyCount = m_rng.getInt(1, 10);
    for (int i = 0; i < enemyCount; ++i)
    {
        m_enemies.emplace_back(std::make_shared<Enemy>(sf::Vector2f{ 0, 0 }, &m_map, this));
        m_enemies[i]->setSpawnPosition(m_map);
        m_entities.push_back(m_enemies[i]);
    }

    m_key = std::make_unique<Key>(sf::Vector2f{ 0, 0 }, &m_map, this);
    m_key->setSpawnPosition(m_map);
}

std::vector<std::shared_ptr<Enemy>>& GameplayScene::getEnemies()
{
    return m_enemies;
}

void GameplayScene::drawDebug(sf::RenderWindow& t_target)
{
    //ImGui::SFML::Render(t_target);
    ImGui::Begin("Debug Menu");

    // Let's assume that entities have a unique ID, and that they have getter and setter methods for their position.
    for (const auto& entity : m_entities)
    {
        if (ImGui::TreeNode(entity->getName().c_str()))
        {
            sf::Vector2f position = entity->getPosition();
            if (ImGui::InputFloat2("Position", &position.x))
            {
                entity->setPosition(position);
            }

            
            if (auto* player = dynamic_cast<Player*>(entity.get()))
            {
                bool isInvincible = player->getIsInvincible();
                bool hasKey = player->getHasKey();
                float speed = player->getSpeed();
                // Make invincible
                if (ImGui::Checkbox("Is Invincible", &isInvincible))
                {
                    player->setInvincible(isInvincible);
                }

                if (ImGui::Checkbox("Has Key", &hasKey))
                {
                    player->setHasKey(hasKey);
                }

                if (ImGui::DragFloat("Speed", &speed, 0.1f, 0.001f, 20))
                {
                    player->setSpeed(speed);
                }
            }
            // You can add more controls for other properties here...

            ImGui::TreePop();
        }
    }

    if (ImGui::Checkbox("View Entire Map", &m_mapVisibility))
    {
    }

    ImGui::Text(" %s", m_generators[m_currentGenerator]->getName().c_str());
    ImGui::Text("Seed: %d", m_rng.getSeed());

    ImGui::End();
}

bool GameplayScene::getMapVisibility() const
{
    return m_mapVisibility;
}

std::shared_ptr<Key>& GameplayScene::getKey()
{
    return m_key;
}


void GameplayScene::registerGenerators()
{
    m_generators.emplace_back(std::make_unique<BigCave>());
    m_generators.emplace_back(std::make_unique<ConnectedCaves>());

    m_generators.emplace_back(std::make_unique<CavesAndLava>());
    m_generators.emplace_back(std::make_unique<CavesAndRooms>());

    m_generators.emplace_back(std::make_unique<ClassicDungeon>());
}
