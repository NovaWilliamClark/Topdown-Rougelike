#pragma once
#include <SFML/Graphics.hpp>
#include "../Scene.hpp"

class MainMenuScene final : public Scene {
public:
    MainMenuScene();

    void update(float t_deltaTime) override;
    void draw(sf::RenderWindow& t_target) override;
    void handleInput(const sf::Event& t_event) override;

private:
    void selectNextItem();
    void selectPreviousItem();
    void activateSelectedItem() const;

    std::vector<sf::Text> m_menuItems;
    int m_selectedItemIndex;
    sf::Font m_font;
};

