#pragma once
#include <SFML/Graphics.hpp>
#include "Scene/Scene.hpp"

class TitleScene final : public Scene {
public:
    TitleScene();

    void update(float t_deltaTime) override;
    void draw(sf::RenderWindow& t_target) override;
    void handleInput(const sf::Event& t_event) override;

private:
    std::vector<sf::Text> m_texts;
    sf::Font m_font;
};